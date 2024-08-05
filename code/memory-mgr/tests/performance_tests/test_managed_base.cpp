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


#include "memory-mgr/new.h"

namespace
{
	template<class MemMgr>
	class test_class: public memory_mgr::managed_base< MemMgr >
	{
		int m_i;
	};

	class test_class2
	{
		int m_i;
	};

	//////////////////////////////////////////////////////////////////////////
	// new class, managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr>
	long double test_new_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mgr;

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			memory_mgr::new_<test_class2>( mgr )();
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			mgr.clear();
			break;
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{		
		MemMgr::instance();

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			memory_mgr::new_<test_class2, MemMgr>()();
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			MemMgr::instance().clear();
			break;
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_new_managed_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{
		typedef test_class<MemMgr> test_class;

		//TEST_PRINT_MEM_INFO( op_repeat, per_alloc, MemMgr, test_class);
		MemMgr::instance();
		
		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			new test_class;
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			MemMgr::instance().clear();
			break;
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	// new/delete class, managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr, class TestType>
	long double test_new_delete_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mgr;

		TEST_PRINT_MEM_INFO( op_repeat, per_alloc, MemMgr, TestType );
		typedef TestType test_type;
		typedef test_type* pointer_type;

		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = memory_mgr::new_<test_type>(mgr)();
		}
		TEST_SPLIT_LOOP;
		{
			delete_( mgr, TEST_GET_TRACKED_PTR );;
		}
		TEST_END_LOOP( std::wcout );

		

		return TEST_ELAPCED_MCS;
	}
	
	template<class MemMgr>
	long double test_new_delete_managed_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{
		typedef test_class<MemMgr> test_class;

		MemMgr::instance();

		typedef test_class* pointer_type;
		
		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = new test_class;
		}
		TEST_SPLIT_LOOP;
		{
			delete TEST_GET_TRACKED_PTR;
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	// new/delete singleton maanger
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr, class TestType>
	long double test_new_delete_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{		
		MemMgr::instance();

		typedef TestType test_type;
		typedef test_type* pointer_type;
		
		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = memory_mgr::new_<test_type, MemMgr>()();
		}
		TEST_SPLIT_LOOP;
		{
			delete_<MemMgr>( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		

		return TEST_ELAPCED_MCS;
	}
	
	const char* new_class_category = "new class (std/managed/new.h)";
	const char* new_delete_class_category = "new/delete class (std/managed/new.h)";
	const char* new_delete_int_category = "new/delete int (std/new.h)";

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{		
		/*
		long double test_new_mem_mgr( const int op_repeat, per_alloc, const int per_alloc )
	
		long double test_new_singleton_mem_mgr( const int op_repeat, per_alloc, const int per_alloc )
		
		long double test_new_managed_singleton_mem_mgr( const int op_repeat, per_alloc, const int per_alloc )
		
		long double test_std_new_class( const int op_repeat, per_alloc, const int per_alloc )
		*/

		typedef test_class2* test_class_type;
		
		//////////////////////////////////////////////////////////////////////////
		// new class
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_class_category, "new class heap sz mgr",
 			test_new_mem_mgr<heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
 		run_perf_test( new_class_category, "new class shared sz mgr",
			test_new_mem_mgr<shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_class_category, "new class shared seg sz mgr",
// 			test_new_mem_mgr<seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
 
		run_perf_test( new_class_category, "new class sing heap sz mgr",
			test_new_singleton_mem_mgr<sing_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_class_category, "new class sing shared sz mgr",
			test_new_singleton_mem_mgr<sing_shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_class_category, "new class sing heap seg sz mgr",
// 			test_new_singleton_mem_mgr<sing_seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );

 		run_perf_test( new_class_category, "new managed class sing heap sz mgr",
 			test_new_managed_singleton_mem_mgr<sing_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
 		run_perf_test( new_class_category, "new managed class sing shared sz mgr",
			test_new_managed_singleton_mem_mgr<sing_shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_class_category, "new managed class sing seg heap sz mgr",
// 			test_new_managed_singleton_mem_mgr<sing_seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
 
		run_perf_test( new_class_category, "std new class",
			test_std_new<test_class_type>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		// new/delete class
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_delete_class_category, "new/delete class heap sz mgr",
			test_new_delete_mem_mgr<heap_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete class shared sz mgr",
			test_new_delete_mem_mgr<shared_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_delete_class_category, "new/delete class heap seg sz mgr",
// 			test_new_delete_mem_mgr<seg_heap_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );

		run_perf_test( new_delete_class_category, "new/delete class sing heap sz mgr",
			test_new_delete_singleton_mem_mgr<sing_heap_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete class sing shared sz mgr",
			test_new_delete_singleton_mem_mgr<sing_shared_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_delete_class_category, "new/delete class sing heap seg sz mgr",
// 			test_new_delete_singleton_mem_mgr<sing_seg_heap_sz_mgr, test_class_type>, op_repeat, per_alloc, test_repeat );

		run_perf_test( new_delete_class_category, "new/delete managed class sing heap sz mgr",
			test_new_delete_managed_singleton_mem_mgr<sing_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_delete_class_category, "new/delete managed class sing shared sz mgr",
			test_new_delete_managed_singleton_mem_mgr<sing_shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( new_delete_class_category, "new/delete managed class sing seg heap sz mgr",
// 			test_new_delete_managed_singleton_mem_mgr<sing_seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );

		run_perf_test( new_delete_class_category, "std new/delete class",
			test_std_new_delete<test_class_type>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		// new/delete int
		//////////////////////////////////////////////////////////////////////////
		run_perf_test( new_delete_int_category, "new/delete int heap sz mgr",
			test_new_delete_mem_mgr<heap_sz_mgr, int>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_delete_int_category, "new/delete int shared sz mgr",
			test_new_delete_mem_mgr<shared_sz_mgr, int>, op_repeat, per_alloc, test_repeat );

		run_perf_test( new_delete_int_category, "new/delete int sing heap sz mgr",
			test_new_delete_singleton_mem_mgr<sing_heap_sz_mgr, int>, op_repeat, per_alloc, test_repeat );
		run_perf_test( new_delete_int_category, "new/delete int sing shared sz mgr",
			test_new_delete_singleton_mem_mgr<sing_shared_sz_mgr, int>, op_repeat, per_alloc, test_repeat );

		run_perf_test( new_delete_int_category, "std new/delete int",
			test_std_new_delete<int>, op_repeat, per_alloc, test_repeat );

	}
}
bool test_managed_base( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<memmgr_type>( L"Testing managed base, operator new",
		op_repeat, per_alloc, test_repeat );
	run_all_tests( op_repeat, per_alloc, test_repeat );

	return true;
}
