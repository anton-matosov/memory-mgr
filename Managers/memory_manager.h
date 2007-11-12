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

		static inline char* unconst_char( const char* p )
		{
			return const_cast< char* >( p );
		}

		static inline ptrdiff_t diff( const void* p1, const void* p2 )
		{
			return char_cast(p1) - char_cast(p2);
		}

		static inline char* shift( void* p, size_t offset )
		{
			return char_cast(p) + offset;
		}

 		static inline const char* shift( const void* p, const size_t offset )
 		{
 			return char_cast(p) + offset;
 		}
	}

	//Memory manager
	//Encapsulates work with memory
	//Template parameters:
	//BlockType	 - integral type, it will be used in static_bitset
	//MemorySize - memory maximum which manager can allocate
	//ChunkSize  - size of minimum block in bytes, which manager can allocate
	template< class BlockType, size_t MemorySize, size_t ChunkSize >
	class memory_manager
	{
		enum
		{
			chunk_size = ChunkSize,
			memory_size = MemorySize,
			num_chunks = memory_size / chunk_size			
		};
		typedef bit_manager<BlockType, num_chunks, mcNone> bitmgr_t;
		bitmgr_t m_bitmgr;

		char* m_membase;
	public:

		typedef typename bitmgr_t::block_ptr_type		block_ptr_type;		
		typedef typename bitmgr_t::size_type			size_type;
		typedef memory_manager<BlockType, MemorySize, ChunkSize> self_type;
		
		//Offset pointer class
		class ptr_t
		{
			typedef self_type mgr_t;

			size_type m_offset;
			friend mgr_t;

		protected:
			const char* do_get_ptr( const memory_manager& mgr ) const
			{
				return detail::shift( mgr.get_base(), m_offset );
			}
		public:
			//Construct pointer from offset
			explicit ptr_t( const size_type offset )
				:m_offset( offset )
			{}

			//Construct pointer from memory address
			explicit ptr_t( const memory_manager& mgr, const void* ptr )			
				:m_offset( detail::diff( ptr, mgr.get_base() ) )
			{}
			
			//Call this method to get offset
			const size_type get_off() const
			{
				return m_offset;
			}

			//Call this method to get real memory address
			void* get_ptr( const memory_manager& mgr )
			{
				return detail::unconst_char( do_get_ptr( mgr ) );
			}

			//Call this method to get real memory address
			const void* get_ptr( const memory_manager& mgr ) const
			{
				return do_get_ptr( mgr );
			}
		};

		explicit memory_manager( void* mem_base )
			:m_bitmgr( static_cast< block_ptr_type >( mem_base ) )
		{
			m_membase = detail::shift( mem_base, bitmgr_t::memory_usage );
		}
		
		//Call this method to allocate memory block
		//size - block size in bytes
		ptr_t allocate( size_type size )
		{
			size_type chunk_ind = do_allocate( size );
			if( chunk_ind == bitmgr_t::npos )
			{
				throw std::bad_alloc();
			}
			return ptr_t( calc_offset( chunk_ind ) );
		}


		//Call this method to allocate memory block
		//size - block size in bytes
		ptr_t allocate( size_type size, const std::nothrow_t& )/*throw()*/
		{
			return ptr_t( calc_offset( do_allocate( size ) ) );
		}

		//Call this method to deallocate memory block
		//size - block size in bytes
 		void deallocate( const ptr_t off, size_type size )
 		{
 			m_bitmgr.deallocate( chunk_index( off.get_off() ), chunks_required( size ) );
 		}

		//Returns base address
		const char* get_base() const
		{
			return m_membase;
		}
	private:
		//Returns offset by chunk index
		static inline size_type calc_offset( size_type chunk_ind )
		{ return chunk_ind * chunk_size; }

		//Returns chunk index by offset
		static inline size_type chunk_index( size_type offset )
		{ return offset / chunk_size; }

		//Returns chunks count by bytes count
		static inline size_type chunks_count( size_type size )
		{ return chunk_index( size ); }

		//Returns number of extra bytes
		static inline size_type extra_bytes( size_type size )
		{ return size % chunk_size; }

		//Returns number of chunks required to store requested bytes number
		static inline size_type chunks_required( size_type size )
		{
			return chunks_count( size ) + (extra_bytes( size ) ? 1 : 0);
		};


		//Call this method to allocate memory block
		//size - block size in bytes
		//returns chunk index
		inline size_type do_allocate( size_type size )
		{			
			return m_bitmgr.allocate( chunks_required( size ) );
		}
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
			return static_cast< size_type* >( p.get_ptr(m_memmgr) );
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
			return ptr_t( m_memmgr, ++psize );
		}

		void deallocate( ptr_t ptr )
		{
			size_type *ps = size_cast( ptr );
			--ps;
			m_memmgr.deallocate( ptr_t( m_memmgr, ps ), *ps );
		}
	};
}
