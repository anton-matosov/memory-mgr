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

BOOST_AUTO_TEST_CASE_TEMPLATE( test_reserve, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	size_t arr_len = GSTL_ARRAY_LEN( arr );
	size_t double_len = arr_len * 2;

	container_type cont( arr, GSTL_ARRAY_END( arr ) );
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

}



