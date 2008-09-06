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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#include "stdafx.h"

#include <gstl/algorithm>
#include <vector>

namespace
{
	int array[] = { 54, 758, 448, 548, 66, 1, 40 };
	size_t array_len = GSTL_ARRAY_LEN( array, int );
	int *array_end = array + array_len;
	int array_item = array[ array_len / 2 ];
}

class algorithm_test_fixture
{
public:
	typedef std::vector<int> int_vec_type;
	int_vec_type vec_;
	
	algorithm_test_fixture()
		:vec_( array, array_end )
	{

	}
};


BOOST_FIXTURE_TEST_SUITE( algorithm_test, algorithm_test_fixture );

typedef boost::mpl::list< int, float, double > t_list;

BOOST_AUTO_TEST_CASE( test_non_modifying )
{
	gstl::find( vec_.begin(), vec_.end(), array_item );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_1, type, t_list )
{

}

BOOST_AUTO_TEST_SUITE_END();








