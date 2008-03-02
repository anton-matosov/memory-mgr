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
#include "test_case.h"
#include "memory_manager.h"
#include "shared_segment.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

template class memory_mgr::shared_segment< memmgr_type >;

MGR_DECLARE_SEGMENT_NAME( test_segment, "test segment" );

typedef memory_mgr::shared_segment< memmgr_type, MGR_SEGMENT_NAME(test_segment) > shared_mgr_type;
typedef memmgr_type::offset_type offset_type;

bool test_shared_segment()
{
	TEST_START( L"shared_segment" );
	TEST_PRINT( L"Creating shared_memory_manager" );
	shared_mgr_type mgr;
	TEST_CHECK( mgr.free() );

	offset_type p1, p2, p3, p4, p5;
	offset_type inv_off = memory_mgr::offset_traits<offset_type>::invalid_offset;
	const memmgr_type::size_type obj_size = 4;
	{
		TEST_PRINT( L"Creating second shared_memory_manager" );
		shared_mgr_type mgr2;
		TEST_CHECK( mgr2.free() );
 		
		TEST_PRINT( L"Allocating p1 using second manager" );
		p1 = mgr2.allocate( obj_size );
		TEST_CHECK( p1 != inv_off );

		TEST_PRINT( L"Allocating p2 using second manager" );
 		p2 = mgr2.allocate( obj_size );
		TEST_CHECK( p2 != inv_off && p1 != p2 );

		TEST_PRINT( L"Creating third shared_memory_manager" );
		shared_mgr_type mgr3;
		TEST_CHECK( !mgr3.free() );

		TEST_PRINT( L"Allocating p3 using third manager" );
 		p3 = mgr3.allocate( obj_size );
		TEST_CHECK( p3 != inv_off );

		TEST_PRINT( L"Allocating p4 using third manager" );
 		p4 = mgr3.allocate( obj_size );
		TEST_CHECK( p4 != inv_off );

		TEST_PRINT( L"Allocating p5 using third manager" );
 		p5 = mgr3.allocate( obj_size );
		TEST_CHECK( p5 != inv_off );

		TEST_PRINT( L"Deallocating p3 using second manager" );
		mgr2.deallocate( p3, obj_size );

		TEST_PRINT( L"Deallocating p1 using third manager" );
		mgr3.deallocate( p1, obj_size );
	}

	TEST_PRINT( L"Deallocating p2 using first manager" );
	mgr.deallocate( p2, obj_size );
	
	TEST_PRINT( L"Deallocating p4 using first manager" );
	mgr.deallocate( p4, obj_size );
	
	TEST_PRINT( L"Deallocating p5 using first manager" );
	mgr.deallocate( p5, obj_size );
 	
	TEST_END( mgr.free() );
}
