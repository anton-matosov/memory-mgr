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


BOOST_AUTO_TEST_CASE_TEMPLATE( test_erase_range, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	value_type arr_wo_range[] = { 1, 5, 6, 7 };
	size_t range_first_id = 1;
	size_t range_last_id = 4;

	container_type cont( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(), arr, GSTL_ARRAY_END( arr ) );

	typedef typename container_type::iterator iterator;
	iterator range_first = cont.begin();
	iterator range_last = cont.begin();

	gstl::advance( range_first, range_first_id );
	gstl::advance( range_last, range_last_id );

	cont.erase( range_first, range_last );
	BOOST_CHECK_EQUAL_COLLECTIONS( cont.begin(), cont.end(),
		arr_wo_range, GSTL_ARRAY_END( arr_wo_range ) );
}


