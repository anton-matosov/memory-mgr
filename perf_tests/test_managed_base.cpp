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

#include <iostream>
#include "detail/test.h"
#include "managers.h"

namespace
{
	template<class MemMgr>
	class test_class: public memory_mgr::managed_base< sing_shared_sz_pt_mgr >
	{
		int m_i;
	};

	class test_class2
	{
		int m_i;
	};

	//////////////////////////////////////////////////////////////////////////
	// new class
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr>
	long double test_new_mem_mgr( const int count )
	{
		MemMgr mgr;

		TEST_START_LOOP( count );
		new( mem_mgr( mgr ) ) test_class2;
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_singleton_mem_mgr( const int count )
	{		
		MemMgr::instance();
		TEST_START_LOOP( count );
		new( mem_mgr( MemMgr::instance() ) ) test_class2;
		TEST_END_LOOP( std::wcout );
		MemMgr::destruct();
		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_managed_singleton_mem_mgr( const int count )
	{
		typedef test_class<MemMgr> test_class;

		MemMgr::instance();
		TEST_START_LOOP( count );
		new test_class;
		TEST_END_LOOP( std::wcout );
		MemMgr::destruct();
		return TEST_ELAPCED_MCS;
	}

	long double test_std_new_class( const int count )
	{

		TEST_START_LOOP( count );
		new test_class2;
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	// new/delete class
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr>
	long double test_new_delete_mem_mgr( const int count )
	{
		MemMgr mgr;

		TEST_START_LOOP( count );
		do_delete( new( mem_mgr( mgr ) ) test_class2, mem_mgr( mgr ) );
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_delete_singleton_mem_mgr( const int count )
	{		
		MemMgr::instance();
		TEST_START_LOOP( count );
		do_delete( new( mem_mgr( MemMgr::instance() ) ) test_class2,
			mem_mgr( MemMgr::instance() ) );
		TEST_END_LOOP( std::wcout );
		MemMgr::destruct();
		return TEST_ELAPCED_MCS;
	}
	
	template<class MemMgr>
	long double test_new_delete_managed_singleton_mem_mgr( const int count )
	{
		typedef test_class<MemMgr> test_class;

		MemMgr::instance();
		TEST_START_LOOP( count );
		delete new test_class;
		TEST_END_LOOP( std::wcout );
		MemMgr::destruct();

		return TEST_ELAPCED_MCS;
	}

	long double test_std_new_delete_class( const int count )
	{

		TEST_START_LOOP( count );
		delete new test_class2;
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	// new/delete int
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr>
	long double test_new_delete_int_mem_mgr( const int count )
	{
		MemMgr mgr;

		TEST_START_LOOP( count );
		do_delete( new( mem_mgr( mgr ) ) int, mem_mgr( mgr ) );
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_delete_int_singleton_mem_mgr( const int count )
	{		
		MemMgr::instance();
		TEST_START_LOOP( count );
		do_delete( new( mem_mgr( MemMgr::instance() ) ) int,
			mem_mgr( MemMgr::instance() ) );
		TEST_END_LOOP( std::wcout );
		MemMgr::destruct();
		return TEST_ELAPCED_MCS;
	}
	
	long double test_std_new_delete_int( const int count )
	{
		TEST_START_LOOP( count );
		delete new int;
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	const char* new_class_category = "new class (std/managed/new.h)";
	const char* new_delete_class_category = "new/delete class (std/managed/new.h)";
	const char* new_delete_int_category = "new/delete int (std/new.h)";

	void run_all_tests( const int op_repeat, const int test_repeat )
	{		
		/*
		long double test_new_mem_mgr( const int count )
	
		long double test_new_singleton_mem_mgr( const int count )
		
		long double test_new_managed_singleton_mem_mgr( const int count )
		
		long double test_std_new_class( const int count )
		*/
		
		//////////////////////////////////////////////////////////////////////////
		// new class
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_class_category, "new class heap sz pt mgr",
 			test_new_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
 		run_perf_test( new_class_category, "new class shared sz pt mgr",
 			test_new_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );
 
		run_perf_test( new_class_category, "new class sing heap sz pt mgr",
			test_new_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_class_category, "new class sing shared sz pt mgr",
			test_new_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

 		run_perf_test( new_class_category, "new managed class sing heap sz pt mgr",
 			test_new_managed_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
 		run_perf_test( new_class_category, "new managed class sing shared sz pt mgr",
 			test_new_managed_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );
 
		run_perf_test( new_class_category, "std new class",
			test_std_new_class, op_repeat, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		// new/delete class
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_delete_class_category, "new/delete class heap sz pt mgr",
			test_new_delete_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete class shared sz pt mgr",
			test_new_delete_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( new_delete_class_category, "new/delete class sing heap sz pt mgr",
			test_new_delete_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete class sing shared sz pt mgr",
			test_new_delete_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( new_delete_class_category, "new/delete managed class sing heap sz pt mgr",
			test_new_delete_managed_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete managed class sing shared sz pt mgr",
			test_new_delete_managed_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( new_delete_class_category, "std new/delete class",
			test_std_new_delete_class, op_repeat, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		// new/delete int
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_delete_int_category, "new/delete int heap sz pt mgr",
			test_new_delete_int_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_delete_int_category, "new/delete int shared sz pt mgr",
			test_new_delete_int_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( new_delete_int_category, "new/delete int sing heap sz pt mgr",
			test_new_delete_int_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( new_delete_int_category, "new/delete int sing shared sz pt mgr",
			test_new_delete_int_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( new_delete_int_category, "std new/delete int",
			test_std_new_delete_int, op_repeat, test_repeat );

	}
}
bool test_managed_base( const int op_repeat, const int test_repeat )
{	
	print_perf_test_header<memmgr_type>( L"Testing managed base, operator new",
		op_repeat, test_repeat );
	run_all_tests( op_repeat, test_repeat );

	return true;
}