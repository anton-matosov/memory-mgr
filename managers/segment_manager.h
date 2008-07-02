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
#include "detail/decorator_base.h"
#include <assert.h>
#include <functional>
#include <vector>
#include <map>

namespace memory_mgr
{

	/**
	   @brief memory segments manager
	   @details 
	   @tparam MemMgr memory manager with attached memory segment
	   @tparam SegmentsCount maximum number of segments
	*/
	template
	<
		class MemMgr,
		size_t SegmentsCount
	>
	class segment_manager
	{
		typedef /*detail::decorator_base<*/ MemMgr/* >*/		mgr_type;
		typedef mgr_type*	mgr_pointer_type;

	public:
		enum
		{
			num_segments = SegmentsCount,
			segment_size = manager_traits<mgr_type>::memory_size,
			allocable_memory = manager_traits<mgr_type>::allocable_memory
			//offset_mask,
			//segment_mask
		};
	private:

		

		typedef typename manager_traits<mgr_type>::size_type		size_type;
		typedef typename manager_traits<mgr_type>::offset_type		offset_type;

		typedef char* base_ptr_type;

		size_type calc_offset_bits( size_type seg_size )
		{
			size_type log2 = helpers::integer_log2( seg_size );
			size_type extra = seg_size % log2;

			return log2 + (extra ? 1 : 0);
		}

		size_type m_segment_mask;
		size_type m_offset_mask;
		size_type m_offset_bits;

		size_type m_curr_segment;

		//base_ptr_type		m_bases[num_segments];
		mgr_pointer_type	m_segments[num_segments];


		typedef char*								seg_data_type;
		typedef std::map<seg_data_type, size_t>		seg_bases_type;

		seg_bases_type m_bases;

		bool in_segment( const char* base, const char* ptr )
		{
			if ( ptr >= base )
			{
				if( (ptr - base) <= allocable_memory )
				{
					return true;
				}
			}
			return false;
		}
	public:
		segment_manager()
			:m_curr_segment(0)
		{
			m_offset_bits = calc_offset_bits( allocable_memory );
			m_segment_mask = ~size_type(0) << m_offset_bits;
			m_offset_mask = ~m_segment_mask;
			//offset_bits( segment_size );
			std::fill<mgr_pointer_type*,mgr_pointer_type>( m_segments, m_segments + num_segments, 0 );

			//m_bases.reserve( num_segments );
		}

		~segment_manager()
		{
			mgr_pointer_type* pseg = m_segments;
			mgr_pointer_type* end = m_segments + num_segments;
			while( pseg != end )
			{
				if( *pseg )
				{
					delete *pseg;
					*pseg = 0;
				}
				++pseg;
			}
		}

		inline mgr_pointer_type get_segment( size_type seg_id )
		{
			mgr_pointer_type segment = m_segments[seg_id];
			if( !segment )
			{
				segment = m_segments[seg_id] = new mgr_type( seg_id );
				m_bases[segment->get_offset_base()] = seg_id;
			}
			return segment;
		}

		inline offset_type add_seg_id_to_offset( offset_type offset, size_type seg_id )
		{
			return offset | (seg_id << m_offset_bits);
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
			if( offset == offset_traits<offset_type>::invalid_offset )
			{
// 				size_type seg_id = offset >> m_offset_bits;
// 				assert( seg_id < num_segments );
// 				offset_type real_offset = offset & m_offset_mask;

				get_segment( offset >> m_offset_bits )->deallocate( offset & m_offset_mask, size );
			}
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

			mgr_pointer_type segment = get_segment(m_curr_segment);

			offset_type offset = segment->allocate( size, std::nothrow_t() );
			size_type seg_id = m_curr_segment + 1;

			while ( offset == offset_traits<offset_type>::invalid_offset &&
				seg_id != m_curr_segment )
			{
				if( seg_id >= num_segments )
				{
					seg_id = 0;
				}

				segment = get_segment(seg_id);
				offset = segment->allocate( size, std::nothrow_t() );
				if( offset != offset_traits<offset_type>::invalid_offset )
				{
					m_curr_segment = seg_id;
				}
				else
				{
					++seg_id;
				}
			}

			if( offset == offset_traits<offset_type>::invalid_offset )
			{
				OnNoMemoryOp();
			}

			return add_seg_id_to_offset( offset, m_curr_segment );
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
				size_type seg_id = offset >> m_offset_bits;
				assert( seg_id < num_segments );
				offset_type real_offset = offset & m_offset_mask;

				mgr_pointer_type seg = get_segment(seg_id);
				char* base = seg->get_offset_base( real_offset );
				return base;
			}
		}

		/**
		   @add_comments
		*/
		inline char* get_ptr_base( const void*  ptr )
		{	
			seg_bases_type::const_iterator fres = find_segment( ptr );
			if( fres != m_bases.end() )
			{
				return fres->first;
			}
			return 0;
			
		}

		inline seg_bases_type::const_iterator find_segment( const void*  ptr )
		{
			const char* p = detail::char_cast( ptr );
			seg_bases_type::const_iterator fres = m_bases.upper_bound( detail::unconst_char( p ) );
			if( fres != m_bases.begin() )
			{
				--fres;
				if( in_segment( fres->first, p ) )
				{
					return fres;
				}
			}
			return m_bases.end();
		}

		/**
		   @add_comments
		*/
		inline void* offset_to_pointer( offset_type offset )
		{
			if( offset == offset_traits<offset_type>::invalid_offset )
			{
				return 0;
			}
			else
			{
				size_type seg_id = offset >> m_offset_bits;
				assert( seg_id < num_segments );
				offset_type real_offset = offset & m_offset_mask;

				return get_segment(seg_id)->offset_to_pointer( real_offset );
			}
		}

		/**
		   @add_comments
		*/
		inline offset_type pointer_to_offset( const void* ptr )
		{
			seg_bases_type::const_iterator fres = find_segment( ptr );
			if( fres != m_bases.end() )
			{
				return add_seg_id_to_offset( detail::diff( ptr, fres->first ), fres->second );
			}
			return offset_traits<offset_type>::invalid_offset;
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
			mgr_pointer_type* end = m_segments + num_segments;
			while( pseg != end && result )
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
			mgr_pointer_type* end = m_segments + num_segments;
			while( pseg != end && result )
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
			mgr_pointer_type* end = m_segments + num_segments;
			while( pseg != end )
			{
				if( *pseg )
				{
					(*pseg)->clear();
					//delete *pseg;
					//*pseg = 0;
				}
				++pseg;
			}
			m_curr_segment = 0;
			m_bases.clear();
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
