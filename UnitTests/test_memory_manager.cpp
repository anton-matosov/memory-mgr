#include "StdAfx.h"
#include <vector>
#include "test_case.h"
#include "memory_manager.h"

typedef unsigned char chunk_t;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef managers::memory_manager<chunk_t, memory_size, chunk_size > memmgr_t;
template memmgr_t;
template class managers::size_tracking< memmgr_t >;

typedef memmgr_t::ptr_t ptr_t;


#include "heap_memory_manager.h"
class CC{

public:
	CC()
	{

	}

	~CC()
	{

	}
};
typedef managers::singleton<CC> ccsing;
template class managers::singleton<int>;

template< class mem_mgr = managers::def_heap_mgr >
class TestClass
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
	static void* operator new( size_t size )
	{
		return mem_mgr::instance().allocate( size ).get_ptr( mem_mgr::instance() );
	}

	static void* operator new(  size_t, void* p )
	{
		return p;
	}

	// 	static void operator delete( void* p )
	// 	{
	// 		mgr.deallocate( p );
	// 	}

	static void operator delete( void* p, size_t size )
	{
		mem_mgr::instance().deallocate( mem_mgr::ptr_t( mem_mgr::instance(), p ), size );
	}
};



bool test_memory_manager()
{
 	TEST_START( L"static_bitset" );
	std::vector<chunk_t> memory( memory_size );
	memmgr_t mgr( &*memory.begin() );
	managers::size_tracking< memmgr_t > track_mgr( &*memory.begin() );


	ccsing::instance();

	TestClass<>* t = new TestClass<>();

	t->set( 100 );
// 
// 
//  	ptr_t p1 = track_mgr.allocate( 4 );
//  	ptr_t p2 = track_mgr.allocate( 4 );
//  	ptr_t p3 = track_mgr.allocate( 4 );
//  	ptr_t p4 = track_mgr.allocate( 4 );
//  
//  	track_mgr.deallocate( p3 );
//  	track_mgr.deallocate( p1 );
//  	track_mgr.deallocate( p4 );
//  
  	delete t;
//  
//  	track_mgr.deallocate( p2 );

	return true;
}