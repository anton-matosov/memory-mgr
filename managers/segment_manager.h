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
#include "detail/helpers.h"
#include <assert.h>
#include <functional>

namespace memory_mgr
{

	/**
	   @brief memory segments manager
	   @details 
	   @tparam MemMgr memory manager with attached memory segment
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
			:curr_segment(0)
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
			return do_allocate( size, helpers::throw_bad_alloc );
		}


		/**
		   @brief Call this method to allocate memory block
		   @param size    size of memory block in bytes 
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   
		   @exception newer  throws
		   @return offset in bytes from memory base address.          
		*/
		inline offset_type allocate( size_type size, const std::nothrow_t& /*nothrow*/ )/*throw()*/
		{			
			return do_allocate( size, helpers::do_nothing );
		}

		/**
		   @brief Call this method to deallocate memory block
		   @param offset  offset returned by allocate method
		   @param size   size of memory block in bytes
		   @exception newer  throws
		*/
 		inline void deallocate( const offset_type offset, size_type size )
 		{
			size_type seg_id = offset >> offset_bits;
			assert( seg_id < num_segments );
			offset_type real_offset = offset & offset_mask;

			return get_segment(seg_id)->deallocate( real_offset, size );
 		}

		template< class OnNoMemory >
		inline offset_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{
			//If requested block size is greater than allocable memory size
			if( size > manager_traits<mgr_type>::allocable_memory )
			{
				OnNoMemoryOp();
				return offset_traits<offset_type>::invalid_offset;
			}

			mgr_pointer_type segment = get_segment(curr_segment);

			offset_type offset = segment->allocate( size, std::nothrow_t() );
			size_type seg_id = curr_segment + 1;

			while ( offset == offset_traits<offset_type>::invalid_offset &&
				seg_id != curr_segment )
			{
				if( seg_id >= num_segments )
				{
					seg_id = 0;
				}

				segment = get_segment(seg_id);
				offset = segment->allocate( size, std::nothrow_t() );
				if( offset != offset_traits<offset_type>::invalid_offset )
				{
					curr_segment = seg_id;
				}
				++seg_id;
			}

			if( offset == offset_traits<offset_type>::invalid_offset )
			{
				OnNoMemoryOp();
			}

			return add_seg_id_to_offset( offset, curr_segment );
		}

		/**
		   @brief Call this method to get memory base address from which offset
		   is calculated
		   @exception newer  throws
		   @return pointer to memory base address                               
		*/
		inline char* get_offset_base( const offset_type offset = 0 )
		{
			if( offset == offset_traits<offset_type>::invalid_offset )
			{
				return 0;
			}
			else
			{
				size_type seg_id = offset >> offset_bits;
				assert( seg_id < num_segments );
				offset_type real_offset = offset & offset_mask;

				return get_segment(seg_id)->get_offset_base( real_offset );
			}
		}

		/**
		   @add_comments
		*/
		inline char* get_ptr_base( const void* ptr )
		{
			return 0;//m_mgr.get_ptr_base( ptr );
		}

		/**
		   @brief Call this method to know is there available memory in
		   manager
		   
		   @exception newer  throws
		   @retval true   if there is no more free memory to
		                  allocate
		   @retval false  otherwise                                    
		*/
		inline bool empty()
		{
			bool result = true;
			mgr_pointer_type* pseg = m_segments;
			while( pseg != (m_segments + num_segments) && result )
			{
				if( *pseg )
				{
					result &= (*pseg)->empty();
				}
				else
				{
					result = false;
				}
				++pseg;
			}
			return result;
		}

		/**
		   @brief Call this method to know that there is no allocated blocks
		   @exception newer  throws
		   @retval true   no blocks are allocated by this manager
		   @retval false  otherwise
		*/
		inline bool free()
		{
			bool result = true;
			mgr_pointer_type* pseg = m_segments;
			while( pseg != (m_segments + num_segments) && result )
			{
				if( *pseg )
				{
					result &= (*pseg)->free();
				}
				++pseg;
			}
			return result;
		}

		/**
		   @brief Call this method to deallocate all allocated memory
		   @exception newer  throws                                     
		*/
		inline void clear()
		{
			mgr_pointer_type* pseg = m_segments;
			while( pseg != m_segments + num_segments )
			{
				if( *pseg )
				{
					(*pseg)->clear();
				}
				++pseg;
			}
			//std::for_each( m_segments, m_segments + num_segments, std::mem_fun( &mgr_type::clear ) );
		}
	};

	/**
	   @brief memory_manager + size_tracking traits
	   @details Adds size_tracking_tag to manager_category
	*/
	template<class MemMgr, size_t SegCount>
	struct manager_traits< segment_manager< MemMgr, SegCount > > 
		: public manager_traits< MemMgr >
	{
	};
}

#endif //MGR_SEGMENT_MANAGER_HEADER
