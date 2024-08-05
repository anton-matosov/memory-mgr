// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Uncomment this to stub out all MT locking


#define BOOST_NO_MT

#include "memory-mgr/pool/pool_alloc.hpp"
#include "memory-mgr/pool/object_pool.hpp"

#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <deque>
#include <set>

// use of test_main() eases automatic regression testing by eliminating
// the need for manual intervention on test failures (Beman Dawes)
#include <boost/test/test_tools.hpp>


// Each "tester" object below checks into and out of the "cdtor_checker",
//  which will report any problems related to the construction/destruction of
//  "tester" objects.
class construct_destruct_checker
{
private:
	// Each constructed object registers its "this" pointer into "objs"
	std::set<void *> objs;

public:
	~construct_destruct_checker()
	{
		// At end of program, print out memory leaks
		//  (assuming no static "tester"s)
		for (std::set<void *>::const_iterator i = objs.begin(); i != objs.end(); ++i)
		{
			BOOST_ERROR( "Memory leak: " << *i );
		}
	}

	void check_in(void * const This)
	{
		BOOST_REQUIRE_MESSAGE( objs.find(This) == objs.end(),
			"Double-constructed memory: " << This );

		objs.insert(This);
	}

	void check_out(void * const This)
	{
		BOOST_REQUIRE_MESSAGE( objs.find(This) != objs.end(),
			"Destroyed non-constructed memory: " << This );
		
		objs.erase(This);
	}

	// true iff all objects that have checked in have checked out
	bool ok() const { return objs.empty(); }
};
static construct_destruct_checker mem;

struct tester
{
	tester(int arg1, int arg2 = 0)
	{
		if (arg1 == 17 && arg2 == 17)
		{
			BOOST_MESSAGE( this << ": tester not constructed" );

			throw std::logic_error( "No construction allowed!" );
		}
		BOOST_MESSAGE( this << ": tester::tester()" );

		mem.check_in(this);
	}

	tester(const tester & other)
	{
		BOOST_MESSAGE( this << ": tester::tester(" << &other << ')' );
	
		mem.check_in(this);
	}

	~tester()
	{
		BOOST_MESSAGE( this << ": tester::~tester()" );

		mem.check_out(this);
	}
};

class test_pool_fixture
{
public:
	~test_pool_fixture()
	{
		BOOST_REQUIRE_MESSAGE( mem.ok(), "memory inconsistent" );
	}
};

BOOST_FIXTURE_TEST_SUITE( test_object_pool, test_pool_fixture )


	BOOST_AUTO_TEST_CASE( test_default_pool_constructor )
	{
		// should do nothing
		memory_mgr::object_pool<tester> pool;
	}

	BOOST_AUTO_TEST_CASE( test_garbage_collection_non_deleted )
	{
		// Construct several tester objects.  Don't delete them (i.e.,
		//  test pool's garbage collection).
		//  
		memory_mgr::object_pool<tester> pool;
		for (int i = 0; i < 10; ++i)
		{
			BOOST_REQUIRE( pool.construct(13) );
			BOOST_REQUIRE( pool.construct(13, 13) );
		}
	}

	BOOST_AUTO_TEST_CASE( test_garbage_collection_some_deleted )
	{
		// Construct several tester objects.  Delete some of them.

		memory_mgr::object_pool<tester> pool;
		std::vector<tester *> v;
		for (size_t i = 0; i < 10; ++i)
		{
			v.push_back(pool.construct(13, 13));
			BOOST_REQUIRE( v.back() );
		}

		std::random_shuffle(v.begin(), v.end());
		for (size_t j = 0; j < (v.size() / 2); ++j)
		{
			pool.destroy(v[j]);
		}

	}

	BOOST_AUTO_TEST_CASE( test_exception_safety_during_construction )
	{
		// Test how pool reacts with constructors that throw exceptions.
		//  Shouldn't have any memory leaks.
		memory_mgr::object_pool<tester> pool;
		for (int i = 0; i < 5; ++i)
		{
			BOOST_REQUIRE( pool.construct(13, 13) );
		}

		for (int j = 0; j < 5; ++j)
		{
			// The following constructor will raise an exception.
			BOOST_REQUIRE_THROW( pool.construct(17, 17) , std::logic_error );
		}
	}

BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE( test_pool_allocators, test_pool_fixture )

	BOOST_AUTO_TEST_CASE( test_allocator_with_vector )
	{
		// Allocate several tester objects.  Delete one.
		std::vector<tester, memory_mgr::pool_allocator<tester> > l;
		for (int i = 0; i < 10; ++i)
		{
			l.push_back( tester(13, 13) );
		}
		l.pop_back();
	}

	BOOST_AUTO_TEST_CASE( test_allocator_with_deque )
	{
		// Allocate several tester objects.  Delete two.
		std::deque<tester, memory_mgr::pool_allocator<tester> > l;
		for (int i = 0; i < 10; ++i)
		{
			l.push_back( tester(13, 13) );
		}
		l.pop_back();
		l.pop_front();
	}

	BOOST_AUTO_TEST_CASE( test_allocator_with_list )
	{
		// Allocate several tester objects.  Delete two.

		std::list<tester, memory_mgr::fast_pool_allocator<tester> > l;
		// lists rebind their allocators, so dumping is useless
		for (int i = 0; i < 10; ++i)
		{
			l.push_back( tester(13, 13) );
		}
		l.pop_back();
		l.pop_front();
	}

	BOOST_AUTO_TEST_CASE( test_allocator_cleanup )
	{
		tester * tmp = NULL;
		{
			// Create a memory leak on purpose.  (Allocator doesn't have
			//  garbage collection)
			
			memory_mgr::pool_allocator<tester> a;

			// (Note: memory leak)
			tmp = a.allocate(1, 0);
			BOOST_REQUIRE( tmp );

			new (tmp) tester(13, 13);
		}
		BOOST_CHECK_MESSAGE( ! mem.ok(), "Error: Pool allocator cleaned up!" );
		
		// Remove memory checker entry (to avoid error later) and
		//  clean up memory leak
		tmp->~tester();
		memory_mgr::pool_allocator<tester>::deallocate(tmp, 1);
	}

	BOOST_AUTO_TEST_CASE( test_allocating_zero_elements )
	{
		// test allocating zero elements
		memory_mgr::pool_allocator<tester> alloc;
		tester* ip = alloc.allocate(0);
		alloc.deallocate(ip, 0);
	}

	BOOST_AUTO_TEST_CASE( test_void_allocators_specifications )
	{

		typedef memory_mgr::pool_allocator<void> void_allocator;
		typedef memory_mgr::fast_pool_allocator<void> fast_void_allocator;

		typedef void_allocator::rebind<int>::other int_allocator;
		typedef fast_void_allocator::rebind<int>::other fast_int_allocator;

		std::vector<int, int_allocator> v1;
		std::vector<int, fast_int_allocator> v2;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE( test_pool_memory, test_pool_fixture )

// This is a wrapper around a UserAllocator.  It just registers alloc/dealloc
//  to/from the system memory.  It's used to make sure pool's are allocating
//  and deallocating system memory properly.
// Do NOT use this class with static or singleton pools.
template <typename UserAllocator>
struct TrackAlloc
{
	typedef typename UserAllocator::size_type size_type;
	typedef typename UserAllocator::difference_type difference_type;

	static std::set<char *> allocated_blocks;
	static UserAllocator m_alloc;

	static char* allocate (const size_type bytes)
	{
		char* const ret = m_alloc.allocate(bytes);
		BOOST_REQUIRE_MESSAGE( ret, "memory is not allocated. requested size: " << bytes );
		allocated_blocks.insert(ret);
		return ret;
	}

	static void deallocate(char * const block, size_type size)
	{
		BOOST_CHECK_MESSAGE( allocated_blocks.find(block) != allocated_blocks.end(),
			"Free'd non-allocate'ed block: " << (void *) block );

		allocated_blocks.erase(block);
		m_alloc.deallocate(block, size);
	}

	static bool ok()
	{
		return allocated_blocks.empty();
	}
};
template <typename UserAllocator>
std::set<char *> TrackAlloc<UserAllocator>::allocated_blocks;

template <typename UserAllocator>
UserAllocator TrackAlloc<UserAllocator>::m_alloc;

typedef TrackAlloc<memory_mgr::default_user_allocator_new_delete> track_alloc;

BOOST_AUTO_TEST_CASE( test_memory_usage )
{
	typedef memory_mgr::pool<track_alloc> pool_type;

	{
		// Constructor should do nothing; no memory allocation
		pool_type pool(sizeof(int));
		BOOST_CHECK_MESSAGE( track_alloc::ok(), "Memory error" );

		BOOST_CHECK_MESSAGE( ! pool.release_memory(), "Pool released memory, however should not" );
		BOOST_CHECK_MESSAGE( ! pool.purge_memory(), "Pool purged memory, however should not" );

		// Should allocate from system
		pool.deallocate((pool.allocate)());
		BOOST_CHECK_MESSAGE( ! track_alloc::ok(), "Memory error" );

		// Ask pool to give up memory it's not using; this should succeed
		BOOST_CHECK_MESSAGE( pool.release_memory(), "Pool didn't release memory" );
		BOOST_CHECK_MESSAGE( track_alloc::ok(), "Memory error" );

		// Should allocate from system again
		pool.allocate(); // loses the pointer to the returned chunk (*A*)

		// Ask pool to give up memory it's not using; this should fail
		BOOST_CHECK_MESSAGE( ! pool.release_memory(), "Pool released memory, however should not" );

		// Force pool to give up memory it's not using; this should succeed
		//  This will clean up the memory leak from (*A*)
		BOOST_CHECK_MESSAGE( pool.purge_memory(), "Pool didn't purge memory" );
		BOOST_CHECK_MESSAGE( track_alloc::ok(), "Memory error" );

		// Should allocate from system again
		pool.allocate(); // loses the pointer to the returned chunk (*B*)

		// pool's destructor should purge the memory
		//  This will clean up the memory leak from (*B*)
	}

	BOOST_REQUIRE_MESSAGE( track_alloc::ok(), "memory inconsistent" );
}


BOOST_AUTO_TEST_SUITE_END();


