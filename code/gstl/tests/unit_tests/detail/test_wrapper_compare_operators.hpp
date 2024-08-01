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


