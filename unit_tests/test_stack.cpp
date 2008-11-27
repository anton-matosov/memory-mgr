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

#include <gstl/stack>
#include <gstl/vector>
#include <stack>
#include <vector>
#include <boost/foreach.hpp>
#include "operations_tracer.hpp"
#include "managers.hpp"

class stack_fixture
{
public:
	typedef gstl::test::operations_tracer<int>	tracer_type;
	//typedef gstl::list<tracer_type>			traced_vector_type;

};

BOOST_FIXTURE_TEST_SUITE( stack_test, stack_fixture )



typedef int								test_value_type;
typedef std::stack<test_value_type>		std_stack;

typedef gstl::vector<test_value_type>	gstl_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::allocator<test_value_type, ptr_alloc_mgr>, gstl::pointer_traits<test_value_type> >			memory_mgr_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_vector;

typedef gstl::stack<test_value_type, gstl_vector>		gstl_vec_stack;
typedef gstl::stack<test_value_type, memory_mgr_vector>		gstl_memory_mgr_vec_stack;
typedef gstl::stack<test_value_type, memory_mgr_off_vector>		gstl_memory_mgr_off_vec_stack;

typedef boost::mpl::list< /**/std_stack/**/, gstl_vec_stack/**, memory_mgr_list/**, memory_mgr_off_list/**/> t_list;


BOOST_AUTO_TEST_CASE_TEMPLATE( test_construction, wrapper_type, t_list )
{
	wrapper_type wrap1;
	BOOST_CHECK_EQUAL( wrap1.size(), sz_null );
	BOOST_CHECK_EQUAL( wrap1.empty(), true );

	typedef typename wrapper_type::container_type container_type;

	container_type cont1;
	wrapper_type wrap2( cont1 );
	BOOST_CHECK_EQUAL( wrap2.size(), sz_null );
	BOOST_CHECK_EQUAL( wrap2.empty(), true );


	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	//Iterators constructor
	container_type cont2( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( cont2.size(), sz_three );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), cont2.end(), arr, GSTL_ARRAY_END( arr ) );

	wrapper_type wrap3( cont2 );
	BOOST_CHECK_EQUAL( wrap3.size(), cont2.size() );
	BOOST_CHECK_EQUAL( wrap3.empty(), false );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_wrapper_compare_operators, wrapper_type, t_list )
{
	typedef typename wrapper_type::container_type container_type;
	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	value_type arr2[] = { 1, 2, 2 };

	container_type cont1( arr, GSTL_ARRAY_END( arr ) );
	container_type cont2( arr, GSTL_ARRAY_END( arr ) );
	container_type cont3( arr2, GSTL_ARRAY_END( arr2 ) );

	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont1.begin(), cont1.end(),
		cont2.begin(), cont2.end() );


	wrapper_type wrap1( cont1 );
	wrapper_type wrap2( cont2 );
	wrapper_type wrap3( cont3 );

	//operator==
	BOOST_CHECK( wrap1 == wrap2 );

	//operator!=
	BOOST_CHECK( wrap1 != wrap3 );

	//operator<
	BOOST_CHECK( wrap3 < wrap1 );

	//operator>
	BOOST_CHECK( wrap1 > wrap3 );

	//operator<=
	BOOST_CHECK( wrap1 <= wrap2 ); //Equal
	BOOST_CHECK( wrap3 <= wrap1 ); //Less

	//operator>=
	BOOST_CHECK( wrap2 >= wrap1 ); //Equal
	BOOST_CHECK( wrap1 >= wrap3 ); //Greater
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_push_pop, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type val = 1;

	container_type cont;
	BOOST_CHECK_EQUAL( cont.size(), sz_null );
	
	cont.push( val );
	BOOST_CHECK_EQUAL( cont.size(), sz_one );
	cont.push( val );
	BOOST_CHECK_EQUAL( cont.size(), sz_two );
	cont.push( val );
	BOOST_CHECK_EQUAL( cont.size(), sz_three );

	cont.pop();
	BOOST_CHECK_EQUAL( cont.size(), sz_two );
	cont.pop();
	BOOST_CHECK_EQUAL( cont.size(), sz_one );
	cont.pop();
	BOOST_CHECK_EQUAL( cont.size(), sz_null );
}


BOOST_AUTO_TEST_SUITE_END()


