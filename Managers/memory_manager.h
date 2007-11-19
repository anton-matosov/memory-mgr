/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3, 29 June 2007 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA <http://fsf.org/>


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#ifndef MGR_MEMORY_MANAGER_HEADER
#define MGR_MEMORY_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/bit_manager.h"
#include "detail/critical_section.h"
#include "detail/locks.h"

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

		//Offset pointer class
		template< class Mgr >
		class off_ptr_t
		{
		protected:
			typedef Mgr mgr_t;

			const char* do_get_ptr( const mgr_t& mgr ) const
			{
				return detail::shift( mgr.get_base(), m_offset );
			}
		public:
			typedef typename mgr_t::size_type size_type;
			//Construct pointer from offset
			explicit off_ptr_t( const size_type offset )
				:m_offset( offset )
			{}

			//Construct pointer from memory address
			off_ptr_t( const mgr_t& mgr, const void* ptr )			
				:m_offset( detail::diff( ptr, mgr.get_base() ) )
			{}

			//Call this method to get offset
			const size_type get_off() const
			{
				return m_offset;
			}

			//Call this method to get real memory address
			void* get_ptr( const mgr_t& mgr )
			{
				return detail::unconst_char( do_get_ptr( mgr ) );
			}

			//Call this method to get real memory address
			const void* get_ptr( const mgr_t& mgr ) const
			{
				return do_get_ptr( mgr );
			}
		private:
			size_type m_offset;	
		};
	}

	//Memory manager
	//Encapsulates work with memory
	//Template parameters:
	//BlockType	 - integral type, it will be used in static_bitset
	//MemorySize - memory maximum which manager can allocate
	//ChunkSize  - size of minimum block in bytes, which manager can allocate
	template
	<
		class BlockType, 
		size_t MemorySize,
		size_t ChunkSize, 
		template <class> class PtrT = detail::off_ptr_t,
		class SyncObj = detail::sync::critical_section >
	class memory_manager : protected detail::sync::object_level_lockable<SyncObj>
	{
	public:
		enum
		{
			chunk_size = ChunkSize,
			memory_size = MemorySize,
			num_chunks = memory_size / chunk_size			
		};

	private:
		typedef detail::bit_manager<BlockType, num_chunks, detail::mcNone> bitmgr_t;
		bitmgr_t m_bitmgr;

		char* m_membase;

	public:
		typedef typename bitmgr_t::block_ptr_type					block_ptr_type;		
		typedef typename bitmgr_t::size_type						size_type;
		typedef memory_manager<BlockType, MemorySize, ChunkSize>	self_type;
		typedef SyncObj												sync_object;

		typedef PtrT<self_type> ptr_t;
		static const ptr_t null_ptr;
		

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
		//off - offset returned by allocate method
		//size - block size in bytes
 		void deallocate( const ptr_t off, size_type size )
 		{
			lock l(*this);
 			m_bitmgr.deallocate( chunk_index( off.get_off() ), chunks_required( size ) );
 		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		//size - block size in bytes
		void deallocate( const void* p, size_type size )
		{
			assert( p > m_membase && "Invalid pointer value" );
			deallocate( ptr_t( *this, p ), size );
		}

		//Returns base address
		const char* get_base() const
		{
			return m_membase;
		}

		bool empty()
		{
			return m_bitmgr.empty();
		}

		bool free()
		{
			return m_bitmgr.free();
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
			lock l(*this);
			return m_bitmgr.allocate( chunks_required( size ) );
		}
	};

	template < class BlockType, size_t MemorySize, size_t ChunkSize, template <class> class PtrT, class SyncObj >
	typename const memory_manager< BlockType, MemorySize, ChunkSize, PtrT, SyncObj >::ptr_t memory_manager< BlockType, MemorySize, ChunkSize, PtrT, SyncObj >::null_ptr( memory_manager< BlockType, MemorySize, ChunkSize, PtrT, SyncObj >::bitmgr_t::npos );

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
		enum
		{
			chunk_size = memmgr_t::chunk_size,
			memory_size =  memmgr_t::memory_size,
			num_chunks =  memmgr_t::num_chunks
		};

		typedef typename memmgr_t::block_ptr_type		block_ptr_type;		
		typedef typename memmgr_t::size_type			size_type;
		typedef typename memmgr_t::ptr_t				ptr_t;

		static const ptr_t null_ptr;

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

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		void deallocate( const void* p )
		{
			assert( p > m_memmgr.get_base() && "Invalid pointer value" );
			deallocate( ptr_t( m_memmgr, p ) );
		}

		bool empty()
		{
			return m_memmgr.empty();
		}

		bool free()
		{
			return m_memmgr.free();
		}

		//Returns base address
		const char* get_base() const
		{
			return m_memmgr.get_base();
		}
	};

	template< class MemMgr >
	typename const size_tracking<MemMgr>::ptr_t size_tracking<MemMgr>::null_ptr( size_tracking<MemMgr>::memmgr_t::null_ptr );
}


#endif// MGR_MEMORY_MANAGER_HEADER