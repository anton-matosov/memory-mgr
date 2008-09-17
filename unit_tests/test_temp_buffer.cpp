/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

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


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#include "StdAfx.h"
#include <stdio.h>
#include "test_case.h"
#include <memory-mgr/detail/temp_buffer.h>

using memory_mgr::detail::char_buffer;
using memory_mgr::detail::temp_buffer;

template class temp_buffer<char>;

const char test_array[] = "Test array";
const size_t test_array_len = sizeof( test_array )/sizeof( char );

BOOST_AUTO_TEST_SUITE( test_temp_buffer )

	BOOST_AUTO_TEST_CASE( test_temp_buffer )
	{
		const char* null = 0;
		char_buffer chbuf;
		BOOST_CHECK( chbuf.get() == null );

		chbuf.allocate( 100 );
		BOOST_CHECK_NE( chbuf.get(), null );
		BOOST_CHECK_EQUAL( chbuf.count(), 100U );

		char_buffer chbuf2( 200 );
		BOOST_CHECK_NE( chbuf2.get(), null );
		BOOST_CHECK_EQUAL( chbuf2.count(), 200U );

		char_buffer chbuf3( chbuf );
		BOOST_CHECK_NE( chbuf3.get(), null );
		BOOST_CHECK_EQUAL( chbuf.count(), 100U );

		chbuf = chbuf2;
		BOOST_CHECK_NE( chbuf.get(), null );
		BOOST_CHECK_EQUAL( chbuf.count(), 200U );
		BOOST_CHECK( chbuf.get() != chbuf2.get() );

		chbuf.reallocate( 100 );
		BOOST_CHECK_NE( chbuf.get(), null );
		BOOST_CHECK_EQUAL( chbuf.count(), 100U );
		BOOST_CHECK_EQUAL( chbuf2.count(), 200U );

		chbuf.put( test_array, test_array_len );
		BOOST_CHECK_EQUAL( strcmp( chbuf, test_array ), 0 );
	}

BOOST_AUTO_TEST_SUITE_END();

