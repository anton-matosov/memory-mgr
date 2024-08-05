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


BOOST_AUTO_TEST_CASE_TEMPLATE( test_resize, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
	size_t arr_len = GSTL_ARRAY_LEN( arr );
	size_t arr_len_2 = arr_len / 2;	
	size_t zero_length = 0;
	size_t extra_size = 5;
	size_t new_size = arr_len + extra_size;
	value_type val = arr[arr_len_2];

	/*
	Resize effects:
	if (sz > size())
	insert(end(), sz-size(), c);	// 1
	else if (sz < size())
	{
	iterator i = begin();			// 2
	advance(i, sz);
	erase(i, end());
	}
	else								// 3
	;
	*/
	/************************************************************************/
	/* Case 1                                                               */
	/************************************************************************/
	container_type case1_container( arr, GSTL_ARRAY_END( arr ) );
	case1_container.resize( new_size, val );				
	BOOST_CHECK_EQUAL( case1_container.size(), new_size );

	typename container_type::iterator old_end = case1_container.begin();
	gstl::advance( old_end, arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( case1_container.begin(), old_end,
		arr, GSTL_ARRAY_END( arr ) );

	test_compare_n_values( old_end, val, extra_size - 1 );


	/************************************************************************/
	/* Case 2                                                               */
	/************************************************************************/	
	container_type case2_container( arr, GSTL_ARRAY_END( arr ) );
	BOOST_CHECK_EQUAL( case2_container.size(), arr_len );
	BOOST_REQUIRE_EQUAL_COLLECTIONS( case2_container.begin(), case2_container.end(),
		arr, GSTL_ARRAY_END( arr ) );

	case2_container.resize( arr_len_2 );
	BOOST_CHECK_EQUAL( case2_container.size(), arr_len_2 );
	BOOST_CHECK_EQUAL_COLLECTIONS( case2_container.begin(), case2_container.end(),
		arr, arr + arr_len_2 );

	case2_container.resize( zero_length );
	BOOST_CHECK_EQUAL( case2_container.size(), zero_length );
	BOOST_CHECK_EQUAL_COLLECTIONS( case2_container.begin(), case2_container.end(),
		arr, arr + zero_length );

	/************************************************************************/
	/* Case 3                                                               */
	/************************************************************************/
	container_type case3_container( arr, GSTL_ARRAY_END( arr ) );
	case3_container.resize( case3_container.size(), val );				
	BOOST_CHECK_EQUAL( case3_container.size(), arr_len );
	BOOST_CHECK_EQUAL_COLLECTIONS( case3_container.begin(), case3_container.end(),
		arr, GSTL_ARRAY_END( arr ) );	
}



