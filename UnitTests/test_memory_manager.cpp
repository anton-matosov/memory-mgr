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

#include "StdAfx.h"
#include <vector>
#include "test_case.h"
#include "memory_manager.h"
#include "heap_memory.h"
#include "TestClass.h"

typedef unsigned char chunk_t;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef memory_mgr::memory_manager<chunk_t, memory_size, chunk_size > memmgr_t;
template memmgr_t;
template class memory_mgr::size_tracking< memmgr_t >;

typedef memmgr_t::ptr_type ptr_t;




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
	memory_mgr::size_tracking< memmgr_t > track_mgr( &*memory.begin() );

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
	memory_mgr::size_tracking< memmgr_t > mgr( &*memory.begin() );

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
 	TEST_START( L"memory memory_mgr" );

	TEST_END( test_alloc_dealloc() &&
	test_size_tracking() &&
	test_out_of_memory() &&
	test_managed_base()
	);

}