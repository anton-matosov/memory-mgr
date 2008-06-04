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
#include "test_case.h"
#include "memory_manager.h"
#include "singleton_manager.h"
#include "size_tracking.h"
#include "heap_segment.h"
#include "allocator.h"
#include <string>
#include <vector>
#include <map>

typedef memory_mgr::singleton_manager
< 
	memory_mgr::size_tracking
	<
		memory_mgr::pointer_convert
		< 
			memory_mgr::heap_segment
			< 
				memory_mgr::memory_manager<size_t, 1234 * 321, 4> 
			> 
		>
	>
> alloc_mgr;

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, memory_mgr::allocator<wchar_t, 
alloc_mgr> > string_type;
template class std::basic_string<wchar_t, std::char_traits<wchar_t>, memory_mgr::allocator<wchar_t, 
alloc_mgr> >;

typedef std::vector<int, memory_mgr::allocator<int, 
alloc_mgr> > vector_type;
template class std::vector<int, memory_mgr::allocator<int, 
alloc_mgr> >;

typedef std::map<int, int, std::less<int>,  memory_mgr::allocator< std::pair<int, int>, 
alloc_mgr> > map_type;
template class std::map<int, int, std::less<int>,  memory_mgr::allocator< std::pair<int, int>, 
alloc_mgr> >;

bool unit_test()
{
	return false;
}

bool std_containers_test()
{
	SUBTEST_START( L"std containers + custom allocator" );
	{
		const int items_count = 1000;
		vector_type vec;
		vec.resize( items_count );
		for( vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			*it = rand() % items_count;
		}
		std::random_shuffle( vec.begin(), vec.end() );

		map_type map;
		for( vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			map[ rand() % items_count ] = *it;
		}
	}
	SUBTEST_END( alloc_mgr::instance().free() );
}

bool test_allocator()
{
	TEST_START( L"allocator class" );

	TEST_END( /*unit_test()
		&&*/ std_containers_test()
		);

}