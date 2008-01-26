/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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

#include "detail/pointer_traits.h"
#include "detail/cmp_helper.h"

namespace memory_mgr
{	
	//Offset pointer class
	template< class Mgr >
	class offset_pointer : public detail::cmp_helper< offset_pointer< Mgr > >
	{		
	public:
		typedef Mgr									mgr_type;
		typedef typename mgr_type::size_type		offset_type;
		typedef typename offset_pointer<mgr_type>	self_type;

		//Constructs null pointer
		explicit offset_pointer( detail::null_type )
			:m_offset( ~offset_type(0) )
		{}


		//Construct pointer from offset
		offset_pointer( const mgr_type& /*mgr*/, const offset_type offset )
			:m_offset( offset )
		{}

		offset_pointer( const offset_pointer& ptr )
			:m_offset( ptr.m_offset )
		{}

		//Construct pointer from memory address
		offset_pointer( const mgr_type& mgr, const void* ptr )			
			:m_offset( detail::diff( ptr, mgr.get_base() ) )
		{ assert( ptr > mgr.get_base() && "Invalid pointer value" ); }

		offset_pointer& operator=( const offset_pointer& ptr )				
		{
			m_offset = ptr.m_offset;
			return *this;
		}

		//Call this method to get offset
		const offset_type get_off( const mgr_type& /*mgr*/ ) const
		{
			return m_offset;
		}

		//Call this method to get real memory address
		void* get_ptr( const mgr_type& mgr )
		{
			return detail::unconst_char( do_get_ptr( mgr ) );
		}

		//Call this method to get real memory address
		const void* get_ptr( const mgr_type& mgr ) const
		{
			return do_get_ptr( mgr );
		}

		bool operator==( const self_type& rhs ) const
		{
			return m_offset == rhs.m_offset;
		}

		bool operator<( const self_type& rhs ) const
		{
			return m_offset < rhs.m_offset;
		}

		bool operator>( const self_type& rhs ) const
		{
			return m_offset > rhs.m_offset;
		}
	private:
		offset_type m_offset;

		const char* do_get_ptr( const mgr_type& mgr ) const
		{
			return detail::shift( mgr.get_base(), m_offset );
		}
	};

	
}


#endif// MGR_OFFSET_POINTER_HEADER
