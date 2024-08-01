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

#include <gstl/utility>
#include "operations_tracer.hpp"


class utilities_test_fixture
{
public:
};

namespace gstl
{

	template <class T1, class T2, class StreamT>
	StreamT& operator<<( StreamT& stream, const pair<T1, T2>& p )
	{
		return stream << p.first << ' ' << p.second;
	}
}

BOOST_FIXTURE_TEST_SUITE( utilities_test, utilities_test_fixture );

typedef boost::mpl::list< int, float, double > t_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_1, type, t_list )
	{
		
	}

	BOOST_AUTO_TEST_CASE( test_rel_ops )
	{
		typedef gstl::test::operations_tracer<int> test_type;
		test_type v1 = 1;
		test_type v2 = 1;
		test_type v3 = 2;

		BOOST_REQUIRE( v1 == v2 );
		BOOST_REQUIRE( v1 < v3 );

		BOOST_CHECK( gstl::rel_ops::operator !=( v1, v3 ) );

		BOOST_CHECK( gstl::rel_ops::operator <=( v1, v2 ) ); //Equal
		BOOST_CHECK( gstl::rel_ops::operator <=( v1, v3 ) ); //Less

		BOOST_CHECK( gstl::rel_ops::operator >=( v1, v2 ) ); //Equal
		BOOST_CHECK( gstl::rel_ops::operator >=( v3, v1 ) ); //Greater

		BOOST_CHECK( gstl::rel_ops::operator >( v3, v1 ) ); //Greater
	}

	GSTL_TEST_CASE_TEMPLATE_TWO_LISTS( test_pair, type_1, type_2, t_list, t_list  )
	{
		type_1 v11( 1 );
		type_1 v12( 2 );
		type_2 v21( 3 );
		type_2 v22( 4 );
		gstl::pair<type_1, type_2> pair1 = gstl::make_pair( v11, v21 );
		gstl::pair<type_2, type_1> pair2 = gstl::make_pair( v21, v11 );
		gstl::pair<type_1, type_1> pair3 = gstl::make_pair( v11, v12 );
		gstl::pair<type_2, type_2> pair4 = gstl::make_pair( v21, v22 );

		BOOST_CHECK_EQUAL( pair1.first, v11 );
		BOOST_CHECK_EQUAL( pair1.second, v21 );


		BOOST_CHECK_LT( v11, v12 );
		BOOST_CHECK_LT( v21, v22 );
		
		BOOST_CHECK_EQUAL( pair1, pair1 );
		BOOST_CHECK_EQUAL( pair2, pair2 );
		BOOST_CHECK_EQUAL( pair1, pair1 );
		BOOST_CHECK_EQUAL( pair1, pair1 );
		BOOST_CHECK_EQUAL( pair1, pair1 );
		BOOST_CHECK_EQUAL( pair1, pair1 );
	}

BOOST_AUTO_TEST_SUITE_END();








