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


#include "stdafx.h"
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/allocator.h>
#include <memory-mgr/offset_allocator.h>
#include "managers.h"
#include <string>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::memory_manager<size_t, 1234 * 321, 4> 
	> 
> alloc_mgr;

MGR_DECLARE_MANAGER_CLASS(allocator_manager, alloc_mgr);

//////////////////////////////////////////////////////////////////////////
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, memory_mgr::allocator<wchar_t, 
				allocator_manager, std::allocator<void> > > string_type;

template class std::basic_string<wchar_t, std::char_traits<wchar_t>, memory_mgr::allocator<wchar_t, 
				allocator_manager, std::allocator<void> > >;

typedef memory_mgr::allocator<int, allocator_manager, std::allocator<void> > allocator;
typedef memory_mgr::member_allocator<int, heap_mgr, std::allocator<void> > member_alloc;
typedef memory_mgr::polymorphic_allocator<int, allocator_manager, std::allocator<void> > poly_alloc;
typedef memory_mgr::polymorphic_member_allocator<int, heap_mgr, std::allocator<void> > poly_member_alloc;
//////////////////////////////////////////////////////////////////////////
typedef std::vector<int, memory_mgr::allocator_decorator<int, std::allocator<void> > > decor_alloc_vector_type;
typedef std::vector<int, allocator > vector_type;
typedef std::vector<int, member_alloc > member_alloc_vector_type;
typedef std::vector<int, poly_member_alloc> poly_member_alloc_vector_type;

template class std::vector<int, allocator >;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::allocator< std::pair<int, int>, allocator_manager, std::allocator<void> > map_allocator;
typedef memory_mgr::offset_allocator< std::pair<int, int>, allocator_manager > map_offset_allocator;

typedef std::map<int, int, std::less<int>, map_offset_allocator > map_type;

typedef boost::unordered_map<int, int, boost::hash<int>, std::equal_to<int>, map_offset_allocator > unordered_map_type;

template class std::map<int, int, std::less<int>, map_allocator >;

BOOST_AUTO_TEST_SUITE( test_allocator )

	BOOST_AUTO_TEST_CASE( polymorphic_allocators )
	{
 		heap_mgr mgr;
 		poly_alloc polymorphic_allocator;
 		decor_alloc_vector_type decor_vec( polymorphic_allocator );
 		decor_vec.resize( 100 );
 		decor_vec.resize( 10 );
 
 		poly_member_alloc polymorphic_member_allocator(&mgr);
 		decor_alloc_vector_type decor_vec2( polymorphic_member_allocator );
 		decor_vec2.resize( 100 );
 		decor_vec2.resize( 10 );
	}

 	BOOST_AUTO_TEST_CASE( std_containers_test )
 	{
 		{
 			const int items_count = 1000;
 			vector_type vec;
 			vec.resize( items_count );
 			for( vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
 			{
 				*it = rand() % items_count;
 			}
 			std::random_shuffle( vec.begin(), vec.end() );
 
 			unordered_map_type map;
 			for( vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
 			{
 				map[ rand() % items_count ] = *it;
 			}
 		}
 		BOOST_CHECK( allocator_manager::instance().is_free() );
 	}

BOOST_AUTO_TEST_SUITE_END()



