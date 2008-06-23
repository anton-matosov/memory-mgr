/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

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
#include "segment_manager.h"
#include "memory_manager.h"
#include "pointer_convert.h"
#include "size_tracking.h"
#include "heap_segment.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 257;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

//typedef memory_mgr::pointer_convert<memmgr_type> pconv_type;
//typedef memory_mgr::size_tracking< memmgr_type > sz_pconv_track_mgr;
typedef memory_mgr::heap_segment<memmgr_type> heapmgr_type;
typedef memory_mgr::segment_manager<heapmgr_type, 4> segmgr_type;


bool test_get_offset_base()
{
	SUBTEST_START( L"get_offset_base" );
	
	segmgr_type segmgr;

	size_t p1 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory );
	bool not_free = segmgr.free();
	bool not_empty = segmgr.empty();
	size_t p2 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory );
	size_t p3 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory );
	size_t p4 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory );
	size_t p5 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory, std::nothrow_t() );
	segmgr.get_offset_base( p1 );
	segmgr.get_offset_base( p2 );
	segmgr.get_offset_base( p3 );
	segmgr.get_offset_base( p4 );
	segmgr.get_offset_base( p5 );

	segmgr.deallocate( p3, memory_mgr::manager_traits<segmgr_type>::allocable_memory  );
	//segmgr.deallocate( p4, memory_mgr::manager_traits<segmgr_type>::allocable_memory  );
	segmgr.deallocate( p1, memory_mgr::manager_traits<segmgr_type>::allocable_memory );
	segmgr.deallocate( p2, memory_mgr::manager_traits<segmgr_type>::allocable_memory );

	segmgr.get_offset_base( memory_size - 1 );
	segmgr.get_offset_base( memory_size );
	segmgr.get_offset_base( memory_size + 1 );
	segmgr.get_offset_base( memory_size * 3 + 5 );

	segmgr.clear();
	return true;
}

bool test_segment_manager()
{
	TEST_START( L"test_segment_manager" );
	TEST_END( test_get_offset_base() );
}



