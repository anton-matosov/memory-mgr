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

BOOST_AUTO_TEST_CASE_TEMPLATE( test_swap, contaier_type, t_list )
{		
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	value_type arr2[] = { 7, 7, 7, 7, 7 };
	size_t arr_len = GSTL_ARRAY_LEN( arr );
	size_t arr2_len = GSTL_ARRAY_LEN( arr2 );

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	contaier_type cont2( arr2, GSTL_ARRAY_END( arr2 ) );

	cont.swap( cont2 );
	BOOST_CHECK_EQUAL( cont2.size(), arr_len );
	BOOST_CHECK_GE( cont2.capacity(), arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), cont2.end(), arr, GSTL_ARRAY_END( arr ) );

	BOOST_CHECK_EQUAL( cont.size(), arr2_len );
	BOOST_CHECK_GE( cont.capacity(), arr2_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(), arr2, GSTL_ARRAY_END( arr2 ) );

	//swap function
	swap( cont, cont2 );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(), arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), cont2.end(), arr2, GSTL_ARRAY_END( arr2 ) );
}



