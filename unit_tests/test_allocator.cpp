/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
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


#include "test_case.hpp"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <gstl/allocator>


typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> > string_type;
template class std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> >;

typedef std::vector<int, gstl::allocator<int > > vector_type;
template class std::vector<int, gstl::allocator<int > >;

typedef std::map<int, int, std::less<int>,  gstl::allocator< std::pair<const int, int> > > map_type;
template class std::map<int, int, std::less<int>,  gstl::allocator< std::pair<int, int> > >;

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
	return true;
}

bool test_allocator()
{
	TEST_START( L"allocator class" );

	TEST_END( /*unit_test()
		&&*/ std_containers_test()
		);

}

