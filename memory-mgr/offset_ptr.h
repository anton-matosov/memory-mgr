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

#ifndef MGR_SELF_OFFSET_PTR_HEADER
#define MGR_SELF_OFFSET_PTR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/offset_ptr_base.h>

namespace memory_mgr
{	
	/**
		@brief		Offset pointer class.
		@details	Smart pointer class which stores offset between 'this' and object it points.
					This allows to put this pointer	to the shared memory and/or
					memory mapped file which are mapped to the different base 
					addresses in different processes.

		@note		Declaration examples:
					offset_ptr<char>		char_ptr; // char*
					offset_ptr<const char>	const_char_ptr; // const char*
					const offset_ptr<char>		char_const_ptr; // char* const
					const offset_ptr<const char>	const_char_const_ptr; // const char* const
	*/
	template< class T >
	class offset_ptr
		: public detail::offset_ptr_base< T, std::ptrdiff_t, offset_ptr< T > >
	{		
	public:
		typedef detail::offset_ptr_base< T, std::ptrdiff_t, offset_ptr< T > > base_type;
		friend base_type;
		typedef offset_ptr self_type;
		

		//Default constructor
		//Constructs null pointer
		inline offset_ptr()
			:base_type()
		{}

		//Copy constructor
		inline offset_ptr( const offset_ptr& ptr )
			:base_type( ptr )
		{
		}

		//Pointer constructor
		inline offset_ptr( const_pointer p )
			:base_type( p )
		{	
		}

		//Polymorph copy constructor
		template < typename U >
		inline offset_ptr( const offset_ptr< U >& ptr )
			:base_type( ptr )
		{
		}

		//////////////////////////////////////////////////////////////////////////
		//Polymorph copy operators
		inline offset_ptr& operator=( const_pointer p )			
		{
			base_type::operator =( p );				
			return *this;
		}

		inline offset_ptr& operator=( const offset_ptr& ptr )			
		{
			base_type::operator =( ptr );
			return *this;
		}

		//Polymorph copy operator
		template < typename U >
		inline offset_ptr& operator=( const offset_ptr< U >& ptr )			
		{
			base_type::operator =( ptr );
			return *this;
		}

	private:
		inline void do_set_pointer( const_pointer ptr )
		{
			//offset == invalid_offset1 && ptr != 0 is not legal for this pointer
			if( ptr == NULL )
			{
				m_offset = offset_traits<offset_type>::invalid_offset;
			}
			else
			{
				m_offset = detail::diff( ptr, this );
				assert( (m_offset != offset_traits<offset_type>::invalid_offset) && "Invalid offset value" );
			}
		}

		inline const_pointer do_get_pointer() const
		{
			if( m_offset == offset_traits<offset_type>::invalid_offset )
			{
				return 0;
			}
			return static_cast<const_pointer>( detail::shift( this, m_offset ) );	
		}
	};
}


#endif// MGR_SELF_OFFSET_PTR_HEADER
