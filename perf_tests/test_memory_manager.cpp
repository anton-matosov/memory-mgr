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
#include "managed_base.h"
#include "singleton_manager.h"
#include "detail/test.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4
;
static const size_t memory_size = 32 * 1024 * 1024;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

typedef memmgr_type::offset_type offset_type;

typedef memory_mgr::heap_segment< memmgr_type > heap_mgr;

typedef memory_mgr::shared_segment< memmgr_type > shared_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memmgr_type
	>
> heap_sz_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::heap_segment
<
	memory_mgr::pointer_convert
	< 
		memmgr_type
	>
> heap_pt_mgr;

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memmgr_type
			> 
		>
	>
> sing_heap_sz_pt_mgr;

//////////////////////////////////////////////////////////////////////////
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
long double test_alloc_mem_mgr( const int count )
{
	MemMgr mgr;

	TEST_START_LOOP( count );
	mgr.allocate( chunk_size );
	TEST_END_LOOP( std::wcout );
	
	return TEST_ELAPCED_MCS;
}

template<class MemMgr>
long double test_alloc_singleton_mem_mgr( const int count )
{
	MemMgr::instance();
	TEST_START_LOOP( count );
	MemMgr::instance().allocate( chunk_size );
	TEST_END_LOOP( std::wcout );
	MemMgr::destruct();
	return TEST_ELAPCED_MCS;
}


long double test_malloc( const int count )
{
	
	TEST_START_LOOP( count );
	malloc(chunk_size);
	TEST_END_LOOP( std::wcout );
	
	return TEST_ELAPCED_MCS;
}

long double test_std_new_int( const int count )
{

	TEST_START_LOOP( count );
	new int;
	TEST_END_LOOP( std::wcout );

	return TEST_ELAPCED_MCS;
}


template<class MemMgr>
long double test_alloc_dealloc_mem_mgr( const int count )
{
	MemMgr mgr;

	TEST_START_LOOP( count );
	mgr.deallocate( mgr.allocate( chunk_size ), chunk_size );
	TEST_END_LOOP( std::wcout );

	return TEST_ELAPCED_MCS;
}

template<class MemMgr>
long double test_alloc_dealloc_sz_mem_mgr( const int count )
{
	MemMgr mgr;

	TEST_START_LOOP( count );
	mgr.deallocate( mgr.allocate( chunk_size ) );
	TEST_END_LOOP( std::wcout );

	return TEST_ELAPCED_MCS;
}

template<class MemMgr>
long double test_alloc_dealloc_singleton_mem_mgr( const int count )
{
	MemMgr::instance();
	TEST_START_LOOP( count );
	MemMgr::instance().deallocate( MemMgr::instance().allocate( chunk_size ), chunk_size );
	TEST_END_LOOP( std::wcout );
	MemMgr::destruct();
	return TEST_ELAPCED_MCS;
}

template<class MemMgr>
long double test_alloc_dealloc_singleton_sz_mem_mgr( const int count )
{
	MemMgr::instance();
	TEST_START_LOOP( count );
	MemMgr::instance().deallocate( MemMgr::instance().allocate( chunk_size ) );
	TEST_END_LOOP( std::wcout );
	MemMgr::destruct();
	return TEST_ELAPCED_MCS;
}

long double test_malloc_free( const int count )
{

	TEST_START_LOOP( count );
	free( malloc(chunk_size) );
	TEST_END_LOOP( std::wcout );

	return TEST_ELAPCED_MCS;
}

long double test_std_new_delete_int( const int count )
{

	TEST_START_LOOP( count );
	delete new int;
	TEST_END_LOOP( std::wcout );

	return TEST_ELAPCED_MCS;
}

void run_all_tests( const int op_repeat, const int test_repeat )
{
	run_perf_test( "alloc heap mgr", test_alloc_mem_mgr<heap_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc shared mgr", test_alloc_mem_mgr<shared_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc heap sz mgr", test_alloc_mem_mgr<heap_sz_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc heap pt mgr", test_alloc_mem_mgr<heap_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc heap sz pt mgr", test_alloc_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc shared sz pt mgr", test_alloc_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc sing heap sz pt mgr", test_alloc_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc sing shared sz pt mgr", test_alloc_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc dealloc heap mgr", test_alloc_dealloc_mem_mgr<heap_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc dealloc shared mgr", test_alloc_dealloc_mem_mgr<shared_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc dealloc heap sz mgr", test_alloc_dealloc_sz_mem_mgr<heap_sz_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc dealloc heap pt mgr", test_alloc_dealloc_mem_mgr<heap_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc dealloc heap sz pt mgr", test_alloc_dealloc_sz_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc dealloc shared sz pt mgr", test_alloc_dealloc_sz_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "alloc dealloc sing heap sz pt mgr", test_alloc_dealloc_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
	run_perf_test( "alloc dealloc sing shared sz pt mgr", test_alloc_dealloc_singleton_sz_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

	run_perf_test( "std new delete int", test_std_new_delete_int, op_repeat, test_repeat );
	run_perf_test( "malloc - free", test_malloc_free, op_repeat, test_repeat );

	run_perf_test( "std new int", test_std_new_int, op_repeat, test_repeat );
	run_perf_test( "malloc", test_malloc, op_repeat, test_repeat );
}

bool test_memory_manager()
{	
	const int op_repeat = 50000;
	const int test_repeat = 10;

	memory_mgr::perf_timer timer;

	std::wcout << L"Items count: " << op_repeat << L"\n";
	std::wcout << L"Memory size: " << memory_mgr::manager_traits<memmgr_type>::memory_size << L"\n";
	std::wcout << L"Required memory: " << op_repeat * chunk_size << L"\n";
	std::wcout << L"Timer resolution: " << std::fixed << timer.resolution_mcs() << L" mcs\n";
	
	run_all_tests( op_repeat, test_repeat );
	
 	return false;
}

