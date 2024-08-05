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


#include "detail/test_mt.h"
#include "memory-mgr/offset_ptr.h"

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
#	include <boost/interprocess/managed_shared_memory.hpp>
#	include <boost/interprocess/shared_memory_object.hpp>
#	include <boost/interprocess/segment_manager.hpp>
#	include <boost/interprocess/offset_ptr.hpp>
#	include <boost/shared_ptr.hpp>
namespace bi = boost::interprocess;

bi::managed_shared_memory* managed_shm;
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

namespace
{
	typedef sing_heap_sz_mgr mem_mgr_t;

	enum{ num_test_threads = 25, start_delay = 0 };

	long double test_malloc_free( const int op_repeat, const int per_alloc )
	{
		typedef int* pointer_type;

		TEST_START_LOOP_MT( op_repeat, per_alloc, pointer_type );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( malloc(chunk_size) );
		}
		TEST_SPLIT_LOOP;
		{
			free( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP_MT;

		TEST_START_MT_TESTS( num_test_threads, start_delay );

		return TEST_ELAPCED_MCS_MT;
	}

	//////////////////////////////////////////////////////////////////////////
	// alloc/dealloc managers
	//////////////////////////////////////////////////////////////////////////
	template<class MemMgr, class PointerType >
	long double test_alloc_dealloc_mem_mgr( const int op_repeat, const int per_alloc )
	{
		MemMgr mem_mgr;

		typedef PointerType pointer_type;
		TEST_PRINT_MEM_INFO( op_repeat, per_alloc, MemMgr, pointer_type );

		TEST_START_LOOP_MT_2( op_repeat, per_alloc, pointer_type, MemMgr& mgr );
		{
			TEST_TRACK_PTR = static_cast<pointer_type>( mgr.allocate( chunk_size ) );
		}
		TEST_SPLIT_LOOP;
		{
			mgr.deallocate( TEST_GET_TRACKED_PTR, chunk_size );
		}
		TEST_END_LOOP_MT;

		TEST_START_MT_TESTS_2( num_test_threads, start_delay, mem_mgr );

		return TEST_ELAPCED_MCS_MT;
	}

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
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
		managed_shm = new bi::managed_shared_memory ( bi::create_only,"MySharedMemory", per_alloc * chunk_size * 8 * num_test_threads );

		TEST_START_LOOP_MT( op_repeat, per_alloc, void* );
		{
			TEST_TRACK_PTR = managed_shm->allocate( chunk_size );
		}
		TEST_SPLIT_LOOP;
		{
			managed_shm->deallocate( TEST_GET_TRACKED_PTR );
		}
		TEST_END_LOOP_MT( std::wcout );

		TEST_START_MT_TESTS( num_test_threads, start_delay );

		delete managed_shm;
		return TEST_ELAPCED_MCS_MT;
	}
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

	const char* tests_category = "multithreaded alloc/dealloc";

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		run_perf_test( tests_category, "test malloc/free",
			test_malloc_free, op_repeat, per_alloc, test_repeat );

		run_perf_test( tests_category, "test alloc/dealloc shared_sz_mgr",
			test_alloc_dealloc_mem_mgr<shared_sz_mgr, void*>, op_repeat, per_alloc, test_repeat );

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
		run_perf_test( tests_category, "test BOOST alloc/dealloc",
			test_boost_interprocess, op_repeat, per_alloc, test_repeat );
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS
	}

}


bool test_multithreaded_alloc_dealloc( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<sing_heap_sz_mgr>( L"Testing multithreaded alloc/dealloc",
		op_repeat, per_alloc, test_repeat );

	run_all_tests( op_repeat, per_alloc, test_repeat );


	return true;
}
