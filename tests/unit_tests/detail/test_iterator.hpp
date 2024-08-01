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

BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator_default_constructor, container_type, t_list )
{
	typename container_type::iterator iter;
	typename container_type::const_iterator const_iter;

	gstl::helpers::unused_variable( iter );
	gstl::helpers::unused_variable( const_iter );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator_const_assign, container_type, t_list )
{
	typename container_type::iterator iter;
	typename container_type::const_iterator const_iter = iter;

	gstl::helpers::unused_variable( iter );
	gstl::helpers::unused_variable( const_iter );
}

#ifdef GSTL_ASSERT_EXCEPTION

BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator_diff_containers, container_type, t_list )
{
	if( ! gstl::detail::is_pointer_to_any_location<container_type::pointer>::value )
	{
		return;
	}

	container_type container1;
	container_type container2;

	BOOST_CHECK_THROW( container1.begin() != container2.end(), gstl::assert_exception );
	BOOST_CHECK_THROW( container1.end() != container2.begin(), gstl::assert_exception );

	BOOST_CHECK_THROW( container1.rbegin() != container2.rend(), gstl::assert_exception );
	BOOST_CHECK_THROW( container1.rend() != container2.rbegin(), gstl::assert_exception );
}

#endif