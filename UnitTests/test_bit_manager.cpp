
#include "stdafx.h"
#include "test_case.h"
#include "bit_manager.h"

typedef unsigned char block_type;
static const size_t bits_count = 32;
typedef managers::bit_manager<block_type, bits_count> bit_manager;
typedef bit_manager::size_type ptr_type;
bool test_bit_manager()
{
	TEST_START( L"static_bitset" );
	bit_manager bitmgr;

	TEST_METHOD_PRINT( L"allocate(0)" );
	ptr_type p0 = bitmgr.allocate(0);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p0 == bit_manager::npos, L"allocate(0) work incorrectly." );

	TEST_METHOD_PRINT( L"allocate(1)" );
	ptr_type p1 = bitmgr.allocate(1);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p1 == 0, L"allocate(1) work incorrectly." );

	TEST_METHOD_PRINT( L"allocate(18)" );
	ptr_type p2 = bitmgr.allocate(18);
	std::cout << bitmgr << '\n';
	TEST_CHECH(  p2 == 1, L"allocate(18) work incorrectly." );

	TEST_METHOD_PRINT( L"deallocate(p2, 18)" );
	bitmgr.deallocate(p2, 18);
	std::cout << bitmgr << '\n';
	p2 = bitmgr.allocate(1);
	TEST_CHECH(  p2 == 1, L"deallocate(p2, 18) work incorrectly." );
	bitmgr.deallocate(p2, 1);
	
	TEST_METHOD_PRINT( L"deallocate(p1, 1)" );
	bitmgr.deallocate(p1, 1);
	std::cout << bitmgr << '\n';
	p1 = bitmgr.allocate(1);
	TEST_CHECH(  p1 == 0, L"deallocate(p1, 1) work incorrectly." );
	bitmgr.deallocate(p1, 1);


	return true;
}