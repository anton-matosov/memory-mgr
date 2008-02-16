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

typedef memory_mgr::shared_segment< memmgr_type > shared_mgr_type;
typedef memmgr_type::offset_type offset_type;

bool test_shared_segment()
{
	TEST_START( L"shared_segment" );
	shared_mgr_type mgr;
	offset_type p1, p2, p3, p4, p5;
	const memmgr_type::size_type obj_size = 4;
	{
		shared_mgr_type mgr2;

 		
 		p1 = mgr2.allocate( obj_size );
 		p2 = mgr2.allocate( obj_size );

		shared_mgr_type mgr3;
 		p3 = mgr3.allocate( obj_size );
 		p4 = mgr3.allocate( obj_size );
 		p5 = mgr3.allocate( obj_size );

		mgr.deallocate( p5, obj_size );
	}

	mgr.deallocate( p3, obj_size );
	mgr.deallocate( p2, obj_size );
	mgr.deallocate( p4, obj_size );

 	
 	mgr.deallocate( p1, obj_size );

 	

	TEST_END( mgr.free() );
}