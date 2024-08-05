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


#include "memory-mgr/offset_ptr.h"

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
#	include <boost/interprocess/managed_shared_memory.hpp>
#	include <boost/interprocess/shared_memory_object.hpp>
#	include <boost/interprocess/segment_manager.hpp>
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

#ifdef STD_NEW
#	define GC_ALLOC_DECLARED
#	pragma comment( lib, "boost-memory" )
#endif

namespace
{
	//////////////////////////////////////////////////////////////////////////
	// alloc managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr>
	long double test_alloc_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mgr;
		
		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = mgr.allocate( chunk_size );
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			mgr.deallocate( TEST_GET_TRACKED_PTR, chunk_size );
			break;
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr, class PointerType >
	long double test_dealloc_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mgr;

		typedef PointerType pointer_type;
		//TEST_PRINT_MEM_INFO( op_repeat, per_alloc, MemMgr, int );

		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( mgr.allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP_RESTART_TIMER;
		{
			mgr.deallocate( TEST_GET_TRACKED_PTR, chunk_size );
		}
		TEST_END_LOOP( std::wcout );


		return TEST_ELAPCED_MCS;
	}

	template<class MemMgr>
	long double test_alloc_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr::instance();
		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			MemMgr::instance().allocate( chunk_size );
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

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS

	namespace bi = boost::interprocess;


	//////////////////////////////////////////////////////////////////////////
	// boost interprocess library
	//////////////////////////////////////////////////////////////////////////
	long double test_boost_interprocess( const int op_repeat, const int per_alloc )
	{
		enum
		{
			chunk_size = 4
		};

		//Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove() { bi::shared_memory_object::remove("MySharedMemory"); }
			~shm_remove(){ bi::shared_memory_object::remove("MySharedMemory"); }
		} remover;

		//Managed memory segment that allocates portions of a shared memory
		//segment with the default management algorithm
		bi::managed_shared_memory managed_shm( bi::create_only,"MySharedMemory", per_alloc * chunk_size * 8 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = managed_shm.allocate( chunk_size );
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			managed_shm.deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_boost_interprocess_dealloc( const int op_repeat, const int per_alloc )
	{
		enum
		{
			chunk_size = 4
		};

		//Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove() { bi::shared_memory_object::remove("MySharedMemory"); }
			~shm_remove(){ bi::shared_memory_object::remove("MySharedMemory"); }
		} remover;

		//Managed memory segment that allocates portions of a shared memory
		//segment with the default management algorithm
		bi::managed_shared_memory managed_shm( bi::create_only,"MySharedMemory", per_alloc * chunk_size * 8 );

		TEST_START_LOOP( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = managed_shm.allocate( chunk_size );
		}
		TEST_SPLIT_LOOP;
		{
			managed_shm.deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

	//////////////////////////////////////////////////////////////////////////
	// alloc/dealloc managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr, class PointerType >
	long double test_alloc_dealloc_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mgr;

		typedef PointerType pointer_type;
		//TEST_PRINT_MEM_INFO( op_repeat, per_alloc, MemMgr, int );

		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( mgr.allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP;
		{
			mgr.deallocate( TEST_GET_TRACKED_PTR, chunk_size );
		}
		TEST_END_LOOP( std::wcout );

		
		return TEST_ELAPCED_MCS;
	}

#ifdef GC_ALLOC_DECLARED
	template< class PointerType >
	long double test_alloc_dealloc_gc_alloc( const int op_repeat, const int per_alloc )
	{
		
		//std::BlockPool m_recycle;

		typedef PointerType pointer_type;
		memory_mgr::perf_timer timer__;
		int loop__ = per_alloc;
		int repeate_count__ = per_alloc;
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
#endif //GC_ALLOC_DECLARED
	template<class MemMgr>
	long double test_alloc_dealloc_singleton_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr::instance();
		
		typedef int* pointer_type;
		
		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( MemMgr::instance().allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP;
		{
			MemMgr::instance().deallocate( TEST_GET_TRACKED_PTR, chunk_size );
		}
		TEST_END_LOOP( std::wcout );

		MemMgr::destruct();

		

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// malloc
	//////////////////////////////////////////////////////////////////////////
	long double test_malloc( const int op_repeat, const int per_alloc )
	{		
		TEST_START_LOOP( op_repeat, per_alloc, int* );
		{
			TEST_TRACK_PTR = (int*)malloc(chunk_size);
		}
		TEST_SPLIT_LOOP_STOP_TIMER;
		{
			free( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		

		return TEST_ELAPCED_MCS;
	}

	long double test_malloc_free( const int op_repeat, const int per_alloc )
	{
		typedef int* pointer_type;
		
		
		TEST_START_LOOP( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( malloc(chunk_size) );
		}
		TEST_SPLIT_LOOP;
		{
			free( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP( std::wcout );

		

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////

	const char* simple_alloc_category = "allocate chunk";
	const char* alloc_dealloc_category = "allocate/deallocate chunk";

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		typedef void* pointer_type;

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
		//////////////////////////////////////////////////////////////////////////
		// Boost Interprocess
		run_perf_test( simple_alloc_category, "boost interprocess alloc",
			test_boost_interprocess, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "boost interprocess alloc/dealloc",
			test_boost_interprocess_dealloc, op_repeat, per_alloc, test_repeat );
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( simple_alloc_category, "alloc heap mgr",
			test_alloc_mem_mgr<heap_mgr>, op_repeat, per_alloc, test_repeat );
 		run_perf_test( simple_alloc_category, "DEalloc heap mgr",
 			test_dealloc_mem_mgr<heap_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "alloc shared mgr",
			test_alloc_mem_mgr<shared_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( simple_alloc_category, "alloc seg heap mgr",
// 			test_alloc_mem_mgr<seg_heap_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( simple_alloc_category, "alloc seg shared mgr",
// 			test_alloc_mem_mgr<seg_shared_mgr>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( simple_alloc_category, "alloc heap sz mgr",
			test_alloc_mem_mgr<heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "alloc shared sz mgr",
			test_alloc_mem_mgr<shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "alloc heap mgr",
			test_alloc_mem_mgr<heap_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "alloc LFM heap sz mgr",
			test_alloc_mem_mgr<sz_lfm_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( simple_alloc_category, "alloc heap seg sz mgr",
// 			test_alloc_mem_mgr<seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( simple_alloc_category, "alloc heap seg mgr",
// 			test_alloc_mem_mgr<seg_heap_mgr>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( simple_alloc_category, "alloc sing heap sz mgr",
			test_alloc_singleton_mem_mgr<sing_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "alloc sing shared sz mgr",
			test_alloc_singleton_mem_mgr<sing_shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( simple_alloc_category, "alloc sing heap seg sz mgr",
// 			test_alloc_singleton_mem_mgr<sing_seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap mgr",
			test_alloc_dealloc_mem_mgr<heap_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc shared mgr",
			test_alloc_dealloc_mem_mgr<shared_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap seg mgr",
// 			test_alloc_dealloc_mem_mgr<seg_heap_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap sz mgr",
			test_alloc_dealloc_mem_mgr<heap_sz_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap mgr",
			test_alloc_dealloc_mem_mgr<heap_mgr, int*>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc LFM heap sz mgr",
			test_alloc_dealloc_mem_mgr<sz_lfm_heap_sz_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap seg sz mgr",
// 			test_alloc_dealloc_mem_mgr<seg_heap_sz_mgr, pointer_type>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap sz mgr",
			test_alloc_dealloc_mem_mgr<heap_sz_mgr, int*>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc shared sz mgr",
			test_alloc_dealloc_mem_mgr<shared_sz_mgr, int*>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc LFM heap sz mgr",
			test_alloc_dealloc_mem_mgr<sz_lfm_heap_sz_mgr, int*>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( alloc_dealloc_category, "alloc/dealloc heap seg sz mgr",
// 			test_alloc_dealloc_mem_mgr<seg_heap_sz_mgr, int*>, op_repeat, per_alloc, test_repeat );

		//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "alloc/dealloc sing heap sz mgr",
			test_alloc_dealloc_singleton_mem_mgr<sing_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "alloc/dealloc sing shared sz mgr",
			test_alloc_dealloc_singleton_mem_mgr<sing_shared_sz_mgr>, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( alloc_dealloc_category, "alloc/dealloc sing heap seg sz mgr",
// 			test_alloc_dealloc_singleton_mem_mgr<sing_seg_heap_sz_mgr>, op_repeat, per_alloc, test_repeat );
//////////////////////////////////////////////////////////////////////////
//		run_perf_test( alloc_dealloc_category, "GC Allocator",
//			test_alloc_dealloc_gc_alloc<int>, op_repeat, per_alloc, test_repeat );
//////////////////////////////////////////////////////////////////////////
		run_perf_test( alloc_dealloc_category, "std new/delete int",
			test_std_new_delete<int>, op_repeat, per_alloc, test_repeat );
		run_perf_test( alloc_dealloc_category, "malloc/free",
			test_malloc_free, op_repeat, per_alloc, test_repeat );

		run_perf_test( simple_alloc_category, "std new int",
			test_std_new<int>, op_repeat, per_alloc, test_repeat );
		run_perf_test( simple_alloc_category, "malloc",
			test_malloc, op_repeat, per_alloc, test_repeat );


	}

}

bool test_memory_manager( const int op_repeat, const int per_alloc, const int test_repeat )
{	

	print_perf_test_header<memmgr_type>( L"Testing memory managers",
		op_repeat, per_alloc, test_repeat );
	run_all_tests( op_repeat, per_alloc, test_repeat );


	return false;
}

