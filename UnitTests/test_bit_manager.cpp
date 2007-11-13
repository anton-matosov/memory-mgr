/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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


#include "stdafx.h"
#include "test_case.h"
#include "detail/bit_manager.h"

typedef unsigned char block_type;
static const size_t bits_count = 32;
typedef managers::detail::bit_manager<block_type, bits_count> bit_manager;
typedef bit_manager::size_type ptr_type;
bool test_bit_manager()
{
	TEST_START( L"bit_manager" );
	bit_manager bitmgr;

	TEST_METHOD_PRINT( L"allocate(0)" );
	ptr_type p0 = bitmgr.allocate(0);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p0 == bit_manager::npos );

	TEST_METHOD_PRINT( L"allocate(1)" );
	ptr_type p1 = bitmgr.allocate(1);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p1 == 0 );

	TEST_METHOD_PRINT( L"allocate(18)" );
	ptr_type p2 = bitmgr.allocate(18);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p2 == 1 );

	TEST_METHOD_PRINT( L"deallocate(p2, 18)" );
	bitmgr.deallocate(p2, 18);
	std::cout << bitmgr << '\n';
	p2 = bitmgr.allocate(1);
	TEST_CHECH(  p2 == 1 );
	bitmgr.deallocate(p2, 1);
	
	TEST_METHOD_PRINT( L"deallocate(p1, 1)" );
	bitmgr.deallocate(p1, 1);
	std::cout << bitmgr << '\n';
	p1 = bitmgr.allocate(1);
	TEST_CHECH(  p1 == 0 );
	bitmgr.deallocate(p1, 1);


	return true;
}