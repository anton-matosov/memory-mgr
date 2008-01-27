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
#include "detail/ptr_helpers.h"
#include "offset_pointer.h"

namespace memory_mgr
{
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
		template <class> class PtrT = offset_pointer,
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
		typedef typename detail::sync::object_level_lockable<SyncObj>::lock lock;
		
		typedef detail::bit_manager<BlockType, num_chunks, detail::mcNone> bitmgr_type;
		bitmgr_type m_bitmgr;

		char* m_membase;
		
	public:
		typedef typename bitmgr_type::block_ptr_type					block_ptr_type;
		typedef typename bitmgr_type::size_type						size_type;
		typedef memory_manager										self_type;
		typedef SyncObj												sync_object_type;

		
		typedef PtrT<self_type> ptr_type;		

		explicit memory_manager( void* mem_base )
			:m_bitmgr( static_cast< block_ptr_type >( mem_base ) )
		{
			m_membase = detail::shift( mem_base, bitmgr_type::memory_usage );
		}
		
		//Call this method to allocate memory block
		//size - block size in bytes
		ptr_type allocate( size_type size )
		{			
			return do_allocate( size, throw_bad_alloc );
		}


		//Call this method to allocate memory block
		//Newer throws
		//size - block size in bytes
		ptr_type allocate( size_type size, const std::nothrow_t& )/*throw()*/
		{			
			return do_allocate( size, do_nothing );
		}

		//Call this method to deallocate memory block
		//ptr - pointer returned by allocate method
		//size - block size in bytes
 		void deallocate( const ptr_type ptr, size_type size )
 		{
			lock l(*this);
 			m_bitmgr.deallocate( chunk_index( ptr.get_off( *this ) ), chunks_required( size ) );
 		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		//size - block size in bytes
		void deallocate( const void* p, size_type size )
		{
			assert( p >= m_membase && "Invalid pointer value" );
			deallocate( ptr_type( *this, p ), size );
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

		static inline void throw_bad_alloc()
		{
			throw std::bad_alloc();
		}

		static inline void do_nothing()
		{}

		//Call this method to allocate memory block
		//size - block size in bytes
		//returns poiner
		template< class OnNoMemory >
		inline ptr_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{			
			lock l(*this);
			size_type chunk_ind = m_bitmgr.allocate( chunks_required( size ) );
			if( chunk_ind == bitmgr_type::npos )
			{
				OnNoMemoryOp();
				return pointer_traits<ptr_type>::null_ptr;
			}
			return ptr_type( *this, calc_offset( chunk_ind ) );
		}
	};

	//Size tracking decorator for memory manager
	template< class MemMgr >
	class size_tracking
	{
		typedef MemMgr									memmgr_type;				
	public:
		enum
		{
			chunk_size = memmgr_type::chunk_size,
			memory_size =  memmgr_type::memory_size,
			num_chunks =  memmgr_type::num_chunks
		};

		typedef typename memmgr_type::block_ptr_type	block_ptr_type;		
		typedef typename memmgr_type::size_type			size_type;
		typedef typename memmgr_type::ptr_type			ptr_type;

		explicit size_tracking( void* mem_base )
			:m_memmgr( mem_base )
		{}

		ptr_type allocate( size_type size )
		{
			//allocate additional memory for size storing
			size += sizeof( size_type );
			ptr_type ptr = m_memmgr.allocate( size );
			size_type* psize = size_cast( ptr );
			*psize = size;
			return ptr_type( m_memmgr, ++psize );
		}

		void deallocate( ptr_type ptr )
		{
			size_type *ps = size_cast( ptr );
			--ps;
			m_memmgr.deallocate( ps, *ps );
		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		void deallocate( const void* p )
		{
			assert( p > m_memmgr.get_base() && "Invalid pointer value" );
			deallocate( ptr_type( m_memmgr, p ) );
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

	private:
		size_type* size_cast( ptr_type p ) const
		{
			return static_cast< size_type* >( p.get_ptr(m_memmgr) );
		}

		memmgr_type m_memmgr;
	};

}


#endif// MGR_MEMORY_MANAGER_HEADER
