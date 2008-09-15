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

#ifndef MGR_NAMED_ALLOCATOR_HEADER
#define MGR_NAMED_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <string>
#include <map>
#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/allocator.h>


namespace memory_mgr
{
	namespace detail
	{
		template<class MemMgr, class OffsetT>
		struct named_allocator_traits
		{
			typedef MemMgr						mgr_type;
			typedef OffsetT						offset_type;
			//typedef allocator<char, mgr_type>	allocator_type;
			typedef std::allocator<char>	allocator_type;

			typedef std::basic_string< char, std::char_traits<char>, allocator_type>				string_type;
			typedef std::map< string_type, offset_type, std::less<string_type>, allocator_type>		map_type;
			typedef std::pair< const string_type, offset_type >										map_item_type;
		};

		template<class MemMgr, class TraitsT >
		class named_allocator
		{
			typedef MemMgr mgr_type;
		public:
			typedef TraitsT	traits_type;
			typedef	typename traits_type::offset_type		offset_type;
			typedef	typename traits_type::string_type		string_type;
			typedef	typename traits_type::map_type			map_type;
			typedef	typename traits_type::map_item_type		map_item_type;

			bool is_exists( const string_type& name )
			{
				return m_objects.find( name ) != m_objects.end();
			}

			void add_object( const string_type& name, const offset_type offset )
			{
				assert( !is_exists( name ) );
				m_objects[ name ] = offset;
			}

			const offset_type get_object( const string_type& name )
			{
				offset_type offset = offset_traits<offset_type>::invalid_offset;
				map_type::const_iterator fres = m_objects.find( name );
				if( fres != m_objects.end() )
				{
					offset = fres->second;
				}
				return offset;
			}

			const void remove_object( const string_type& name )
			{
				m_objects.erase( name );
			}

			const void remove_object( const offset_type offset )
			{
				map_type::iterator fres = std::find_if( m_objects.begin(), m_objects.end(), 
					boost::bind( &map_item_type::second, boost::lambda::_1 ) == offset );
				if( fres != m_objects.end() )
				{
					m_objects.erase( fres );
				}
			}
		private:
			map_type m_objects;

			static bool less_second( const map_item_type& x, const map_item_type& y )
			{
				return x.second < y.second;
			}
		};
	}
}

#endif //MGR_NAMED_ALLOCATOR_HEADER