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
#include "perf_timer.h"
#include "memory_manager.h"
#include "heap_segment.h"
#include "shared_segment.h"
#include "new.h"
#include "size_tracking.h"
#include "detail/test.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4
;
static const size_t memory_size = 32 * 1024 * 1024;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

typedef memmgr_type::offset_type offset_type;

typedef memory_mgr::heap_segment< memmgr_type > heap_mgr;

typedef memory_mgr::shared_segment< memmgr_type > shared_mgr;

typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memmgr_type
	>
> heap_sz_mgr;

typedef memory_mgr::heap_segment
<
	memory_mgr::pointer_convert
	< 
		memmgr_type
	>
> heap_pt_mgr;

typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memory_mgr::pointer_convert
		< 
			memmgr_type
		> 
	>
> heap_sz_pt_mgr;

typedef memory_mgr::shared_segment
<
	memory_mgr::size_tracking
	< 
		memory_mgr::pointer_convert
		< 
			memmgr_type
		> 
	>
> shared_sz_pt_mgr;

#include "shared_segment.h"
#include "managed_base.h"
#include "singleton_manager.h"


typedef memory_mgr::singleton_manager
< 
	memory_mgr::shared_segment
	< 
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memmgr_type
			> 
		>
	>
> sing_shared_sz_pt_mgr;

class test_class: public memory_mgr::managed_base< sing_shared_sz_pt_mgr >
{
	int m_i;
};

class test_class2
{
	int m_i;
};


// MGR_DECLARE_TEST( mgr.allocate( chunk_size ), mgr_alloc_test );
// MGR_DECLARE_TEST( new( mem_mgr(mgr) ) int, mgr_new_int_test );
// MGR_DECLARE_TEST( malloc( chunk_size), heap_test );
// MGR_DECLARE_TEST( new int, heap_new_int_test );

template<class MemMgr>
long double test_mem_mgr( const int count )
{
	MemMgr mgr;

	TEST_START_LOOP( count );
	mgr.allocate( chunk_size );
	TEST_END_LOOP( std::wcout );
	
	return TEST_ELAPCED_MCS;
}

template<class MemMgr>
long double test_singleton_mem_mgr( const int count )
{
	MemMgr::instance();
	TEST_START_LOOP( count );
	MemMgr::instance().allocate();
	TEST_END_LOOP( std::wcout );
	MemMgr::destruct();
	return TEST_ELAPCED_MCS;
}


long double test_std_heap( const int count )
{
	
	TEST_START_LOOP( count );
	
	TEST_END_LOOP( std::wcout );
	
	return TEST_ELAPCED_MCS;
}

// #define MEMORY_MANAGER_PERF_TESTS( MgrType, op_retpeat, test_repeat)\
// {\
// 	MGR_RUN_TEST( mgr_alloc_test, MgrType, op_retpeat, test_repeat );\
// 	MGR_RUN_TEST( mgr_new_int_test, MgrType, op_retpeat, test_repeat );\
// 	MGR_RUN_TEST( heap_test, MgrType, op_retpeat, test_repeat );\
// 	MGR_RUN_TEST( heap_new_int_test, MgrType, op_retpeat, test_repeat );\
// }\

bool test_memory_manager()
{	
	const int op_repeat = 500000;
	const int test_repeat = 10;

	memory_mgr::perf_timer timer;

	std::wcout << L"Items count: " << op_repeat << L"\n";
	std::wcout << L"Memory size: " << memory_mgr::manager_traits<memmgr_type>::memory_size << L"\n";
	std::wcout << L"Required memory: " << op_repeat * chunk_size << L"\n";
	std::wcout << L"Timer resolution: " << std::fixed << timer.resolution_mcs() << L" mcs\n";
	
	run_perf_test( "heap sz pt mgr", test_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
	run_perf_test( "shared sz pt mgr", test_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

 	return false;
}

