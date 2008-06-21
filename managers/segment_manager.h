/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

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

#ifndef MGR_SEGMENT_MANAGER_HEADER
#define MGR_SEGMENT_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "manager_traits.h"
#include "manager_category.h"

namespace memory_mgr
{

	/**
	@brief memory segments manager
	@details 
	@tparam MemMgr memory manager with attached memory segment
	@note current implementation works only with heap memory segments
	*/
	template
		<
			class MemMgr,
			size_t SegmentsCount
		>
	class segment_manager
	{
		typedef MemMgr		mgr_type;
		typedef mgr_type*	mgr_pointer_type;

		enum
		{
			num_segments = SegmentsCount,
			segment_size = manager_traits<mgr_type>::memory_size
			//offset_mask,
			//segment_mask
		};
		

		

		typedef typename manager_traits<mgr_type>::size_type		size_type;
		typedef typename manager_traits<mgr_type>::offset_type		offset_type;

		typedef char* base_ptr_type;

		size_type calc_offset_bits( size_type seg_size )
		{
			size_type log2 = helpers::integer_log2( seg_size );
			size_type extra = seg_size % log2;

			return log2 + (extra ? 1 : 0);
		}

		size_type segment_mask;
		size_type offset_mask;
		size_type offset_bits;

		size_type curr_segment;

		//base_ptr_type		m_bases[num_segments];
		mgr_pointer_type	m_segments[num_segments];
	public:
		segment_manager()
			:curr_segment(2)
		{
			offset_bits = calc_offset_bits( segment_size );
			segment_mask = ~size_type(0) << offset_bits;
			offset_mask = ~segment_mask;
			//offset_bits( segment_size );
			std::fill<mgr_pointer_type*,mgr_pointer_type>( m_segments, m_segments + num_segments, 0 );
		}

		~segment_manager()
		{
		}

		mgr_pointer_type get_segment( size_type seg_id )
		{
			mgr_pointer_type& segment = m_segments[seg_id];
			if( !segment )
			{
				segment = new mgr_type( seg_id );
			}
			return segment;
		}

		offset_type add_seg_id_to_offset( offset_type offset, size_type seg_id )
		{
			return offset | (seg_id << offset_bits);
		}
		/**
		   @brief Call this method to allocate memory block
		   @param size size of memory block in bytes
		   @exception bad_alloc if manager went out of memory
		   @return offset in bytes from memory base address.
		*/
		inline offset_type allocate( size_type size )
		{
			mgr_pointer_type segment = get_segment(curr_segment);
// 			if( !segment )
// 			{
// 				segment = new mgr_type( curr_segment );
// 				m_segments[curr_segment] = segment;
// 			}
			return add_seg_id_to_offset( segment->allocate( size ), curr_segment );
		}

		/**
		   @brief Call this method to get memory base address from which offset
		   is calculated
		   @exception newer  throws
		   @return pointer to memory base address                               
		*/
		inline char* get_offset_base( const offset_type offset = 0 )
		{
			size_type seg_id = offset >> offset_bits;
			assert( seg_id < num_segments );
			offset_type real_offset = offset & offset_mask;

			return get_segment(seg_id)->get_offset_base( real_offset );
		}

		/**
		@add_comments
		*/
		inline char* get_ptr_base( const void* ptr )
		{
			return m_mgr.get_ptr_base( ptr );
		}
	};

}

#endif //MGR_SEGMENT_MANAGER_HEADER
