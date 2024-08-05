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

BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign_operator, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3 };

	container_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr, GSTL_ARRAY_END( arr ) );

	container_type cont2;
	cont2 = cont;
	BOOST_CHECK_EQUAL( cont2.size(), cont.size() );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		cont2.begin(), cont2.end() );
}



