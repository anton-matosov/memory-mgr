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

#pragma once

#include "memory-mgr/detail/offset_ptr_base.h"
#include "memory-mgr/detail/ptr_helpers.h"

namespace memory_mgr
{	
	/**
		@brief		Offset pointer class.
		@details	Smart pointer class which stores offset between the base address
					of the memory segment and the object. This allows to put this pointer
					to the shared memory and/or memory mapped file mapped to the different base 
					addresses.

		@note		Declaration examples:
					offset_pointer<char>		char_ptr;
					offset_pointer<const char>	const_char_ptr;
	*/
	template< class T, class Mgr >
	class offset_pointer
		: public detail::offset_ptr_base< T, typename manager_traits<Mgr>::block_offset_type, offset_pointer< T, Mgr > >
	{		
	public:

		typedef detail::offset_ptr_base< T, typename manager_traits<Mgr>::block_offset_type, offset_pointer< T, Mgr > > base_type;
		friend base_type;
		typedef offset_pointer self_type;
		typedef Mgr mgr_type;

		using typename base_type::const_pointer;


		//Default constructor
		//Constructs null pointer
		inline offset_pointer()
			:base_type()
		{}

		//Copy constructor
		inline offset_pointer( const offset_pointer& ptr )
			:base_type( ptr )
		{
		}

		//Pointer constructor
		inline offset_pointer( const_pointer p )
			:base_type( p )
		{	
		}

		//Polymorph copy constructor
		template < typename U >
		inline offset_pointer( const offset_pointer< U, mgr_type >& ptr )
			:base_type( ptr )
		{
		}

		template<class Y, class MgrY>
		offset_pointer(offset_pointer<Y, MgrY> const & r,
			memory_mgr::detail::static_cast_tag tag)
			:base_type( r, tag )
		{
		}

		template<class Y, class MgrY>
		offset_pointer(offset_pointer<Y, MgrY> const & r,
			memory_mgr::detail::dynamic_cast_tag tag)
			:base_type( r, tag )
		{
		}

		template<class Y, class MgrY>
		offset_pointer(offset_pointer<Y, MgrY> const & r,
			memory_mgr::detail::const_cast_tag tag)
			:base_type( r, tag )
		{
		}

		//////////////////////////////////////////////////////////////////////////
		//Polymorph copy operators
		inline offset_pointer& operator=( const_pointer p )			
		{
			base_type::operator =( p );				
			return *this;
		}

		inline offset_pointer& operator=( const offset_pointer& ptr )			
		{
			base_type::operator =( ptr );
			return *this;
		}

		//Polymorph copy operator
		template < typename U >
		inline offset_pointer& operator=( const offset_pointer< U, mgr_type >& ptr )			
		{
			base_type::operator =( ptr );
			return *this;
		}
	private:
		inline const_pointer do_get_pointer() const
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, singleton_manager_tag>::value), Memory_manager_should_be_singleton_manager );
			return static_cast<const_pointer>( detail::offset_to_pointer( this->m_offset, mgr_type::instance() ) );
		}

		inline void do_set_pointer( const_pointer ptr )
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, singleton_manager_tag>::value), Memory_manager_should_be_singleton_manager );
			this->m_offset = detail::pointer_to_offset( ptr, mgr_type::instance() );
		}
	
	};

	template<class T, class U, class Mgr>
	offset_pointer<T, Mgr> static_pointer_cast(offset_pointer<U, Mgr> const & r)
	{
		return offset_pointer<T, Mgr>(r, memory_mgr::detail::static_cast_tag());
	}

	template<class T, class U, class Mgr>
	offset_pointer<T, Mgr> const_pointer_cast(offset_pointer<U, Mgr> const & r)
	{
		return offset_pointer<T, Mgr>(r, memory_mgr::detail::const_cast_tag());
	}

	template<class T, class U, class Mgr>
	offset_pointer<T, Mgr> dynamic_pointer_cast(offset_pointer<U, Mgr> const & r)
	{
		return offset_pointer<T, Mgr>(r, memory_mgr::detail::dynamic_cast_tag());
	}

	template<class Mgr, class T>
	inline void delete_( memory_mgr::offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_<Mgr>( get_pointer_internal(ptr) );
	}

	template<class Mgr, class T>
	inline void delete_array( memory_mgr::offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_array<Mgr>( get_pointer_internal(ptr) );
	}

	template<class T, class Mgr>
	void swap(offset_pointer<T, Mgr>& a, offset_pointer<T, Mgr>& b)
	{
		a.swap(b);
	}
}

MGR_DEFINE_ALL_DELETES( memory_mgr::offset_pointer<T MGR_COMA MemMgr>, get_pointer_internal );
