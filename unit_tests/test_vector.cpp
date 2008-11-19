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

#include <gstl/vector>
#include <vector>
#include "operations_tracer.hpp"
#include "managers.hpp"


class vector_fixture
{
public:
	typedef gstl::test::operations_tracer<int>	tracer_type;
	typedef gstl::vector<tracer_type>			traced_vector_type;

};

BOOST_FIXTURE_TEST_SUITE( vector_test, vector_fixture )

int arr[] = { 1, 2, 3 };
int arr2[] = { 7, 7, 7 };
int val = arr2[0];
size_t count = GSTL_ARRAY_LEN( arr2 );

typedef int								test_value_type;
typedef std::vector<test_value_type>	std_vector;
typedef gstl::vector<test_value_type>	gstl_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::allocator<test_value_type, ptr_alloc_mgr>, gstl::pointer_traits<test_value_type> >			memory_mgr_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_vector;

typedef boost::mpl::list< /**/std_vector,/**/ gstl_vector, memory_mgr_vector/**/, memory_mgr_off_vector/**/> t_list;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_construction, vector_type, t_list )
{
	//Default constructor
	vector_type vec1;
	BOOST_CHECK_EQUAL( vec1.size(), sz_null );
	BOOST_CHECK_GE( vec1.capacity(), sz_null );


	//Iterators constructor
	vector_type vec2( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( vec2.size(), sz_three );
	BOOST_CHECK_GE( vec2.capacity(), sz_three );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec2.begin(), vec2.end(), arr, GSTL_ARRAY_END( arr ) );

	//Fill constructor
	vector_type vec3( count, val );
	BOOST_CHECK_EQUAL( vec3.size(), count );
	BOOST_CHECK_GE( vec3.capacity(), count );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec3.begin(), vec3.end(), arr2, GSTL_ARRAY_END( arr2 ) );

	//Integral Iterator constructor
	vector_type vec4( (int)count, val );
	BOOST_CHECK_EQUAL( vec4.size(), count );
	BOOST_CHECK_GE( vec4.capacity(), count );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec4.begin(), vec4.end(), arr2, GSTL_ARRAY_END( arr2 ) );

	//Copy constructor
	vector_type vec5( vec2 );
	BOOST_CHECK_EQUAL( vec5.size(), vec2.size() );
	BOOST_CHECK_GE( vec5.capacity(), vec2.size() );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec5.begin(), vec5.end(), vec2.begin(), vec2.end() );
}

BOOST_AUTO_TEST_CASE( test_objects_validness )
{
	traced_vector_type vec1;
	long creations = 0;
	long destructions = 0;
	const long fill_insert_overhead = 3;
	BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
	BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

	
	{
		//Iterators constructor
		traced_vector_type vec2( arr, GSTL_ARRAY_END( arr ) );
		creations += GSTL_ARRAY_LEN( arr );
		
		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );
		
		vec2.clear();
		destructions += GSTL_ARRAY_LEN( arr );

		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		vec2.insert( vec2.begin(), count, val );
		creations += count + fill_insert_overhead;
		destructions += fill_insert_overhead;
		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		traced_vector_type vec3 = vec2;
		creations += vec3.size();
		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		vec2 = traced_vector_type();
		destructions += vec3.size();
		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		//Affect of ~vector
		destructions += vec2.size();
		destructions += vec3.size();
	}

	BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
	BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

	tracer_type::clear();
}

BOOST_AUTO_TEST_SUITE_END()


