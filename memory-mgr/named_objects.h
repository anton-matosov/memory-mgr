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

#ifndef MGR_NAMED_OBJECTS_HEADER
#define MGR_NAMED_OBJECTS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/detail/named_allocator.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/manager_category.h>
#include <memory-mgr/offset_pointer.h>
#include <memory-mgr/block_id_converter.h>


namespace memory_mgr
{	
	/**
	   @brief Named objects decorator for
	   @details 
	*/
	/**
		@brief Named objects decorator for memory manager.
		@details These decorator extends functionality of memory manger
				by adding methods to allocate/deallocate named objects
		@tparam MemMgr  memory_manager class, with or w/o decorators,
					must support MemoryManagerConcept.
	                If PointerConverter is not applied then allocation/deallocation
					operations will work faster
		@todo Implement support for named objects in shared memory
	*/
	template
	< 
		class MemMgr,
		class NamedAllocatorTraits = detail::named_allocator_traits<MemMgr>	
	>	
	class named_objects
		:public detail::decorator_base<MemMgr>
	{
		typedef detail::decorator_base<MemMgr>								base_type;

		typedef MemMgr														mgr_type;
		typedef NamedAllocatorTraits										named_allocator_traits;
		typedef detail::named_allocator<MemMgr, named_allocator_traits>		named_allocator_type;

		typedef typename named_allocator_type::string_type					string_type;

		named_allocator_type	m_alloc;

		typedef typename manager_traits<MemMgr>::block_id_converter_type	converter;

	public:
		/**
		@brief Type used to store size, commonly std::size_t
		@see static_bitset::size_type
		*/
		//typedef typename base_type::size_type			size_type;
		typedef typename manager_traits<mgr_type>::size_type		size_type;

		/**
		@brief type that used to store memory offset
		@see memory_manager::offset_type
		*/
		typedef typename named_allocator_type::offset_type		offset_type;

		typedef typename manager_traits<MemMgr>::block_id_type	block_id_type;
	
		/**
		@brief Type of synchronization object passed as template
		parameter                                               
		*/
		typedef typename base_type::sync_object_type		sync_object_type;

		/**
		@brief lock type, used for synchronization
		*/
		typedef typename base_type::lock_type				lock_type;

		/**
		@brief Default constructor, creates memory manager 
		@remarks Can be used only if decorates memory manager with 
		attached memory segment
		@see memory_manager::memory_segment                        
		*/
		inline named_objects()
			:m_alloc( base_type::m_mgr )
		{}

		/**
		@brief Constructor, creates memory manager with specified
		base address
		@param segment_base  Pointer to memory which will be managed by
		manager
		@see memory_manager::memory_manager                        
		*/
		inline named_objects( void* segment_base )
			:base_type( segment_base ),
			m_alloc( base_type::m_mgr )
		{}

		bool is_exists( const char* name )
		{
			lock_type lock( this->get_lockable() );
			return m_alloc.is_exists( name );
		}

		inline block_id_type allocate( size_type size, const char* name )
		{
			lock_type lock( this->get_lockable() );
			block_id_type offset = converter::to_block_id( m_alloc.get_object( name ), *this );
			if( offset ==  offset_traits<block_id_type>::invalid_offset )
			{
				offset = this->m_mgr.allocate( size );
				m_alloc.add_object( name, detail::to_offset( offset, *this ) );
			}
			return offset;
		}

		inline void deallocate( const block_id_type p, size_type size, const char* name )
		{
			lock_type lock( this->get_lockable() );
			m_alloc.remove_object( name );
			this->m_mgr.deallocate( p, size );
		}
 		/**
 		@brief Call this method to allocate memory block
 		@param size size of memory block in bytes
 		@exception bad_alloc if manager went out of memory
 		@return pointer to allocated memory block
 		*/
 		inline block_id_type allocate( size_type size )
 		{			
 			return this->m_mgr.allocate( size );
 		}
 
 		/**
 		@brief Call this method to allocate memory block
 		@param size    size of memory block in bytes 
 		@param nothrow  unused parameter, just to overload existing
 		function
 
 		@exception newer  throws
 		@return pointer to allocated memory block         
 		*/
 		inline block_id_type allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
 		{			
 			return this->m_mgr.allocate( size, nothrow );
 		}
 
 		/**
 		@brief Call this method to deallocate memory block
 		@param p  pointer to memory block, returned by allocate method
 		@param size   size of memory block in bytes
 		@exception newer  throws
 		*/
 		inline void deallocate( const block_id_type p, size_type size = 0 )
 		{
			m_alloc.remove_object( detail::to_offset( p, *this ) );
 			this->m_mgr.deallocate( p, size );
 		}
	};

	/**
	@brief memory_manager + pointer_convert traits
	@details Adds pointer_conversion_tag to manager_category
	*/
	template< class MemMgr, class NamedAllocatorTraits >
	struct manager_traits< named_objects< MemMgr, NamedAllocatorTraits > > 
		: public manager_traits< MemMgr >
	{
		/**
		@brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;

		/**
		@brief Add pointer_conversion_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual named_objects_manager_tag
		{};
	};
}
#endif// MGR_NAMED_OBJECTS_HEADER
