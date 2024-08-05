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


BOOST_AUTO_TEST_CASE_TEMPLATE( test_resize_reserve_throw, container_type, t_list )
{
	typedef typename container_type::value_type value_type;
	value_type val = value_type();
	container_type cont;
	
	//resize/reserve Throws: length_error if n > max_size().
	//218)reserve() uses Allocator::allocate() which may throw an appropriate exception.
	BOOST_CHECK_THROW( cont.resize( cont.max_size() + 1 ), std::length_error );
	BOOST_CHECK_THROW( cont.resize( cont.max_size() + 1, val ), std::length_error );

	BOOST_CHECK_THROW( cont.reserve( cont.max_size() + 1 ), std::length_error );	
}



