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
#include <memory-mgr/segment_manager.h>
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/pointer_convert.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>

namespace
{

	typedef unsigned char chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 1024;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

	typedef memory_mgr::heap_segment<memmgr_type> heapmgr_type;
	typedef memory_mgr::segment_manager<heapmgr_type, 50> segmgr_type;

	typedef memory_mgr::pointer_convert<segmgr_type> pconv_segmgr_type;
	typedef memory_mgr::size_tracking< pconv_segmgr_type > sz_pconv_segmgr_type;

	typedef memory_mgr::manager_traits<segmgr_type>::offset_type offset_type;


	enum
	{
		alloc_chunks = memory_mgr::manager_traits<segmgr_type>::allocable_chunks * segmgr_type::num_segments - 1
	};

	bool is_valid_ptr( offset_type p )
	{
		return p != memory_mgr::offset_traits<size_t>::invalid_offset;
	}

	bool is_valid_ptr( void* p )
	{
		return p != 0;
	}

	template< class MemMgr, class PtrType>
	bool test_alloc_dealloc()
	{
		SUBTEST_START( L"allocation/deallocation" );

		typedef PtrType					ptr_type;
		typedef std::vector< ptr_type >			ptrs_vec;

		MemMgr segmgr;
		ptrs_vec ptrs;

		ptr_type p = 0;

		ptrs.reserve( alloc_chunks );
		for( size_t i = 0; i < alloc_chunks; ++i )
		{
			p = segmgr.allocate( chunk_size, std::nothrow_t() ) ;

			ptrs.push_back( p );
			if( !is_valid_ptr( p ) )
			{
				break;
			}
		}

		std::random_shuffle( ptrs.begin(), ptrs.end() );

		for ( typename ptrs_vec::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it )
		{
			segmgr.deallocate( *it, chunk_size );
		}

		SUBTEST_END( segmgr.is_free() );
	}

	bool test_offset_convertions()
	{
		SUBTEST_START( L"get_offset_base" );


		typedef std::vector< offset_type > ptrs_vec;

		segmgr_type segmgr;

		size_t p = 0;
		ptrs_vec ptrs;

		ptrs.reserve( alloc_chunks );
		for( size_t i = 0; i < alloc_chunks && p != memory_mgr::offset_traits<size_t>::invalid_offset; ++i )
		{
			p = segmgr.allocate( chunk_size, std::nothrow_t() );
			ptrs.push_back( p );
		}

		std::random_shuffle( ptrs.begin(), ptrs.end() );

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

			size_t off = segmgr.pointer_to_offset( vp ) ;
			if( off != p )
			{
				TEST_FAILED_MSG( L"Invalid offset" );
			}

			segmgr.deallocate( *it, chunk_size );
		}

		SUBTEST_END( segmgr.is_free() );
	}

	template<class MemMgr>
	bool test_null_ptr()
	{
		SUBTEST_START( L"deallocation of null ptr" );
		MemMgr mgr;

		mgr.deallocate( 0, 0 );

		SUBTEST_SUCCEDED;
	}

	bool test_inv_offset()
	{
		SUBTEST_START( L"deallocation of null ptr" );
		segmgr_type mgr;

		offset_type null_ptr = memory_mgr::offset_traits<offset_type>::invalid_offset;
		mgr.deallocate( null_ptr, 0 );

		SUBTEST_SUCCEDED;
	}
}

bool test_segment_manager()
{
	TEST_START( L"test_segment_manager" );
	TEST_END( (
		test_alloc_dealloc<segmgr_type, offset_type>()
		&& test_offset_convertions()
		&& test_alloc_dealloc<pconv_segmgr_type, void*>()
		&& test_alloc_dealloc<sz_pconv_segmgr_type, void*>()
		&& test_inv_offset()
		&& test_null_ptr<pconv_segmgr_type>()
		&& test_null_ptr<sz_pconv_segmgr_type>()
		)  );
}



