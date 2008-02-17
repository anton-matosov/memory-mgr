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

#include "detail/ptr_helpers.h"
#include "detail/bit_manager.h"
#include "detail/critical_section.h"
#include "detail/locks.h"
#include "detail/offset_traits.h"
#include "manager_category.h"

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
		class OffsetType = size_t,
		class SyncObj = detail::sync::critical_section
	>
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
		typedef memory_manager								self_type;
		typedef memory_manager_tag							manager_category;

		typedef typename bitmgr_type::block_ptr_type		block_ptr_type;
		typedef typename bitmgr_type::size_type				size_type;
		
		typedef SyncObj										sync_object_type;
		typedef OffsetType									offset_type;
		

		explicit memory_manager( void* mem_base )
			:m_bitmgr( static_cast< block_ptr_type >( mem_base ) )
		{
			m_membase = detail::char_cast( detail::shift( mem_base, bitmgr_type::memory_usage ) );
		}
		
		//Call this method to allocate memory block
		//size - block size in bytes
		//Returns: offset in bytes from memory base address.
		offset_type allocate( size_type size )
		{			
			return do_allocate( size, helpers::throw_bad_alloc );
		}


		//Call this method to allocate memory block
		//Newer throws
		//size - block size in bytes
		//Returns: offset in bytes from memory base address.
		offset_type allocate( size_type size, const std::nothrow_t& )/*throw()*/
		{			
			return do_allocate( size, helpers::do_nothing );
		}

		//Call this method to deallocate memory block
		//offset - offset returned by allocate method
		//size - block size in bytes
 		void deallocate( const offset_type offset, size_type size )
 		{
			lock l(*this);
 			m_bitmgr.deallocate( chunk_index( offset ), chunks_required( size ) );
 		}

		//Returns base address
		char* get_base() const
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
		//Returns: offset in bytes from memory base address.
		template< class OnNoMemory >
		inline offset_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{			
			lock l(*this);
			size_type chunk_ind = m_bitmgr.allocate( chunks_required( size ) );
			if( chunk_ind == bitmgr_type::npos )
			{
				OnNoMemoryOp();
				return offset_traits<offset_type>::invalid_offset;
			}
			return calc_offset( chunk_ind );
		}
	};	
}


#endif// MGR_MEMORY_MANAGER_HEADER
