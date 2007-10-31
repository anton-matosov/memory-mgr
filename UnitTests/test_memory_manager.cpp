#include "StdAfx.h"
#include <vector>
#include "memory_manager.h"

typedef unsigned char chunk_t;
std::vector<chunk_t> vec(10000);
typedef managers::memory_manager<chunk_t, 1024, 4 > memmgr_t;
memmgr_t mgr( &*vec.begin() );
managers::size_tracking< memmgr_t > track_mgr( &*vec.begin() );

template class managers::memory_manager<chunk_t, 1024, 4 >;
template class managers::size_tracking< memmgr_t >;

typedef memmgr_t::ptr_t ptr_t;

class TestClass
{
public:

	TestClass()
	{}
	~TestClass()
	{}

	static void* operator new( size_t size )
	{
		return mgr.allocate( size ).get();
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
		mgr.deallocate( memmgr_t::ptr_t(p), size );
	}
};

bool test_memory_manager()
{
 	TestClass* test = new TestClass();


 	ptr_t p1 = track_mgr.allocate( 4 );
 	ptr_t p2 = track_mgr.allocate( 4 );
 	ptr_t p3 = track_mgr.allocate( 4 );
 	ptr_t p4 = track_mgr.allocate( 4 );
 
 	track_mgr.deallocate( p3 );
 	track_mgr.deallocate( p1 );
 	track_mgr.deallocate( p4 );
 
 	delete test;
 
 	track_mgr.deallocate( p2 );

	return true;
}