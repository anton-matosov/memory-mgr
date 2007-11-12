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

bool test_memory_manager()
{
 	TEST_START( L"static_bitset" );
	std::vector<chunk_t> memory( memory_size );
	memmgr_t mgr( &*memory.begin() );
	managers::size_tracking< memmgr_t > track_mgr( &*memory.begin() );


	TestClass* t = new TestClass();

	t->set( 100 );

 	for( int i = 0; i < 1000; ++i )
 	{
 		track_mgr.allocate( 128 );
 	}
// 
// 
 	ptr_t p1 = track_mgr.allocate( 4 );
//  	ptr_t p2 = track_mgr.allocate( 4 );
//  	ptr_t p3 = track_mgr.allocate( 4 );
//  	ptr_t p4 = track_mgr.allocate( 4 );
//  
//  	track_mgr.deallocate( p3 );
	track_mgr.deallocate( p1 );
  	track_mgr.deallocate( p1 );
//  	track_mgr.deallocate( p4 );
//  
  	delete t;
//  
//  	track_mgr.deallocate( p2 );

	return true;
}