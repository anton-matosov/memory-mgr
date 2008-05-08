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
#include "detail/temp_buffer.h"

using memory_mgr::detail::char_buffer;
using memory_mgr::detail::temp_buffer;

template class temp_buffer<char>;

const char test_array[] = "Test array";
const size_t test_array_len = sizeof( test_array )/sizeof( char );


bool test_temp_buffer()
{
	TEST_START( L"temp_buffer" );

	TEST_PRINT( L"Testing default constructor" );
	char_buffer chbuf;
	TEST_CHECK( chbuf.get() == 0 );

	TEST_METHOD_PRINT( L"allocate()" );
	chbuf.allocate( 100 );
	TEST_CHECK( chbuf.count() == 100 );

	TEST_PRINT( L"Testing allocating constructor" );
	char_buffer chbuf2( 200 );
	TEST_CHECK( chbuf2.count() == 200 );

	TEST_PRINT( L"Testing copy constructor" );
	char_buffer chbuf3( chbuf );
	TEST_CHECK( chbuf.count() == 100 );

	TEST_OPERATOR_PRINT( L"=" );
	chbuf = chbuf2;
	TEST_CHECK( chbuf.count() == 200 && chbuf != chbuf2 );

	TEST_METHOD_PRINT( L"reallocate()" );
	chbuf.reallocate( 100 );
	TEST_CHECK( chbuf.count() == 100 && chbuf2.count() == 200 );
	
	TEST_METHOD_PRINT( L"put()" );
	chbuf.put( test_array, test_array_len );
	TEST_CHECK( strcmp( chbuf, test_array ) == 0 );

	return true;
}

