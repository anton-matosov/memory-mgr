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
#include <boost/mpl/if.hpp>


namespace memory_mgr
{	
	/**
	@add_comment
	*/
	template
	< 
		class MemMgr,
		class NamedAllocator = detail::named_allocator<MemMgr, 
			detail::named_allocator_traits<MemMgr, 
				typename boost::mpl::if_c< 
					is_category_supported< MemMgr, pointer_conversion_tag>::value,
					void*,
					typename manager_traits<MemMgr>::offset_type>::type
			>
		>
	
	>	
	class named_objects
		:public detail::decorator_base<MemMgr>
	{
		typedef detail::decorator_base<MemMgr> base_type;
		//typedef MemMgr base_type;
		typedef MemMgr mgr_type;
		typedef NamedAllocator	named_allocator;
		typedef typename named_allocator::string_type string_type;
		named_allocator	m_alloc;
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
		//typedef typename base_type::offset_type			offset_type;
		typedef typename named_allocator::offset_type		offset_type;


		/**
		@brief Default constructor, creates memory manager 
		@remarks Can be used only if decorates memory manager with 
		attached memory segment
		@see memory_manager::memory_segment                        
		*/
		inline named_objects()
			:named_allocator( base_type::m_mgr )
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

		inline offset_type allocate( size_type size, const char* name )
		{
			offset_type offset = m_alloc.get_object( name );
			if( offset ==  offset_traits<offset_type>::invalid_offset )
			{
				offset = this->m_mgr.allocate( size );
				m_alloc.add_object( name, offset );
			}
			return offset;
		}

		inline void deallocate( const offset_type p, size_type size, const char* name )
		{
			m_alloc.remove_object( name );
			this->m_mgr.deallocate( p, size );
		}
 		/**
 		@brief Call this method to allocate memory block
 		@param size size of memory block in bytes
 		@exception bad_alloc if manager went out of memory
 		@return pointer to allocated memory block
 		*/
 		inline offset_type allocate( size_type size )
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
 		inline offset_type allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
 		{			
 			return this->m_mgr.allocate( size, nothrow );
 		}
 
 		/**
 		@brief Call this method to deallocate memory block
 		@param p  pointer to memory block, returned by allocate method
 		@param size   size of memory block in bytes
 		@exception newer  throws
 		*/
 		inline void deallocate( const offset_type p, size_type size )
 		{
			m_alloc.remove_object( p );
 			this->m_mgr.deallocate( p, size );
 		}
	};

	/**
	@brief memory_manager + pointer_convert traits
	@details Adds pointer_conversion_tag to manager_category
	*/
	template< class MemMgr >
	struct manager_traits< named_objects< MemMgr > > 
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
