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
#include "test_case.h"
#include <memory-mgr/detail/static_bitset.h>

typedef unsigned char block_type;

static const size_t bits_count = 32;
typedef memory_mgr::static_bitset<block_type, bits_count, memory_mgr::static_array> bitset_type;


bool test_static_bitset()
{	
	TEST_START( L"static_bitset" );
	bitset_type bitset;
	bitset.reset();

	TEST_METHOD_PRINT( L"size()" );
	TEST_CHECK( bitset.size() == bits_count );

	TEST_METHOD_PRINT( L"test(0)" );
	TEST_CHECK( bitset.test(0) == false );

	TEST_METHOD_PRINT( L"set(0)" );
	bitset.set(0);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(0) == true );

	TEST_METHOD_PRINT( L"set(1,3)" );
	bitset.set(1,3);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(1,3) == true );
	
	TEST_METHOD_PRINT( L"set(6,5)" );
	bitset.set(6,5);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(6,5) == true );

	TEST_METHOD_PRINT( L"find_first()" );
	bitset_type::size_type pos = bitset.find_first();
	std::wcout << bitset << L'\n';
	TEST_CHECK(  pos == 0 );

	TEST_METHOD_PRINT( L"find_next()" );
	pos = bitset.find_next(pos);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  pos == 1 );

	TEST_METHOD_PRINT( L"find_n(5)" );
	pos = bitset.find_n(5);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  pos == 6 );


	TEST_METHOD_PRINT( L"flip(0)" );
	bitset.flip(0);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(0) == false );

	TEST_METHOD_PRINT( L"reset(1)" );
	bitset.reset(1);
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(1) == false );

	TEST_METHOD_PRINT( L"reset(2,3)" );
	bitset.reset(2,3);
	std::wcout << bitset << L'\n';
	TEST_CHECK( bitset.test(2,3) == false );


	TEST_METHOD_PRINT( L"reset(pos, 5)" );
	bitset.reset( pos, 5 );
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test( pos, 5 ) == false );

	TEST_METHOD_PRINT( L"set(0,all_bits)" );
	bitset.set( 0, bitset_type::num_bits );
	std::wcout << bitset << L'\n';
	TEST_CHECK(  bitset.test(0, bitset_type::num_bits ) == true );

	return true;
}
