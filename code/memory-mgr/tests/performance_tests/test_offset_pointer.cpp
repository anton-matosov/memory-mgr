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
#include "detail/test.h"
#include "perf_timer.h"

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
#	include <boost/interprocess/managed_shared_memory.hpp>
#	include <boost/interprocess/shared_memory_object.hpp>
#	include <boost/interprocess/segment_manager.hpp>
#	include <boost/interprocess/offset_ptr.hpp>
#	include <boost/shared_ptr.hpp>
namespace bi = boost::interprocess;

boost::shared_ptr<bi::managed_shared_memory> managed_shm;
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

namespace
{
	typedef sing_heap_sz_mgr mem_mgr_t;

	long double test_pointer( const int op_repeat, const int per_alloc )
	{
		int* ptr = new int();

		TEST_START_LOOP( op_repeat, per_alloc, int );
		{
			*ptr = 1;
		}
		TEST_END_LOOP( std::wcout );

		delete ptr;

		return TEST_ELAPCED_MCS;
	}

	long double test_offset_pointer( const int op_repeat, const int per_alloc )
	{
		memory_mgr::offset_ptr<int> ptr( memory_mgr::new_<int, mem_mgr_t>()() );

		TEST_START_LOOP( op_repeat, per_alloc, int );
		{
			*ptr = 1;
		}
		TEST_END_LOOP( std::wcout );

		delete_<mem_mgr_t>( ptr );

		return TEST_ELAPCED_MCS;
	}

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
	long double test_boost_offset_ptr( const int op_repeat, const int per_alloc )
	{
		bi::offset_ptr<int> ptr( (int*)managed_shm->allocate( chunk_size ) );
		TEST_START_LOOP( op_repeat, per_alloc, int );
		{
			*ptr = 1;
		}
		TEST_END_LOOP( std::wcout );

		managed_shm->deallocate( &*ptr );
		

		return TEST_ELAPCED_MCS;
	}
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

	const char* offset_tests_category = "offset pointers";

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		run_perf_test( offset_tests_category, "test C++ pointer",
			test_pointer, op_repeat, per_alloc, test_repeat );

		run_perf_test( offset_tests_category, "test offset_ptr",
			test_offset_pointer, op_repeat, per_alloc, test_repeat );

#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
		run_perf_test( offset_tests_category, "test BOOST offset_ptr",
			test_boost_offset_ptr, op_repeat, per_alloc, test_repeat );
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS
	}

}


bool test_offset_pointer( const int op_repeat, const int per_alloc, const int test_repeat )
{
	print_perf_test_header<sing_heap_sz_mgr>( L"Testing offset_ptr",
		0, 0, 0 );


#ifdef INCLUDE_BOOST_INTERPROCESS_TESTS
	//Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() { bi::shared_memory_object::remove("MySharedMemory"); }
		~shm_remove(){ bi::shared_memory_object::remove("MySharedMemory"); }
	} remover;

	//Managed memory segment that allocates portions of a shared memory
	//segment with the default management algorithm
	managed_shm.reset( new bi::managed_shared_memory( bi::create_only,"MySharedMemory", per_alloc * chunk_size * 8 ) );
#endif //INCLUDE_BOOST_INTERPROCESS_TESTS

	run_all_tests( op_repeat, per_alloc, test_repeat );


	return true;
}
