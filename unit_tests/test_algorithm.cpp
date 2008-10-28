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

class iterator_shifter
{
public:
	template<class IterT>
	iterator_shifter( IterT& iter, size_t offset )
	{
		gstl::advance( iter, offset );
	}
};

#define ARRAY_NAME( name )	name##array

#define DECLARE_INT_CONTAINER( name, array_values, cont_type )	\
	int ARRAY_NAME( name )[] = {array_values};					\
	cont_type<int>	name( ARRAY_NAME( name ),					\
	ARRAY_NAME( name ) + GSTL_ARRAY_LEN( ARRAY_NAME( name ) ) );


#define DECLARE_INT_VECTOR( name, array_values )	\
	DECLARE_INT_CONTAINER( name, array_values, std::vector )

#define DECLARE_INT_LIST( name, array_values )	\
	DECLARE_INT_CONTAINER( name, array_values, std::list )

#define DECLARE_INT_CONST_VECTOR( name, array_values )	\
	DECLARE_INT_CONTAINER( name, array_values, const std::vector )

#define DECLARE_INT_CONST_LIST( name, array_values )	\
	DECLARE_INT_CONTAINER( name, array_values, const std::list )

#define DECLARE_ITER( name, offset, cont_type, cont )	\
	cont_type::const_iterator name = cont.begin();		\
	iterator_shifter name##shifter( name, offset );

namespace
{
	typedef std::vector<int>	int_vec_type;
	typedef std::list<int>		int_list_type;
	typedef int_vec_type::const_iterator int_vec_iter;
	typedef int_list_type::const_iterator int_list_iter;
	
#define ARRAY_ITEMS  1, 2, 3, 3, 2, 3, 3, 1, 2, 2, 3


	DECLARE_INT_VECTOR( int_vec, ARRAY_ITEMS );
	DECLARE_INT_CONST_VECTOR( const_int_vec, ARRAY_ITEMS );
	DECLARE_INT_LIST( int_list, ARRAY_ITEMS );

	//////////////////////////////////////////////////////////////////////////
	//	{ 1, 2, 3, 3, 2, 3, 3, 1, 2, 2, 3 }
	//         |----|               |-----|
	//    0  1  2  3  4  5  6  7  8  9  10
	//         search              find_end
	//////////////////////////////////////////////////////////////////////////

#define COMMA  ,

	DECLARE_INT_VECTOR( search_vec, 3 COMMA 3 );
	DECLARE_ITER( search_vec_fres, 2, int_vec_type, int_vec );
	DECLARE_ITER( search_list_fres, 2, int_list_type, int_list );

	DECLARE_INT_VECTOR( find_end_vec, 2 COMMA 3 );
	DECLARE_ITER( find_end_vec_fres, 9, int_vec_type, int_vec );
	DECLARE_ITER( find_end_list_fres, 9, int_list_type, int_list );
}

class algorithm_test_fixture
{
public:
	
	algorithm_test_fixture()
	{

	}

	static bool eq_arr_item( int val )
	{
		return val == arr_item_;
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

	static int arr_item_;
};
int algorithm_test_fixture::arr_item_ = 0;

BOOST_FIXTURE_TEST_SUITE( algorithm_test, algorithm_test_fixture );

typedef boost::mpl::list< int, float, double > t_list;

BOOST_AUTO_TEST_CASE( test_for_each )
{
	for_each_verifier<int_vec_iter> ver1( int_vec.begin() );
	gstl::for_each( int_vec.begin(), int_vec.end(), ver1 );


	for_each_verifier<int_vec_iter> ver2( const_int_vec.begin() );
	gstl::for_each( const_int_vec.begin(), const_int_vec.end(), ver2 );
}



BOOST_AUTO_TEST_CASE( test_find )
{
	arr_item_ = 3;
	int_vec_iter fres1 = gstl::find( int_vec.begin(), int_vec.end(), arr_item_ );
	BOOST_CHECK_EQUAL( *fres1, arr_item_ );

	int_vec_iter fres2 = gstl::find( int_vec.begin(), int_vec.end(), 0 );
	BOOST_CHECK( fres2 == int_vec.end() );
}

BOOST_AUTO_TEST_CASE( test_find_if )
{
	arr_item_ = 3;
	int_vec_iter fres1 = gstl::find_if( int_vec.begin(), int_vec.end(), eq_arr_item );
	BOOST_CHECK_EQUAL( *fres1, arr_item_ );

	arr_item_ = 0;
	int_vec_iter fres2 = gstl::find_if( int_vec.begin(), int_vec.end(), eq_arr_item );
	BOOST_CHECK( fres2 == int_vec.end() );
}

BOOST_AUTO_TEST_CASE( test_search )
{
	//RanIt version
	int_vec_iter fres = gstl::search( int_vec.begin(), int_vec.end(),
		search_vec.begin(), search_vec.end(), std::equal_to<int>() );
	BOOST_CHECK_EQUAL( *fres, *search_vec_fres );
	BOOST_CHECK( fres == search_vec_fres );

	//FwdIt version
	int_list_iter list_fres = gstl::search( int_list.begin(), int_list.end(),
		search_vec.begin(), search_vec.end(), std::equal_to<int>() );
	BOOST_CHECK_EQUAL( *list_fres, *search_list_fres );
	BOOST_CHECK( list_fres == search_list_fres );
}

BOOST_AUTO_TEST_CASE( test_find_end )
{
	//RanIt version
	int_vec_iter fres = gstl::find_end( int_vec.begin(), int_vec.end(),
		find_end_vec.begin(), find_end_vec.end(), std::equal_to<int>() );
	BOOST_CHECK_EQUAL( *fres, *find_end_vec_fres );
	BOOST_CHECK( fres == find_end_vec_fres );

	//FwdIt version
	int_list_iter list_fres = gstl::find_end( int_list.begin(), int_list.end(),
		find_end_vec.begin(), find_end_vec.end(), std::equal_to<int>() );
	BOOST_CHECK_EQUAL( *list_fres, *find_end_list_fres );
	BOOST_CHECK( list_fres == find_end_list_fres );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( test_1, type, t_list )
{

}

BOOST_AUTO_TEST_SUITE_END();








