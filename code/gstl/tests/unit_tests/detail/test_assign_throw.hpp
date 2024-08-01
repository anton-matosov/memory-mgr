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

/**
   @brief		exception safety for operator=
   @details		unit test that checks exception safety for operator=
   @note		requires that "traced_container_type" type is defined 
				as container with operations_tracer class as value type
				e.g. vector<operations_tracer>
*/
BOOST_AUTO_TEST_CASE( test_assign_throw )
{
	typedef traced_container_type container_type;
	typedef container_type::value_type value_type;
	
	value_type::clear();

	value_type arr[] = { 1, 2, 3, 4, 5 };
	value_type val = arr[0];
	size_t arr_len = GSTL_ARRAY_LEN( arr );

	const long throw_on = 3;
	container_type cont( arr, GSTL_ARRAY_END( arr ) );
	
	container_type cont2;
	
	value_type::clear();
	value_type::set_throw_ctor( throw_on );
	BOOST_CHECK_THROW( cont2 = cont, value_type::test_exception );
	BOOST_CHECK_EQUAL( value_type::creations(), throw_on );
	BOOST_CHECK_EQUAL( value_type::destructions(), throw_on - 1 );
	BOOST_CHECK_EQUAL( cont2.size(), sz_null );

	value_type::clear();
	value_type::set_throw_ctor( throw_on );
	BOOST_CHECK_THROW( cont2.assign( cont.begin(), cont.end() ), value_type::test_exception );
	BOOST_CHECK_EQUAL( value_type::creations(), throw_on );
	BOOST_CHECK_EQUAL( value_type::destructions(), throw_on - 1 );
	BOOST_CHECK_EQUAL( cont2.size(), sz_null );

	value_type::clear();
	value_type::set_throw_ctor( throw_on );
	BOOST_CHECK_THROW( cont2.assign( arr_len, val ), value_type::test_exception );
	BOOST_CHECK_EQUAL( value_type::creations(), throw_on );
	BOOST_CHECK_EQUAL( value_type::destructions(), throw_on - 1 );
	BOOST_CHECK_EQUAL( cont2.size(), sz_null );
}



