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

#include <memory-mgr/manager_traits.h>
#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/math.h>
#include <memory-mgr/detail/ptr_helpers.h>
#include <memory-mgr/detail/offset_traits.h>
#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/detail/segment_storage_map.h>
#include <memory-mgr/detail/segment_storage_vector.h>
#include <memory-mgr/detail/assert.h>
#include <functional>
#include <boost/tuple/tuple.hpp>	//For boost::tie

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
		size_t SegmentsCount,
		template <class, size_t> class storage_policy = detail::segment_storage_vector
	>
	class segment_manager
		:private storage_policy<MemMgr, SegmentsCount>
	{
		typedef storage_policy<MemMgr, SegmentsCount> base_type;
		typedef typename base_type::size_type					size_type;
		typedef typename base_type::block_offset_type			block_offset_type;
		typedef typename base_type::segment_data_type			segment_data_type;
		typedef typename base_type::segment_ptr_type			segment_ptr_type;
		typedef typename base_type::segment_type				segment_type;
	public:

		/**
		@brief Type of synchronization object passed as template
		parameter                                               
		*/
		typedef typename base_type::sync_object_type	sync_object_type;

		/**
		@brief lockable type, used for synchronization
		*/
		typedef typename base_type::lockable_type		lockable_type;

		/**
		@brief lock type, used for synchronization
		*/
		typedef typename base_type::lock_type			lock_type;

		enum
		{
			num_segments		= base_type::num_segments,
			segment_size		= base_type::segment_size,
			allocable_memory	= base_type::allocable_memory,
			offset_bits 		= base_type::offset_bits,
			segment_mask 		= base_type::segment_mask,
			offset_mask 		= base_type::offset_mask
		};
	private:	
		size_type m_curr_segment;
		lockable_type	m_lockable;

		static inline block_offset_type add_seg_id_to_offset( block_offset_type offset, size_type seg_id )
		{
			return offset | (seg_id << offset_bits);
		}

		static inline size_type seg_id_by_offset( block_offset_type offset )
		{
			return offset >> offset_bits;
		}


		template< class OnNoMemory >
		inline offset_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{
			//If requested block size is greater than allocable memory size
			if( size > allocable_memory )
			{
				OnNoMemoryOp();
				return offset_traits<offset_type>::invalid_offset;
			}

			segment_ptr_type segment = get_segment(m_curr_segment);

			lock_type lock( get_lockable() );
			block_offset_type offset = segment->allocate( size, std::nothrow_t() );
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
	public:
		/**
		@brief default constructor, creates segment with id==0
		*/
		segment_manager()
			:m_curr_segment(0)
		{
			get_segment( m_curr_segment );
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
			if( offset != offset_traits<offset_type>::invalid_offset )
			{
				lock_type lock( get_lockable() );
				get_segment( seg_id_by_offset( offset ) )->deallocate( offset & offset_mask, size );
			}
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
				size_type seg_id = seg_id_by_offset( offset );
				MGR_ASSERT( seg_id < num_segments, "Segment id is out of range" );
				offset_type real_offset = offset & offset_mask;

				segment_ptr_type seg = get_segment(seg_id);
				char* base = seg->get_offset_base( real_offset );
				return base;
			}
		}

		/**
		   @add_comments
		*/
		inline char* get_ptr_base( void*  ptr )
		{	
			bool found = false;
			segment_data_type seg_data;
			boost::tie( seg_data, found ) = this->find_segment( ptr );
			if( found )
			{
				return seg_data.first;
			}
			return 0;
			
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
				size_type seg_id = this->seg_id_by_offset( offset );
				MGR_ASSERT( seg_id < num_segments, "Segment id is out of range" );
				offset_type real_offset = offset & offset_mask;

				return this->get_segment(seg_id)->offset_to_pointer( real_offset );
			}
		}

		/**
		   @add_comments
		*/
		inline offset_type pointer_to_offset( const void* ptr )
		{
			bool found = false;
			segment_data_type seg_data;
			boost::tie( seg_data, found ) = this->find_segment( ptr );
			if( found )
			{
				return this->add_seg_id_to_offset( detail::diff( ptr, seg_data.first ), seg_data.second );
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
			lock_type lock( this->get_lockable() );
			return this->for_each_if( std::mem_fun( &segment_type::empty ) );
		}

		/**
		   @brief Call this method to know that there is no allocated blocks
		   @exception newer  throws
		   @retval true   no blocks are allocated by this manager
		   @retval false  otherwise
		*/
		inline bool is_free()
		{
			lock_type lock( this->get_lockable() );
			return this->for_each_if( std::mem_fun( &segment_type::is_free ) );
		}

		/**
		   @brief Call this method to deallocate all allocated memory
		   @exception newer  throws                                     
		*/
		inline void clear()
		{
			lock_type lock( this->get_lockable() );
			this->for_each( std::mem_fun( &segment_type::clear ) );
 			m_curr_segment = 0;
			
			//Delete all segments
			this->delete_segments();
		}

		inline lockable_type& get_lockable()
		{
			return m_lockable;
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
		enum
		{
			num_segments		= SegCount /**< maximum number of segments managed by this manager*/,
			allocable_chunks	= manager_traits< MemMgr >::allocable_chunks * num_segments
		};
	};
}

#endif //MGR_SEGMENT_MANAGER_HEADER
