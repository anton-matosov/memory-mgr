#include "StdAfx.h"
#include "test_case.h"
#include "detail/static_bitset.h"

typedef unsigned char block_type;

static const size_t bits_count = 32;
typedef managers::static_bitset<block_type, bits_count, managers::StaticArray> static_bitset;

bool test_static_bitset()
{
	TEST_START( L"static_bitset" );
	static_bitset bitset;

	TEST_METHOD_PRINT( L"size()" );
	TEST_CHECH( bitset.size() == bits_count );

	TEST_METHOD_PRINT( L"test(0)" );
	TEST_CHECH( bitset.test(0) == false );

	TEST_METHOD_PRINT( L"set(0)" );
	bitset.set(0);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(0) == true );

	TEST_METHOD_PRINT( L"set(1,3)" );
	bitset.set(1,3);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(1,3) == true );
	
	TEST_METHOD_PRINT( L"set(6,5)" );
	bitset.set(6,5);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(6,5) == true );

	TEST_METHOD_PRINT( L"find_first()" );
	static_bitset::size_type pos = bitset.find_first();
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 0 );

	TEST_METHOD_PRINT( L"find_next()" );
	pos = bitset.find_next(pos);
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 1 );

	TEST_METHOD_PRINT( L"find_n(5)" );
	pos = bitset.find_n(5);
	std::cout << bitset << '\n';
	TEST_CHECH(  pos == 6 );


	TEST_METHOD_PRINT( L"flip(0)" );
	bitset.flip(0);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(0) == false );

	TEST_METHOD_PRINT( L"reset(1)" );
	bitset.reset(1);
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test(1) == false );

	TEST_METHOD_PRINT( L"reset(2,3)" );
	bitset.reset(2,3);
	std::cout << bitset << '\n';
	TEST_CHECH( bitset.test(2,3) == false );


	TEST_METHOD_PRINT( L"reset(pos, 5)" );
	bitset.reset( pos, 5 );
	std::cout << bitset << '\n';
	TEST_CHECH(  bitset.test( pos, 5 ) == false );


	return true;
}