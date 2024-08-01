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

#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <gstl/allocator>

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> > string_type;
template class std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> >;

typedef std::vector<size_t > def_vector_type;
typedef std::vector<size_t, gstl::allocator<size_t > > vector_type;
template class std::vector<size_t, gstl::allocator<size_t > >;

typedef std::map<size_t, size_t, std::less<size_t>,  gstl::allocator< std::pair<const size_t, size_t> > > map_type;
template class std::map<size_t, size_t, std::less<size_t>,  gstl::allocator< std::pair<size_t, size_t> > >;

class allocator_test_fixture
{
protected:
	static const size_t items_count = 1000;
	def_vector_type vec;
public:
	allocator_test_fixture()
	{
		vec.resize( items_count );
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			*it = rand() % items_count;
		}
		std::random_shuffle( vec.begin(), vec.end() );
	}
};

BOOST_FIXTURE_TEST_SUITE( allocator_test, allocator_test_fixture )


	BOOST_AUTO_TEST_CASE( test_vector )
	{
		vector_type new_vec;
		size_t index = 0;
		new_vec.resize( items_count );
		BOOST_CHECK_GE( new_vec.size(), items_count );
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			index = rand() % items_count;
			BOOST_CHECK_LT( index, items_count );
			new_vec[ index ] = *it;
			BOOST_CHECK_EQUAL( new_vec[ index ], *it );
		}
	}

	BOOST_AUTO_TEST_CASE( test_map )
	{
		map_type map;
		size_t index = 0;
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			index = rand() % items_count;
			BOOST_CHECK_LT( index, items_count );;
			map[ index ] = *it;
			BOOST_CHECK_EQUAL( map[ index ], *it );
		}
	}

BOOST_AUTO_TEST_SUITE_END()



