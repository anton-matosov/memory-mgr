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
#include "size_tracking.h"

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
template class memory_mgr::size_tracking< memmgr_type >;

bool test_size_tracking()
{
	TEST_START( L"size_tracking" );
	std::vector<chunk_type> memory( memory_size );
	memory_mgr::size_tracking< memmgr_type > track_mgr( &*memory.begin() );

	const memmgr_type::size_type obj_size = 4;
	ptr_type p1 = track_mgr.allocate( obj_size );
	ptr_type p2 = track_mgr.allocate( obj_size );
	ptr_type p3 = track_mgr.allocate( obj_size );
	ptr_type p4 = track_mgr.allocate( obj_size );
	ptr_type p5 = track_mgr.allocate( obj_size );

	track_mgr.deallocate( p3 );
	track_mgr.deallocate( p5 );
	track_mgr.deallocate( p1 );
	track_mgr.deallocate( p2 );
	track_mgr.deallocate( p4 );

	TEST_END( track_mgr.free() );
}

