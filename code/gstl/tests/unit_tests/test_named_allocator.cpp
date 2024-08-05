/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/


#include "managers.hpp"
#include <gstl/string>
#include <gstl/hash.hpp>
#include <memory-mgr/allocator_decorator.h>
#include <memory-mgr/named_objects.h>

template<class Mgr>
struct named_allocator_traits
{
	typedef memory_mgr::member_allocator<char, Mgr>	allocator_type;
	typedef Mgr allocator_mgr_type;

	typedef size_t block_offset_type;
	typedef memory_mgr::detail::named_object				map_value_type;
	
	typedef gstl::basic_string< char, gstl::char_traits<char>, allocator_type> string_type;
	typedef std::equal_to<string_type> compare_type;
	typedef boost::unordered_map< string_type, map_value_type,
		boost::hash<string_type>, compare_type, allocator_type> map_type;
	typedef typename map_type::value_type map_item_type;
};

BOOST_AUTO_TEST_SUITE( named_allocator_test )


BOOST_AUTO_TEST_CASE( test_construct )
{
	//int* named_int = memory_mgr::new_<int, sh_mem_manager_type>( "name" )(12345);
	//int* named_int2 = memory_mgr::new_<int, sh_mem_manager_type>( "name" )();

	//*named_int = *named_int2 = 123;
}


BOOST_AUTO_TEST_SUITE_END()
