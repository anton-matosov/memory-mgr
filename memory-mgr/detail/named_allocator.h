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
#include <functional>
#include <map>
#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/allocator.h>
#include <memory-mgr/detail/ptr_helpers.h>


namespace memory_mgr
{
	namespace detail
	{
		template<class MemMgr>
		struct named_allocator_traits
		{
			typedef MemMgr						mgr_type;
			typedef typename manager_traits<mgr_type>::offset_type	offset_type;
			typedef member_allocator<char, mgr_type>	allocator_type;
			//typedef std::allocator<char>		allocator_type;

			typedef std::basic_string< char, std::char_traits<char>, allocator_type>	string_type;
			typedef std::less<string_type>												compare_type;
			typedef std::map< string_type, offset_type, compare_type, allocator_type>	map_type;
			typedef std::pair< const string_type, offset_type >							map_item_type;
		};

		template<class MemMgr, class TraitsT >
		class named_allocator
		{
			typedef MemMgr mgr_type;
		public:
			typedef TraitsT	traits_type;
			typedef	typename traits_type::allocator_type	allocator_type;
			typedef	typename traits_type::offset_type		offset_type;
			typedef	typename traits_type::string_type		string_type;
			typedef	typename traits_type::map_type			map_type;
			typedef	typename traits_type::compare_type		compare_type;
			typedef	typename traits_type::map_item_type		map_item_type;

			named_allocator( mgr_type& mgr )
				:m_alloc( &mgr )
			{
				if( mgr.is_free() )
				{
					typedef member_allocator<map_type, mgr_type> map_alloc_type;
					map_alloc_type map_alloc( &mgr );
					m_objects = map_alloc.allocate( 1 );
					new( m_objects ) map_type( compare_type(), m_alloc );
				}
				else
				{
					m_objects = static_cast<map_type*>( detail::offset_to_pointer( 
						is_category_supported< mgr_type, size_tracking_tag>::value ? 4 : 0, mgr ) );
				}
			}

			bool is_exists( const char* name )
			{
				return is_exists( string_type( name, m_alloc ) );
			}

			bool is_exists( const string_type& name )
			{
				return m_objects->find( name ) != m_objects->end();
			}

			void add_object( const char* name, const offset_type offset )
			{
				string_type object_name( name, m_alloc );
				assert( !is_exists( object_name ) );
				(*m_objects)[ object_name ] = offset;
			}

			const offset_type get_object( const char* name )
			{
				string_type object_name( name, m_alloc );
				offset_type offset = offset_traits<offset_type>::invalid_offset;
				typename map_type::const_iterator fres = m_objects->find( object_name );
				if( fres != m_objects->end() )
				{
					offset = fres->second;
				}
				return offset;
			}

			const void remove_object( const char* name )
			{
				string_type object_name( name, m_alloc );
				m_objects->erase( object_name );
			}

			const void remove_object( const offset_type ptr )
			{
				const offset_type offset = ptr;
				typename map_type::iterator fres = std::find_if( m_objects->begin(), m_objects->end(), 
					std::bind2nd( std::ptr_fun( &equal_second_val<offset_type> ), offset ) );
				if( fres != m_objects->end() )
				{
					m_objects->erase( fres );
				}
			}
		private:
			allocator_type m_alloc;
			map_type* m_objects;

			static bool less_second( const map_item_type& x, const map_item_type& y )
			{
				return x.second < y.second;
			}

			template<class ValT>
			static bool equal_second_val( const map_item_type x, const ValT val )
			{
				return x.second == val;
			}
		};
	}
}

#endif //MGR_NAMED_ALLOCATOR_HEADER
