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

#include "stdafx.h"
#include <memory-mgr/pool/pool.hpp>

namespace
{
	long double test_pool_alloc( const int op_repeat, const int per_alloc )
	{
		memory_mgr::pool<> p( 4 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = p.allocate();
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			p.deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_pool_dealloc( const int op_repeat, const int per_alloc )
	{
		memory_mgr::pool<> p( 4 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = p.allocate();
		}
		TEST_SPLIT_LOOP_RESTART_TIMER;
		{
			p.deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_pool_ordered_dealloc( const int op_repeat, const int per_alloc )
	{
		memory_mgr::pool<> p( 4 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = p.allocate();
		}
		TEST_SPLIT_LOOP_RESTART_TIMER;
		{
			p.ordered_deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_pool_alloc_ordered_dealloc( const int op_repeat, const int per_alloc )
	{
// 		typedef memory_mgr::fast_pool_allocator<larger_structure<N>,
// 			memory_mgr::default_user_allocator_new_delete,
// 			memory_mgr::details::pool::null_mutex> alloc;
// 		typedef memory_mgr::fast_pool_allocator<larger_structure<N> > alloc_sync;

		memory_mgr::pool<> p( 4 );
		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = p.allocate();
		}
		TEST_SPLIT_LOOP;
		{
			p.ordered_deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_pool_alloc_dealloc( const int op_repeat, const int per_alloc )
	{
		memory_mgr::pool<> p( 4 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = p.allocate();
		}
		TEST_SPLIT_LOOP;
		{
			p.deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	const char* pool_tests_category = "pool";

#define RUN_PERF_TEST( _testCategoryName, testMethod )\
	run_perf_test( pool_tests_category, #testMethod,\
		testMethod, op_repeat, per_alloc, test_repeat );

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		RUN_PERF_TEST( pool_tests_category, test_pool_alloc );
		RUN_PERF_TEST( pool_tests_category, test_pool_dealloc );
		//RUN_PERF_TEST( pool_tests_category, test_pool_ordered_dealloc );
		RUN_PERF_TEST( pool_tests_category, test_pool_alloc_dealloc );
	}

}


bool test_pool( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<sing_heap_sz_mgr>( L"Testing pool",
		0, 0, 0 );

	run_all_tests( op_repeat, per_alloc, test_repeat );


	return true;
}