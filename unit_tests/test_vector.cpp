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
#include <boost/foreach.hpp>
#include "operations_tracer.hpp"
#include "managers.hpp"

class vector_fixture
{
public:
	typedef gstl::test::operations_tracer<int>	tracer_type;
	typedef gstl::vector<tracer_type>			traced_vector_type;

};

namespace gstl
{
	template <class T, class Alloc, class PtrTraits, class StreamT>
	StreamT& operator<<( StreamT& stream, const vector<T, Alloc, PtrTraits>& vec )
	{
		BOOST_FOREACH( const T& val, vec )
		{
			stream << val << ' ';
		}
		return stream;
	}
}

namespace std
{
	template <class T, class Alloc, class StreamT>
	StreamT& operator<<( StreamT& stream, const vector<T, Alloc>& vec )
	{
		BOOST_FOREACH( const T& val, vec )
		{
			stream << val << ' ';
		}
		return stream;
	}
}

BOOST_FIXTURE_TEST_SUITE( vector_test, vector_fixture )

int arr[] = { 1, 2, 3 };
int arr2[] = { 7, 7, 7, 7, 7 };
int val = arr2[0];
size_t arr_len = GSTL_ARRAY_LEN( arr );
size_t arr2_len = GSTL_ARRAY_LEN( arr2 );

typedef int								test_value_type;
typedef std::vector<test_value_type>	std_vector;
typedef gstl::vector<test_value_type>	gstl_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::allocator<test_value_type, ptr_alloc_mgr>, gstl::pointer_traits<test_value_type> >			memory_mgr_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_vector;

typedef boost::mpl::list< /**/std_vector, gstl_vector/**, memory_mgr_vector/**, memory_mgr_off_vector/**/> t_list;

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
	vector_type vec3( arr2_len, val );
	BOOST_CHECK_EQUAL( vec3.size(), arr2_len );
	BOOST_CHECK_GE( vec3.capacity(), arr2_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec3.begin(), vec3.end(), arr2, GSTL_ARRAY_END( arr2 ) );

	//Integral Iterator constructor
	vector_type vec4( (int)arr2_len, val );
	BOOST_CHECK_EQUAL( vec4.size(), arr2_len );
	BOOST_CHECK_GE( vec4.capacity(), arr2_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec4.begin(), vec4.end(), arr2, GSTL_ARRAY_END( arr2 ) );

	//Copy constructor
	vector_type vec5( vec2 );
	BOOST_CHECK_EQUAL( vec5.size(), vec2.size() );
	BOOST_CHECK_GE( vec5.capacity(), vec2.size() );
	BOOST_CHECK_EQUAL_COLLECTIONS( vec5.begin(), vec5.end(), vec2.begin(), vec2.end() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_clear, contaier_type, t_list )
{
	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );

	cont.clear();
	BOOST_REQUIRE_EQUAL( cont.size(), sz_null );
	BOOST_CHECK_GE( cont.capacity(), sz_null );

}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_empty, contaier_type, t_list )
{
	contaier_type cont;
	BOOST_REQUIRE_EQUAL( cont.size(), sz_null );
	BOOST_CHECK( cont.empty() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_access_operators, contaier_type, t_list )
{
	size_t test_pos = arr_len / 2;

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_REQUIRE_EQUAL( cont.size(), arr_len );

	BOOST_CHECK_EQUAL( cont[test_pos],  arr[test_pos] );

	/*
	Requires: pos < size()
	Throws: out_of_range if pos >= size().
	Returns: operator[](pos).
	*/
	BOOST_CHECK_EQUAL( cont.at( test_pos ),  cont[test_pos] );
	BOOST_CHECK_THROW( cont.at( cont.size() ),  std::out_of_range );
	BOOST_CHECK_THROW( cont.at( cont.size() + 1 ),  std::out_of_range );

	//Constant container test
	const contaier_type const_cont( cont );
	BOOST_REQUIRE_EQUAL( const_cont.size(), cont.size() );

	BOOST_CHECK_EQUAL( const_cont[test_pos],  arr[test_pos] );
	BOOST_CHECK_EQUAL( const_cont.at( test_pos ),  cont[test_pos] );
	BOOST_CHECK_THROW( const_cont.at( const_cont.size() ),  std::out_of_range );
	BOOST_CHECK_THROW( const_cont.at( const_cont.size() + 1 ),  std::out_of_range );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_swap, contaier_type, t_list )
{		
	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );

	contaier_type cont2( arr2, GSTL_ARRAY_END( arr2 ) );

	cont.swap( cont2 );
	BOOST_CHECK_EQUAL( cont2.size(), arr_len );
	BOOST_CHECK_GE( cont2.capacity(), arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), cont2.end(), arr, GSTL_ARRAY_END( arr ) );

	BOOST_CHECK_EQUAL( cont.size(), arr2_len );
	BOOST_CHECK_GE( cont.capacity(), arr2_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(), arr2, GSTL_ARRAY_END( arr2 ) );
}

//21.3.7 basic_string non-member functions
BOOST_AUTO_TEST_CASE_TEMPLATE( test_compare_operators, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	value_type arr2[] = { 1, 2, 2 };

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	contaier_type cont2( arr, GSTL_ARRAY_END( arr ) );
	contaier_type cont3( arr2, GSTL_ARRAY_END( arr2 ) );

	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		cont2.begin(), cont2.end() );

	//21.3.7.2 operator==
	BOOST_CHECK_EQUAL( cont, cont2 );

	//21.3.7.3 operator!=
	BOOST_CHECK_NE( cont, cont3 );

	//21.3.7.4 operator<
	BOOST_CHECK_LT( cont3, cont );

	//21.3.7.5 operator>
	BOOST_CHECK_GT( cont, cont3 );

	//21.3.7.6 operator<=
	BOOST_CHECK_LE( cont, cont2 ); //Equal
	BOOST_CHECK_LE( cont3, cont ); //Less

	//21.3.7.7 operator>=
	BOOST_CHECK_GE( cont2, cont ); //Equal
	BOOST_CHECK_GE( cont, cont3 ); //Greater

	//21.3.7.8 swap
	swap( cont, cont3 );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(), arr2, GSTL_ARRAY_END( arr2 ) );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont3.begin(), cont3.end(), arr, GSTL_ARRAY_END( arr ) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign_operator, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, GSTL_ARRAY_END( arr ) );

	contaier_type cont2;
	cont2 = cont;
	BOOST_CHECK_EQUAL( cont2.size(), cont.size() );
	BOOST_CHECK_GE( cont2.capacity(), cont.size() );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		cont2.begin(), cont2.end() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_begin_end, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	value_type first = *arr;
	value_type last = *(GSTL_ARRAY_END( arr ) - 1);

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( *cont.begin(), first );
	BOOST_CHECK_EQUAL( *(cont.end() - 1), last );
	BOOST_CHECK_EQUAL( *cont.rbegin(), last );
	BOOST_CHECK_EQUAL( *(cont.rend() - 1), first );

	//Const versions
	const contaier_type const_cont( cont );
	BOOST_CHECK_EQUAL( *const_cont.begin(), first );
	BOOST_CHECK_EQUAL( *(const_cont.end() - 1), last );
	BOOST_CHECK_EQUAL( *const_cont.rbegin(), last );
	BOOST_CHECK_EQUAL( *(const_cont.rend() - 1), first );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_reserve, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	size_t arr_len = GSTL_ARRAY_LEN( arr );
	size_t double_len = arr_len * 2;

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( cont.size(), arr_len );
	BOOST_CHECK_GE( cont.capacity(), arr_len );

	cont.reserve( double_len );
	BOOST_CHECK_EQUAL( cont.size(), arr_len );
	BOOST_CHECK_GE( cont.capacity(), double_len );

	/*
	Calling reserve() with a res_arg argument less than capacity() is in effect a non-binding shrink
	request. A call with res_arg <= size() is in effect a non-binding shrink-to-fit request.
	*/
	
	cont.reserve( 0 );
	//Not implemented in any known STL implementation
	//BOOST_CHECK_LT( cont.capacity(), double_len );
	BOOST_CHECK_GE( cont.capacity(), cont.size() );

	//resize/reserve Throws: length_error if n > max_size().
	//218)reserve() uses Allocator::allocate() which may throw an appropriate exception.
	BOOST_CHECK_THROW( cont.reserve( cont.max_size() + 1 ), std::length_error );	
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_resizee, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	size_t arr_len = GSTL_ARRAY_LEN( arr );
	size_t arr_len_2 = arr_len / 2;	
	size_t extra_size = 5;
	size_t new_size = arr_len + extra_size;
	value_type val = arr[arr_len_2];


	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( cont.size(), arr_len );
	BOOST_CHECK_GE( cont.capacity(), arr_len );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, GSTL_ARRAY_END( arr ) );

	/*
	Resize effects:
		if (sz > size())
			insert(end(), sz-size(), c);	// 1
		else if (sz < size())
		{
			iterator i = begin();			// 2
			advance(i, sz);
			erase(i, end());
		}
		else								// 3
			;
	*/
	/************************************************************************/
	/* Case 1                                                               */
	/************************************************************************/
	contaier_type cont2( arr, GSTL_ARRAY_END( arr ) );
	cont2.resize( new_size, val );				
	BOOST_CHECK_EQUAL( cont2.size(), new_size );
	BOOST_CHECK_GE( cont2.capacity(), new_size );
	typename contaier_type::iterator old_end = cont2.begin();
	gstl::advance( old_end, arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), old_end,
		arr, GSTL_ARRAY_END( arr ) );

	test_compare_n_values( old_end, val, extra_size - 1 );
	

	/************************************************************************/
	/* Case 2                                                               */
	/************************************************************************/	
	cont.resize( arr_len_2 );
	BOOST_CHECK_EQUAL( cont.size(), arr_len_2 );
	BOOST_CHECK_GE( cont.capacity(), arr_len_2 );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, arr + arr_len_2 );

	/************************************************************************/
	/* Case 3                                                               */
	/************************************************************************/
	contaier_type cont3( arr, GSTL_ARRAY_END( arr ) );
	cont3.resize( cont3.size(), val );				
	BOOST_CHECK_EQUAL( cont3.size(), arr_len );
	BOOST_CHECK_GE( cont3.capacity(), arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont3.begin(), cont3.end(),
		arr, GSTL_ARRAY_END( arr ) );

	//resize/reserve Throws: length_error if n > max_size().
	//218)reserve() uses Allocator::allocate() which may throw an appropriate exception.
	BOOST_CHECK_THROW( cont.resize( cont.max_size() + 1 ), std::length_error );
	BOOST_CHECK_THROW( cont.resize( cont.max_size() + 1, val ), std::length_error );	
}

BOOST_AUTO_TEST_CASE( test_objects_validness )
{
	tracer_type::clear();
	traced_vector_type vec1;
	long creations = 0;
	long destructions = 0;
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

		vec2.insert( vec2.begin(), arr2_len, val );
		const long fill_insert_overhead = 5;
		creations += arr2_len + fill_insert_overhead;
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


