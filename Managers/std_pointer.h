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

#ifndef MGR_STD_POINTER_HEADER
#define MGR_STD_POINTER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{

	//Standard pointer class
	template< class Mgr >
	class std_pointer
	{
		typedef Mgr mgr_t;
		void* m_pointer;
	public:
		typedef typename mgr_t::size_type size_type;
		//Construct pointer from offset
		explicit std_pointer( const mgr_t& mgr, const size_type offset )
			:m_pointer( unconst_void( shift( mgr.get_base(), offset ) ) )
		{}

		std_pointer( const std_pointer& ptr )
			:m_pointer( ptr.m_pointer )
		{}

		//Construct pointer from memory address
		std_pointer( const mgr_t&, const void* ptr )			
			:m_pointer( unconst_void( ptr ) )
		{}

		std_pointer& operator=( const std_pointer& ptr )				
		{
			m_pointer = unconst_void( ptr.m_pointer );
			return *this;
		}

		//Call this method to get offset
		const size_type get_off( const mgr_t& mgr ) const
		{
			return detail::diff( m_pointer, mgr.get_base() );
		}

		//Call this method to get real memory address
		void* get_ptr( const mgr_t& )
		{
			return m_pointer;
		}

		//Call this method to get real memory address
		const void* get_ptr( const mgr_t& ) const
		{
			return m_pointer;
		}

		bool is_null() const
		{
			return m_pointer != mgr_t::null_ptr.m_pointer;
		}				
	};

}


#endif// MGR_STD_POINTER_HEADER
