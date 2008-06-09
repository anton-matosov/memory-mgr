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


#ifndef MGR_TEST_CASES_HEADER
#define MGR_TEST_CASES_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif


template<class TestType>
long double test_std_new( const int op_repeat, const int per_alloc )
{
	typedef TestType test_type;
	typedef test_type* pointer_type;
	pointer_type *p = new pointer_type[per_alloc];

	TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
	{
		p[TEST_ITERATION_NUMBER] = new test_type;
	}
	TEST_SPLIT_LOOP_STOP_TIMER;
	{
		delete p[TEST_ITERATION_NUMBER];
	}
	TEST_END_LOOP( std::wcout );

	delete[] p;

	return TEST_ELAPCED_MCS;
}

template<class TestType>
long double test_std_new_delete( const int op_repeat, const int per_alloc )
{
	typedef TestType test_type;
	typedef test_type* pointer_type;
	pointer_type *p = new pointer_type[per_alloc];

	TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
	{
		p[TEST_ITERATION_NUMBER] = new test_type;
	}
	TEST_SPLIT_LOOP;
	{
		delete p[TEST_ITERATION_NUMBER];
	}
	TEST_END_LOOP( std::wcout );

	delete[] p;

	return TEST_ELAPCED_MCS;
}


#endif //MGR_TEST_CASES_HEADER
