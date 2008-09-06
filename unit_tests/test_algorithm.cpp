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


	typedef std::vector<int> int_vec_type;
	typedef const int_vec_type const_int_vec_type;


	int_vec_type int_vec( array, array_end );
	const_int_vec_type const_int_vec( array, array_end );


}

class algorithm_test_fixture
{
public:
	
	algorithm_test_fixture()
	{

	}

	static bool eq_arr_item( int val )
	{
		return val == array_item;
	}

	template<class InputIterator>
	class for_each_verifier
	{
		typedef InputIterator iterator_type;
		typedef typename gstl::iterator_traits<iterator_type>::value_type value_type;

		

		iterator_type first_;

	public:
		for_each_verifier( iterator_type first )
			:first_( first )
		{

		}
		void operator()( value_type val )
		{
			BOOST_CHECK_EQUAL( *first_, val );
			++first_;
		}
	};
};


BOOST_FIXTURE_TEST_SUITE( algorithm_test, algorithm_test_fixture );

typedef boost::mpl::list< int, float, double > t_list;

BOOST_AUTO_TEST_CASE( test_for_each )
{
	for_each_verifier<int_vec_type::iterator> ver1( int_vec.begin() );
	gstl::for_each( int_vec.begin(), int_vec.end(), ver1 );


	for_each_verifier<int_vec_type::const_iterator> ver2( const_int_vec.begin() );
	gstl::for_each( const_int_vec.begin(), const_int_vec.end(), ver2 );
}

BOOST_AUTO_TEST_CASE( test_non_modifying )
{
	int_vec_type::iterator fres1 = gstl::find( int_vec.begin(), int_vec.end(), array_item );
	BOOST_CHECK_EQUAL( *fres1, array_item );
	int_vec_type::const_iterator fres2 = gstl::find( const_int_vec.begin(), const_int_vec.end(), array_item );
	BOOST_CHECK_EQUAL( *fres2, array_item );

 	int_vec_type::iterator fres3 = gstl::find_if( int_vec.begin(), int_vec.end(), eq_arr_item );
 	BOOST_CHECK_EQUAL( *fres3, array_item );
 	int_vec_type::const_iterator fres4 = gstl::find_if( const_int_vec.begin(), const_int_vec.end(), eq_arr_item );
 	BOOST_CHECK_EQUAL( *fres4, array_item );

	//std::find_end()
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_1, type, t_list )
{

}

BOOST_AUTO_TEST_SUITE_END();








