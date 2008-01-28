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
#include "heap_segment.h"
#include "memory_segment.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

template class memory_mgr::memory_segment< memory_mgr::vector_as_allocator, memmgr_type >;

typedef memory_mgr::memory_segment< memory_mgr::vector_as_allocator, memmgr_type > segmmgr_type;
typedef memmgr_type::offset_type offset_type;

bool test_memory_segment()
{
	SUBTEST_START( L"allocation/deallocation" );
	segmmgr_type mgr;

	const memmgr_type::size_type obj_size = 4;
	offset_type p1 = mgr.allocate( obj_size );
	offset_type p2 = mgr.allocate( obj_size );
	offset_type p3 = mgr.allocate( obj_size );
	offset_type p4 = mgr.allocate( obj_size );
	offset_type p5 = mgr.allocate( obj_size );

	mgr.deallocate( p3, obj_size );
	mgr.deallocate( p5, obj_size );
	mgr.deallocate( p1, obj_size );
	mgr.deallocate( p2, obj_size );
	mgr.deallocate( p4, obj_size );

	SUBTEST_END( mgr.free() );
}


