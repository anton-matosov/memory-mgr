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
static const size_t memory_size = 1024;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

//typedef memory_mgr::pointer_convert<memmgr_type> pconv_type;
//typedef memory_mgr::size_tracking< memmgr_type > sz_pconv_track_mgr;
typedef memory_mgr::heap_segment<memmgr_type> heapmgr_type;
typedef memory_mgr::segment_manager<heapmgr_type, 5> segmgr_type;


bool test_get_offset_base()
{
	SUBTEST_START( L"get_offset_base" );
	
	segmgr_type segmgr;

	segmgr.allocate( chunk_size );
// 	size_t p1 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory - chunk_size );
// 	bool not_free = segmgr.free();
// 	bool not_empty = segmgr.empty();
// 	segmgr.allocate( chunk_size );
// 	size_t p2 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory  - chunk_size);
// 	segmgr.allocate( chunk_size );
// 	size_t p3 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory  - chunk_size);
// 	segmgr.allocate( chunk_size );
// 	size_t p4 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory  - chunk_size);
// 	size_t p5 = segmgr.allocate( memory_mgr::manager_traits<segmgr_type>::allocable_memory, std::nothrow_t() );

	size_t p5 = 0;
	typedef std::vector<size_t> ptrs_vec;
	ptrs_vec ptrs;
	size_t alloc_chunks = memory_mgr::manager_traits<segmgr_type>::allocable_chunks * segmgr_type::num_segments;
	ptrs.reserve( alloc_chunks );
 	for( size_t i = 0; i < alloc_chunks && p5 != memory_mgr::offset_traits<size_t>::invalid_offset; ++i )
 	{
		p5 = segmgr.allocate( chunk_size, std::nothrow_t() );
		ptrs.push_back( p5 );
 	}

	//std::random_shuffle( ptrs.begin(), ptrs.end() );

	size_t i =0;

	for ( ptrs_vec::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it )
	{
		size_t p = *it;
		void* p_base = segmgr.get_offset_base( p );
		void* vp = segmgr.offset_to_pointer( p );
		void* vp_base = segmgr.get_ptr_base( vp );
		if( vp_base != p_base )
		{
			TEST_FAILED_MSG( L"Invalid base" );
		}
		++i;
	}

// 	void* p1_base = segmgr.get_offset_base( p1 );
// 	void* p2_base = segmgr.get_offset_base( p2 );
// 	void* p3_base = segmgr.get_offset_base( p3 );
// 	void* p4_base = segmgr.get_offset_base( p4 );
// 	void* p5_base = segmgr.get_offset_base( p5 );
// 	
// 
// 	void* vp1 = segmgr.offset_to_pointer( p1 );
// 	void* vp2 = segmgr.offset_to_pointer( p2 );
// 	void* vp3 = segmgr.offset_to_pointer( p3 );
// 	void* vp4 = segmgr.offset_to_pointer( p4 );
// 	void* vp5 = segmgr.offset_to_pointer( p5 );
// 	void* base0 = segmgr.get_ptr_base( 0 );
// 	void* vp1_base = segmgr.get_ptr_base( vp1 );
// 	void* vp2_base = segmgr.get_ptr_base( vp2 );
// 	void* vp3_base = segmgr.get_ptr_base( vp3 );
// 	void* vp4_base = segmgr.get_ptr_base( vp4 );
// 
// 	TEST_CHECK_MSG( vp1_base == p1_base, L"Invalid base" );
// 	TEST_CHECK_MSG( vp2_base == p2_base, L"Invalid base" );
// 	TEST_CHECK_MSG( vp3_base == p3_base, L"Invalid base" );
// 	TEST_CHECK_MSG( vp4_base == p4_base, L"Invalid base" );

// 	segmgr.deallocate( p3, memory_mgr::manager_traits<segmgr_type>::allocable_memory - chunk_size );
// 	segmgr.deallocate( p4, memory_mgr::manager_traits<segmgr_type>::allocable_memory  );
// 	segmgr.deallocate( p1, memory_mgr::manager_traits<segmgr_type>::allocable_memory - chunk_size );
// 	segmgr.deallocate( p2, memory_mgr::manager_traits<segmgr_type>::allocable_memory - chunk_size );

	//segmgr.get_offset_base( memory_size - 1 );
	//segmgr.get_offset_base( memory_size );
	//segmgr.get_offset_base( memory_size + 1 );
	//segmgr.get_offset_base( memory_size * 3 + 5 );

	segmgr.clear();
	return true;
}

bool test_segment_manager()
{
	TEST_START( L"test_segment_manager" );
	TEST_END( test_get_offset_base() );
}



