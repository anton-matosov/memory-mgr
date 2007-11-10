#include "StdAfx.h"
#include "test_case.h"
#include "static_bitset.h"

typedef unsigned char block_type;

static const size_t bits_count = 32;
typedef managers::static_bitset<block_type, bits_count, managers::StaticArray> static_bitset;

bool test_static_bitset()
{
	TEST_START( L"static_bitset" );
	static_bitset bitset;

	TEST_METHOD_PRINT( L"size()" );
	TEST_CHECH( bitset.size() == bits_count, L"Size incorrect." );

	TEST_METHOD_PRINT( L"test(0)" );
	TEST_CHECH( bitset.test(0) == false, L"Invalid bitset." );

	TEST_METHOD_PRINT( L"set(0)" );
	bitset.set(0);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(0) == true, L"set(0) doesn't work." );

	TEST_METHOD_PRINT( L"set(1,3)" );
	bitset.set(1,3);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(1,3) == true, L"set(1,3) doesn't work." );
	
	TEST_METHOD_PRINT( L"set(6,5)" );
	bitset.set(6,5);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(6,5) == true, L"set(6,5) doesn't work." );

	TEST_METHOD_PRINT( L"find_first()" );
	static_bitset::size_type pos = bitset.find_first();
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 0, L"find_first() doesn't work." );

	TEST_METHOD_PRINT( L"find_next()" );
	pos = bitset.find_next(pos);
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 1, L"find_next() doesn't work." );

	TEST_METHOD_PRINT( L"find_n(5)" );
	pos = bitset.find_n(5);
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 6, L"find_n(5) doesn't work." );


	TEST_METHOD_PRINT( L"flip(0)" );
	bitset.flip(0);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(0) == false, L"flip(0) doesn't work." );

	TEST_METHOD_PRINT( L"reset(1)" );
	bitset.reset(1);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(1) == false, L"reset(1) doesn't work." );

	TEST_METHOD_PRINT( L"reset(2,3)" );
	bitset.reset(2,3);
	std::cout << bitset << '\n';
	TEST_CHECH( bitset.test(2,3) == false, L"reset(2,3) doesn't work." );


	TEST_METHOD_PRINT( L"reset(pos, 5)" );
	bitset.reset( pos, 5 );
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test( pos, 5 ) == false, L"(pos, 5) doesn't work." );


	return true;
}