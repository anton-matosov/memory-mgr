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
	memory_mgr::allocator<test_value_type, ptr_alloc_mgr> >			memory_mgr_vector;
typedef gstl::vector<test_value_type,
	memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_vector;

typedef gstl::stack<test_value_type, gstl_vector>		gstl_vec_stack;
typedef gstl::stack<test_value_type, memory_mgr_vector>		gstl_memory_mgr_vec_stack;
typedef gstl::stack<test_value_type, memory_mgr_off_vector>		gstl_memory_mgr_off_vec_stack;

typedef boost::mpl::list< /**/std_stack/**/, gstl_vec_stack/**/, 
			gstl_memory_mgr_vec_stack/**/, gstl_memory_mgr_off_vec_stack/**/> t_list;


#include "detail/test_wrapper_construction.hpp"
#include "detail/test_wrapper_compare_operators.hpp"
#include "detail/test_push_pop.hpp"

BOOST_AUTO_TEST_CASE_TEMPLATE( test_top, wrapper_type, t_list )
{
	typedef typename wrapper_type::container_type container_type;
	typedef typename container_type::value_type value_type;

	value_type arr[] = { 1, 2, 3 };
	
	container_type cont( arr, GSTL_ARRAY_END( arr ) );
	
	wrapper_type wrap( cont );
	BOOST_REQUIRE_EQUAL( wrap.size(), cont.size() );

	BOOST_CHECK_EQUAL( wrap.top(), arr[wrap.size() - 1] );
	wrap.pop();

	BOOST_CHECK_EQUAL( wrap.top(), arr[wrap.size() - 1] );
	wrap.pop();

	BOOST_CHECK_EQUAL( wrap.top(), arr[wrap.size() - 1] );
	wrap.pop();
}

BOOST_AUTO_TEST_SUITE_END()


