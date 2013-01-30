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
#include <memory-mgr/offset_ptr.h>

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
		class NamedAllocatorTraits = detail::gstl_named_allocator_traits<MemMgr>	
	>	
	class named_objects
		:public detail::decorator_base<MemMgr>
	{
		typedef detail::decorator_base<MemMgr>								base_type;

		typedef MemMgr														mgr_type;
		typedef NamedAllocatorTraits										named_allocator_traits;
		typedef detail::named_allocator<MemMgr, named_allocator_traits>		named_allocator_type;

		typedef typename named_allocator_type::string_type					string_type;

		named_allocator_type	m_named_alloc;

	public:
		typedef manager_traits<mgr_type> mgr_traits;
		typedef typename mgr_traits::size_type		size_type;

		/**
		@brief type that used to store memory offset
		@see named_allocator_type::block_offset_type
		*/
		typedef typename named_allocator_type::block_offset_type block_offset_type;
	
		typedef typename mgr_traits::sync_object_type sync_object_type;
		typedef typename mgr_traits::lock_type lock_type;

		inline named_objects()
			:m_named_alloc( base_type::get_decorated_mgr() )
		{}

		inline named_objects( void* segment_base )
			:base_type( segment_base ),
			m_named_alloc( base_type::get_decorated_mgr() )
		{}

		bool is_exists( const char* name )
		{
			lock_type lock( this->get_lockable() );
			return this->m_named_alloc.is_exists( name );
		}

		using base_type::allocate;
		using base_type::deallocate;

		inline void* allocate( size_type size, const char* name )
		{
			lock_type lock( this->get_lockable() );
			void* ptr = 0;
			block_offset_type offset = this->m_named_alloc.get_object( name );
			if( offset ==  offset_traits<block_offset_type>::invalid_offset )
			{
				ptr = decorated_mgr::allocate( size );
				m_named_alloc.add_object( name, detail::pointer_to_offset( ptr, *this ) );
			}
			else
			{
				ptr = detail::offset_to_pointer( offset, *this );
			}
			return ptr;
		}

		inline bool remove_object( const char* name )
		{
			lock_type lock( this->get_lockable() );
			return this->m_named_alloc.remove_object( name );
		}

		inline void deallocate( const void* p, size_type size, const char* /*name*/ )
		{
			decorated_mgr::deallocate( p, size );
		}

		/**
 		@brief Call this method to deallocate memory block owned by named object
 		@param p  pointer to memory block, returned by allocate method
 		@param size   size of memory block in bytes
 		@exception newer  throws
 		*/
		inline void deallocate_named( void* p, size_type size )
		{
			lock_type lock( this->get_lockable() );
			if( this->m_named_alloc.remove_object( detail::pointer_to_offset( p, *this ) ) )
			{
				decorated_mgr::deallocate( p, size );
			}
		}

		inline void deallocate_named( void* p, size_type size, const char* name )
		{
			if( this->remove_object( name ) )
			{
				decorated_mgr::deallocate( p, size );
			}
		}
	};

	/**
	@brief memory_manager + named_objects traits
	@details Adds named_objects_manager_tag to manager_category
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
