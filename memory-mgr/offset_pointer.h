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

#ifndef MGR_OFFSET_POINTER_HEADER
#define MGR_OFFSET_POINTER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/offset_ptr_base.h>

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
			return static_cast<const_pointer>( detail::offset_to_pointer( m_offset, mgr_type::instance() ) );
		}

		inline void do_set_pointer( const_pointer ptr )
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, singleton_manager_tag>::value), Memory_manager_should_be_singleton_manager );
			m_offset = detail::pointer_to_offset( ptr, mgr_type::instance() );
		}
	
	};
	
	template<class Mgr, class T>
	static inline void delete_( offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_<Mgr>( ptr );
	}

	template<class Mgr, class T>
	static inline void delete_array( offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_array<Mgr>( ptr );
	}
}


#endif// MGR_OFFSET_POINTER_HEADER