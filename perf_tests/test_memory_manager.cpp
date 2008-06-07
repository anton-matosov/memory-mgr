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

#include <iostream>
#include "detail/test.h"
#include "managers.h"
#include "offset_pointer.h"
#include "test_templates.h"
//#include <stdext/Memory.h>

#ifndef STD_NEW
#	define STD_NEW( x, y ) 0
#else
#pragma comment( lib, "boost-memory" )
#endif

namespace
{
	//////////////////////////////////////////////////////////////////////////
	// alloc managers
	//////////////////////////////////////////////////////////////////////////
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
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// alloc/dealloc managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr, class PointerType >
	long double test_alloc_dealloc_mem_mgr( const int count )
	{
		MemMgr mgr;

		typedef PointerType pointer_type;
		pointer_type *p = new pointer_type[count];
		TEST_START_LOOP( count );
		{
			p[TEST_ITERATION_NUMBER] = static_cast<pointer_type>( mgr.allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP;
		{
			mgr.deallocate( p[TEST_ITERATION_NUMBER], chunk_size );
		}
		TEST_END_LOOP( std::wcout );

		delete[] p;
		return TEST_ELAPCED_MCS;
	}

	template< class PointerType >
	long double test_alloc_dealloc_gc_alloc( const int count )
	{
		
		//std::BlockPool m_recycle;

		typedef PointerType pointer_type;
		memory_mgr::perf_timer timer__;
		int loop__ = count;
		int repeate_count__ = count;
		timer__.start();
		{
			std::BlockPool m_recycle;
			std::ScopeAlloc alloc(m_recycle);
			while( --loop__ ){
			{
				pointer_type* p = STD_NEW( alloc, pointer_type );
			}
		}		
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_alloc_dealloc_singleton_mem_mgr( const int count )
	{
		MemMgr::instance();
		
		typedef int* pointer_type;
		pointer_type *p = new pointer_type[count];
		TEST_START_LOOP( count );
		{
			p[TEST_ITERATION_NUMBER] = static_cast<pointer_type>( MemMgr::instance().allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP;
		{
			MemMgr::instance().deallocate( p[TEST_ITERATION_NUMBER], chunk_size );
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		delete[] p;

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// malloc
	//////////////////////////////////////////////////////////////////////////
	long double test_malloc( const int count )
	{
		int **p;
		p = new int*[count];
		TEST_START_LOOP( count );
		{
			p[TEST_ITERATION_NUMBER] = (int*)malloc(chunk_size);
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			free( p[TEST_ITERATION_NUMBER] );
		}
		TEST_END_LOOP( std::wcout );

		delete[] p;

		return TEST_ELAPCED_MCS;
	}

	long double test_malloc_free( const int count )
	{
		typedef int* pointer_type;
		pointer_type *p;
		p = new pointer_type[count];
		TEST_START_LOOP( count );
		{
			p[TEST_ITERATION_NUMBER] = static_cast<pointer_type>( malloc(chunk_size) );
		}
		TEST_SPLIT_LOOP;
		{
			free( p[TEST_ITERATION_NUMBER] );
		}
		TEST_END_LOOP( std::wcout );

		delete[] p;

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////

	const char* simple_alloc_category = "allocate chunk";
	const char* alloc_dealloc_category = "allocate/deallocate chunk";

	void run_all_tests( const int op_repeat, const int test_repeat )
	{
		typedef memory_mgr::manager_traits<shared_mgr>::offset_type offset_type;

		

		run_perf_test( simple_alloc_category, "alloc heap mgr",
			test_alloc_mem_mgr<heap_mgr>, op_repeat, test_repeat );
		run_perf_test( simple_alloc_category, "alloc shared mgr",
			test_alloc_mem_mgr<shared_mgr>, op_repeat, test_repeat );

		run_perf_test( simple_alloc_category, "alloc heap sz mgr",
			test_alloc_mem_mgr<heap_sz_mgr>, op_repeat, test_repeat );
		run_perf_test( simple_alloc_category, "alloc heap pt mgr",
			test_alloc_mem_mgr<heap_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( simple_alloc_category, "alloc heap sz pt mgr",
			test_alloc_mem_mgr<heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( simple_alloc_category, "alloc shared sz pt mgr",
			test_alloc_mem_mgr<shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( simple_alloc_category, "alloc sing heap sz pt mgr",
			test_alloc_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( simple_alloc_category, "alloc sing shared sz pt mgr",
			test_alloc_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );

		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap mgr",
			test_alloc_dealloc_mem_mgr<heap_mgr, offset_type>, op_repeat, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc shared mgr",
			test_alloc_dealloc_mem_mgr<shared_mgr, offset_type>, op_repeat, test_repeat );

		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap sz mgr",
			test_alloc_dealloc_mem_mgr<heap_sz_mgr, offset_type>, op_repeat, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap pt mgr",
			test_alloc_dealloc_mem_mgr<heap_pt_mgr, int*>, op_repeat, test_repeat );

		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap sz pt mgr",
			test_alloc_dealloc_mem_mgr<heap_sz_pt_mgr, int*>, op_repeat, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc shared sz pt mgr",
			test_alloc_dealloc_mem_mgr<shared_sz_pt_mgr, int*>, op_repeat, test_repeat );

		run_perf_test( alloc_dealloc_category, "alloc/dealloc sing heap sz pt mgr",
			test_alloc_dealloc_singleton_mem_mgr<sing_heap_sz_pt_mgr>, op_repeat, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc sing shared sz pt mgr",
			test_alloc_dealloc_singleton_mem_mgr<sing_shared_sz_pt_mgr>, op_repeat, test_repeat );
//////////////////////////////////////////////////////////////////////////
//		run_perf_test( alloc_dealloc_category, "GC Allocator",
//			test_alloc_dealloc_gc_alloc<int>, op_repeat, test_repeat );
//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "std new/delete int",
			test_std_new_delete<int>, op_repeat, test_repeat );
		run_perf_test( alloc_dealloc_category, "malloc/free",
			test_malloc_free, op_repeat, test_repeat );

		run_perf_test( simple_alloc_category, "std new int",
			test_std_new<int>, op_repeat, test_repeat );
		run_perf_test( simple_alloc_category, "malloc",
			test_malloc, op_repeat, test_repeat );
	}

}

bool test_memory_manager( const int op_repeat, const int test_repeat )
{	

	print_perf_test_header<memmgr_type>( L"Testing memory managers",
		op_repeat, test_repeat );
	run_all_tests( op_repeat, test_repeat );


	return false;
}

