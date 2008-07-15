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

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <gstl/allocator>
#include "test_common.hpp"

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> > string_type;
template class std::basic_string<wchar_t, std::char_traits<wchar_t>, gstl::allocator<wchar_t> >;

typedef std::vector<int > def_vector_type;
typedef std::vector<int, gstl::allocator<int > > vector_type;
template class std::vector<int, gstl::allocator<int > >;

typedef std::map<int, int, std::less<int>,  gstl::allocator< std::pair<const int, int> > > map_type;
template class std::map<int, int, std::less<int>,  gstl::allocator< std::pair<int, int> > >;

class AllocatorTest 
	:public CppUnit::TestFixture
{
	static const int items_count = 1000;
	def_vector_type vec;
public:
	void setUp()
	{  
		vec.resize( items_count );
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			*it = rand() % items_count;
		}
		std::random_shuffle( vec.begin(), vec.end() );
	}

	//////////////////////////////////////////////////////////////////////////
	void test_vector()
	{
		vector_type new_vec;
		int index = -1;
		new_vec.resize( items_count );
		CPPUNIT_ASSERT( new_vec.size() >= items_count );
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			index = rand() % items_count;
			CPPUNIT_ASSERT( index < items_count );
			new_vec[ index ] = *it;
			CPPUNIT_ASSERT( new_vec[ index ] == *it );
		}
	}

	void test_map()
	{
		map_type map;
		int index = -1;
		for( def_vector_type::iterator it = vec.begin(); it != vec.end(); ++it )
		{
			index = rand() % items_count;
			CPPUNIT_ASSERT( index < items_count );
			map[ index ] = *it;
			CPPUNIT_ASSERT( map[ index ] == *it );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CPPUNIT_TEST_SUITE( AllocatorTest );
	CPPUNIT_TEST( test_vector );
	CPPUNIT_TEST( test_map );
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( AllocatorTest );


