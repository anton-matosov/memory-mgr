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

BOOST_AUTO_TEST_CASE_TEMPLATE( test_begin_end, contaier_type, t_list )
{
	typedef typename contaier_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };
	value_type first = *arr;
	value_type last = *(GSTL_ARRAY_END( arr ) - 1);

	contaier_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( *cont.begin(), first );
	BOOST_CHECK_EQUAL( *(--cont.end()), last );
	BOOST_CHECK_EQUAL( *cont.rbegin(), last );
	BOOST_CHECK_EQUAL( *(--cont.rend()), first );

	//Const versions
	const contaier_type const_cont( cont );
	BOOST_CHECK_EQUAL( *const_cont.begin(), first );
	BOOST_CHECK_EQUAL( *(--const_cont.end()), last );
	BOOST_CHECK_EQUAL( *const_cont.rbegin(), last );
	BOOST_CHECK_EQUAL( *(--const_cont.rend()), first );
}



