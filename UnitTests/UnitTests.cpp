// UnitTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool test_static_bitset();
bool test_memory_manager();

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	test_static_bitset();

	test_memory_manager();


	return 0;
}

