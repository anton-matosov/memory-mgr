#include "StdAfx.h"
#include <vector>
#include "test_case.h"
#include "memory_manager.h"
#include "heap_memory_manager.h"
#include "managed_base.h"

typedef unsigned char chunk_t;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef managers::memory_manager<chunk_t, memory_size, chunk_size > memmgr_t;
template memmgr_t;
template class managers::size_tracking< memmgr_t >;

typedef memmgr_t::ptr_t ptr_t;


class TestClass: public managers::managed_base< managers::def_heap_mgr >
{
	int m_i;
public:
	typedef managers::managed_base< managers::def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	TestClass()
		:m_i(0)
	{}
	
	~TestClass()
	{
		m_i = 0;
	}

	void set( int i )
	{
		m_i = i;
	}

	int get()
	{
		return m_i;
	}
};

bool test_alloc_dealloc()
{
	SUBTEST_START( L"allocation/deallocation" );
	std::vector<chunk_t> memory( memory_size );
	memmgr_t mgr( &*memory.begin() );
	const memmgr_t::size_type obj_size = 4;
	ptr_t p1 = mgr.allocate( obj_size );
	ptr_t p2 = mgr.allocate( obj_size );
	ptr_t p3 = mgr.allocate( obj_size );
	ptr_t p4 = mgr.allocate( obj_size );
	ptr_t p5 = mgr.allocate( obj_size );

	mgr.deallocate( p3, obj_size );
	mgr.deallocate( p5, obj_size );
	mgr.deallocate( p1, obj_size );
	mgr.deallocate( p2, obj_size );
	mgr.deallocate( p4, obj_size );

	SUBTEST_END( mgr.free() );
}

bool test_size_tracking()
{
	SUBTEST_START( L"size_tracking" );
	std::vector<chunk_t> memory( memory_size );
	managers::size_tracking< memmgr_t > track_mgr( &*memory.begin() );

	const memmgr_t::size_type obj_size = 4;
	ptr_t p1 = track_mgr.allocate( obj_size );
	ptr_t p2 = track_mgr.allocate( obj_size );
	ptr_t p3 = track_mgr.allocate( obj_size );
	ptr_t p4 = track_mgr.allocate( obj_size );
	ptr_t p5 = track_mgr.allocate( obj_size );

	track_mgr.deallocate( p3 );
	track_mgr.deallocate( p5 );
	track_mgr.deallocate( p1 );
	track_mgr.deallocate( p2 );
	track_mgr.deallocate( p4 );

	SUBTEST_END( track_mgr.free() );
}

bool test_out_of_memory()
{
	SUBTEST_START( L"out of memory case" );
	std::vector<chunk_t> memory( memory_size );
	managers::size_tracking< memmgr_t > mgr( &*memory.begin() );

	try
	{
		mgr.allocate( memory_size );
		mgr.allocate( memory_size );
	}
	catch( std::bad_alloc& )
	{
		SUBTEST_SUCCEDED;	
	}
	SUBTEST_FAILED;
}

bool test_managed_base()
{
	SUBTEST_START( L"managed_base" );
	TestClass* t1 = new TestClass();
	TestClass* t2 = new TestClass();
	TestClass* t3 = new TestClass();
	TestClass* t4 = new TestClass();

	t1->set( 101 );
	t2->set( 102 );
	t3->set( 103 );
	t4->set( 104 );

	delete t2;
	delete t4;
	delete t1;
	delete t3;

	SUBTEST_END( TestClass::mem_mgr::instance().free() );
}

bool test_memory_manager()
{
 	TEST_START( L"memory managers" );

	TEST_END( test_alloc_dealloc() &&
	test_size_tracking() &&
	test_out_of_memory() &&
	test_managed_base()
	);
}