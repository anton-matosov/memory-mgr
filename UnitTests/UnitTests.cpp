// UnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool test_static_bitset();
bool test_bit_manager();
bool test_memory_manager();

int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
	test_static_bitset();
	test_bit_manager();
	test_memory_manager();	

	return _getch();
}

