#pragma once

#include "bit_manager.h"

namespace managers
{
	namespace detail
	{
		static inline char* char_cast( void* p )
		{
			return static_cast< char* >( p );
		}

		static inline const char* char_cast( const void* p )
		{
			return static_cast< const char* >( p );
		}

		static inline ptrdiff_t diff( const void* p1, const void* p2 )
		{
			return char_cast(p1) - char_cast(p2);
		}

		static inline char* shift( void* p, size_t offset )
		{
			return char_cast(p) + offset;
		}

// 		static inline const char* shift( const void* p, const size_t offset )
// 		{
// 			return char_cast(p) + offset;
// 		}
	}

	

	//Memory manager
	//TODO: Make memory manager a singleton
	template< class BlockType, size_t MemorySize, size_t ChunkSize >
	class memory_manager
	{
		enum
		{
			chunk_size = ChunkSize,
			memory_size = MemorySize,
			num_chunks = memory_size / chunk_size			
		};
		typedef bit_manager<BlockType, num_chunks> bitmgr_t;
		bitmgr_t m_bitmgr;

		char* m_membase;

	public:

		typedef typename bitmgr_t::block_ptr_type		block_ptr_type;		
		typedef typename bitmgr_t::size_type			size_type;
		typedef memory_manager<BlockType, MemorySize, ChunkSize> self_type;
		//Offset class
		class ptr_t
		{
			typedef self_type mgr_t;

			size_type m_offset;
			friend mgr_t;

			const size_type get_offset() const
			{
				return m_offset;
			}
		public:
			explicit ptr_t( const size_type offset )
				:m_offset( offset )
			{}

			explicit ptr_t( const void* ptr )
				//TODO: Retrieve real memory base address here
				:m_offset( detail::diff( 0, ptr ) )
			{}
			
			void* get() const
			{
				//TODO: Retrieve real memory base address here
				//and use it in shift call
				return detail::shift( 0, m_offset );
			}
		};

		explicit memory_manager( void* mem_base )
			:m_bitmgr( static_cast< block_ptr_type >( mem_base ) )
		{
			m_membase = detail::shift( mem_base, bitmgr_t::memory_usage );
		}

		ptr_t allocate( size_type size )
		{
			return ptr_t( m_bitmgr.allocate( chunks_required( size ) ) * chunk_size );
		}

 		void deallocate( const ptr_t off, size_type size )
 		{
 			m_bitmgr.deallocate( chunk_index( off.get_offset() ), chunks_required( size ) );
 		}

	private:
		static inline size_type chunk_index( size_type size )
		{ return size / chunk_size; }

		static inline size_type chunks_count( size_type size )
		{ return chunk_index( size ); }

		static inline size_type extra_bytes( size_type size )
		{ return size % chunk_size; }

		static inline size_type chunks_required( size_type size )
		{
			return chunks_count( size ) + (extra_bytes( size ) ? 1 : 0);
		};
	};

	//Size tracking decorator for memory manager
	template< class MemMgr >
	class size_tracking
	{
		typedef MemMgr memmgr_t;
		typedef typename memmgr_t::size_type			size_type;
		typedef typename memmgr_t::ptr_t				ptr_t;

		size_type* size_cast( ptr_t p ) const
		{
			return static_cast< size_type* >( p.get() );
		}

		memmgr_t m_memmgr;
	public:
		typedef typename memmgr_t::block_ptr_type		block_ptr_type;		
		typedef typename memmgr_t::size_type			size_type;
		typedef typename memmgr_t::ptr_t				ptr_t;

		explicit size_tracking( void* mem_base )
			:m_memmgr( mem_base )
		{}

		ptr_t allocate( size_type size )
		{
			//allocate additional memory for size storing
			size += sizeof( size_type );
			ptr_t ptr = m_memmgr.allocate( size );
			size_type* psize = size_cast( ptr );
			*psize = size;
			return ptr_t( ++psize );
		}

		void deallocate( ptr_t ptr )
		{
			size_type *ps = size_cast( ptr );
			--ps;
			m_memmgr.deallocate( ptr_t( ps ), *ps );
		}
	};
}
