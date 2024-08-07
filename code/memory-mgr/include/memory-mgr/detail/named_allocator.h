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

#include "gstl/hash.hpp"
#include "gstl/string"
#include "memory-mgr/allocator_decorator.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/offset_allocator.h"
#include "memory-mgr/singleton_manager.h"

#include "memory-mgr/detail/bit_manager.h"
#include "memory-mgr/detail/decorator_base.h"
#include "memory-mgr/detail/ptr_helpers.h"

#include <boost/unordered_map.hpp>

#include <functional>
#include <map>
#include <string>

namespace memory_mgr
{
	namespace detail
	{
		class named_object
		{
		public:
			typedef ulong	block_offset_type;

			named_object()
				: m_ref_count(1)
			{
			}

			block_offset_type get_offset() const
			{
				return m_offset; 
			}
			
			void set_offset(block_offset_type offset) 
			{
				m_offset = offset;
			}

			unsigned int inc_ref_count()
			{
				return ++m_ref_count;
			}
			
			unsigned int dec_ref_count() 
			{
				return --m_ref_count;
			}

			unsigned int get_ref_count() const
			{
				return m_ref_count;
			}

			bool operator < (const named_object& other) const
			{
				return m_offset < other.m_offset;
			}

		private:
			block_offset_type		m_offset;
			unsigned int	m_ref_count;
		};

		template<class KeyT,
		class MappedT,
		class CompareT,
		class AllocT >
		class map_adapted
			: public std::map<KeyT, MappedT, CompareT, AllocT>
		{
		public:
			typedef std::map<KeyT, MappedT, CompareT, AllocT> base_type;

			map_adapted( const AllocT& alloc )
				:base_type( CompareT(), alloc )
			{

			}
		};


		template<class MemMgr>
		struct gstl_named_allocator_traits
		{
			typedef singleton_manager<MemMgr> mgr_type;

			typedef typename manager_traits<mgr_type>::block_offset_type block_offset_type;
			typedef offset_allocator<char, mgr_type> allocator_type;
			typedef memory_mgr::detail::named_object map_value_type;

			typedef gstl::basic_string< char, gstl::char_traits<char>, allocator_type> string_type;
			typedef std::equal_to<string_type> compare_type;
			typedef boost::unordered_map< string_type, map_value_type,
				boost::hash<string_type>, compare_type, allocator_type> map_type;
			typedef typename map_type::value_type map_item_type;
		};

		template<class MemMgr>
		struct std_named_allocator_traits
		{
			typedef singleton_manager<MemMgr> mgr_type;

			typedef typename manager_traits<mgr_type>::block_offset_type block_offset_type;
			typedef offset_allocator<char, mgr_type> allocator_type;

			typedef std::basic_string< char, std::char_traits<char>, allocator_type> string_type;
			typedef std::less<string_type> compare_type;
			typedef map_adapted< string_type, named_object, compare_type, allocator_type> map_type;
			typedef typename map_type::value_type map_item_type;
		};

		template<class MemMgr, class TraitsT >
		class named_allocator
		{
			typedef MemMgr mgr_type;
		public:
			typedef TraitsT	traits_type;
			typedef	typename traits_type::allocator_type	allocator_type;
			typedef	typename traits_type::block_offset_type	block_offset_type;
			typedef	typename traits_type::string_type		string_type;
			typedef	typename traits_type::map_type			map_type;
			typedef	typename traits_type::compare_type		compare_type;
			typedef	typename traits_type::map_item_type		map_item_type;
			typedef typename string_type::value_type		char_type;

			named_allocator( mgr_type& mgr )
				:m_alloc()
			{
				m_objects = mgr.template get_internal_ptr_as<map_type>( detail::internal_ptr_named_objects, 1, m_alloc );
			}

			bool is_exists( const char_type* name )
			{
				return is_exists( string_type( name, m_alloc ) );
			}

			bool is_exists( const string_type& name )
			{
				return m_objects->find( name ) != m_objects->end();
			}

			bool is_exists( const block_offset_type offset )
			{
				return find_by_offset( offset ) != m_objects->end();
			}

			void add_object( const char_type* name, const block_offset_type offset )
			{
				string_type object_name( name, m_alloc );
				MGR_ASSERT( ! is_exists( object_name ), "object with the same name already exists" );
				
				(*m_objects)[ object_name ].set_offset( (named_object::block_offset_type)offset);
			}

			const block_offset_type get_object( const char_type* name )
			{
				block_offset_type offset = offset_traits<block_offset_type>::invalid_offset;

				string_type object_name( name, m_alloc );
				typename map_type::iterator fres = m_objects->find( object_name );
				if( fres != m_objects->end() )
				{
					named_object& obj = fres->second;
					offset = obj.get_offset();
					obj.inc_ref_count();
				}

				return offset;
			}

			const bool remove_object( const char_type* name )
			{
				bool deleted = false;
				string_type object_name( name, m_alloc );
				typename map_type::iterator fres = m_objects->find( object_name );
				
				deleted = do_remove( fres );

				return deleted;
			}

			typename map_type::iterator find_by_offset( const block_offset_type offset )
			{
				return std::find_if( m_objects->begin(), m_objects->end(), 
						std::bind2nd( std::ptr_fun( &equal_second_val ), offset ) );
			}

			const bool remove_object( const block_offset_type offset )
			{
				bool deleted = false;

				deleted = do_remove( find_by_offset( offset ) );

				return deleted;
			}

		private:
			allocator_type m_alloc;
			map_type* m_objects;

			static bool equal_second_val( const map_item_type x, const block_offset_type val )
			{
				return x.second.get_offset() == val;
			}

			bool do_remove( typename map_type::iterator fres ) 
			{
				if( fres == m_objects->end() )
				{
					//If object is not in map, than we treat it as already deleted
					//so there is no need to delete it again
					return false;
				}

				bool deleted = false;
				named_object& obj = fres->second;

				if ( 0 == obj.dec_ref_count() )
				{
					m_objects->erase( fres );
					deleted = true;
				}
				
				return deleted;
			}
		};
	}
}
