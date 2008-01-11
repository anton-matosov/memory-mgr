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

namespace memory_mgr
{
	//Offset pointer class
	template< class Mgr >
	class offset_pointer
	{
		typedef Mgr mgr_t;

		const char* do_get_ptr( const mgr_t& mgr ) const
		{
			return detail::shift( mgr.get_base(), m_offset );
		}
		typename mgr_t::size_type m_offset;
	public:
		typedef typename mgr_t::size_type size_type;
		//Construct pointer from offset
		explicit offset_pointer( const size_type offset )
			:m_offset( offset )
		{}

		offset_pointer( const offset_pointer& ptr )
			:m_offset( ptr.m_offset )
		{}

		//Construct pointer from memory address
		offset_pointer( const mgr_t& mgr, const void* ptr )			
			:m_offset( detail::diff( ptr, mgr.get_base() ) )
		{}

		offset_pointer& operator=( const offset_pointer& ptr )				
		{
			m_offset = ptr.m_offset;
			return *this;
		}

		//Call this method to get offset
		const size_type get_off( const mgr_t& /*mgr*/ ) const
		{
			return m_offset;
		}

		//Call this method to get real memory address
		void* get_ptr( const mgr_t& mgr )
		{
			return detail::unconst_char( do_get_ptr( mgr ) );
		}

		//Call this method to get real memory address
		const void* get_ptr( const mgr_t& mgr ) const
		{
			return do_get_ptr( mgr );
		}

		bool is_null() const
		{
			return m_offset != mgr_t::null_ptr.m_offset;
		}	
	};

}


#endif// MGR_OFFSET_POINTER_HEADER
