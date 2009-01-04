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

BOOST_AUTO_TEST_CASE( test_construction_throw )
{
// 	typedef typename container_type::value_type value_type;
// 	value_type arr[] = { 1, 2, 3 };
// 	value_type arr2[] = { 7, 7, 7, 7, 7 };
// 	value_type val = arr2[0];
// 	size_t arr2_len = GSTL_ARRAY_LEN( arr2 );
// 
// 	//Default constructor
// 	container_type cont1;
// 	BOOST_CHECK_EQUAL( cont1.size(), sz_null );
// 
// 
// 	//Iterators constructor
// 	container_type cont2( arr, GSTL_ARRAY_END( arr ) );
// 	BOOST_CHECK_EQUAL( cont2.size(), sz_three );
// 	BOOST_CHECK_EQUAL_COLLECTIONS( cont2.begin(), cont2.end(), arr, GSTL_ARRAY_END( arr ) );
// 
// 	//Fill constructor
// 	container_type cont3( arr2_len, val );
// 	BOOST_CHECK_EQUAL( cont3.size(), arr2_len );
// 	BOOST_CHECK_EQUAL_COLLECTIONS( cont3.begin(), cont3.end(), arr2, GSTL_ARRAY_END( arr2 ) );
// 
// 	//Integral Iterator constructor
// 	container_type cont4( (int)arr2_len, val );
// 	BOOST_CHECK_EQUAL( cont4.size(), arr2_len );
// 	BOOST_CHECK_EQUAL_COLLECTIONS( cont4.begin(), cont4.end(), arr2, GSTL_ARRAY_END( arr2 ) );
// 
// 	//Copy constructor
// 	container_type cont5( cont2 );
// 	BOOST_CHECK_EQUAL( cont5.size(), cont2.size() );
// 	BOOST_CHECK_EQUAL_COLLECTIONS( cont5.begin(), cont5.end(), cont2.begin(), cont2.end() );


	typedef traced_container_type container_type;
	typedef container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5 };

	const long throw_on = 3;

	value_type::clear();
	value_type::set_throw_ctor( throw_on );
	BOOST_CHECK_THROW( container_type( arr, GSTL_ARRAY_END( arr ) ), value_type::test_exception );
	BOOST_CHECK_EQUAL( value_type::creations(), throw_on );
	BOOST_CHECK_EQUAL( value_type::destructions(), throw_on - 1 );
}


