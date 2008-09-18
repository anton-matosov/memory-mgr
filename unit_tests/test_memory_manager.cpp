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
#include <vector>
#include "test_case.h"
#include "common_manager_tests.h"
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/detail/ptr_helpers.h>

namespace
{

	typedef unsigned char chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

	//template class memory_mgr::memory_manager<chunk_type, memory_size, chunk_size >;

	typedef memmgr_type::offset_type offset_type;
}


BOOST_AUTO_TEST_SUITE( test_memory_manager )

	BOOST_AUTO_TEST_CASE( test_alloc_dealloc )
	{	
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );
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

		BOOST_CHECK( mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_out_of_memory )
	{	
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );

		size_t allocable_memory = memory_mgr::manager_traits<memmgr_type>::allocable_memory;
		
		mgr.allocate( allocable_memory );
		BOOST_CHECK_THROW( mgr.allocate( allocable_memory ), std::bad_alloc );
	}

	BOOST_AUTO_TEST_CASE( test_out_of_memory_nothrow )
	{
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );

		size_t allocable_memory = memory_mgr::manager_traits<memmgr_type>::allocable_memory;
		offset_type null_ptr = memory_mgr::offset_traits<offset_type>::invalid_offset;
		try
		{
			
			offset_type p_inval1 = mgr.allocate( allocable_memory + 1, std::nothrow_t() );
			BOOST_CHECK( p_inval1 == null_ptr );

			
			offset_type p_valid = mgr.allocate( allocable_memory, std::nothrow_t() );
			BOOST_CHECK( p_valid != null_ptr );

			
			offset_type p_inval2 = mgr.allocate( 1, std::nothrow_t() );
			BOOST_CHECK( p_inval2 == null_ptr );

			BOOST_CHECK( p_valid != null_ptr
				&& p_inval1 == null_ptr
				&& p_inval2 == null_ptr );
		}
		catch( std::bad_alloc& )
		{
			BOOST_ERROR( "exception thrown by no_throw() method" );	
		}
	}


	BOOST_AUTO_TEST_CASE( test_size_calculation )
	{	
		/*
		Bug: #1987919: chunks number and available memory calculated incorrectly 

		chunks number and available memory calculated incorrectly, so memory
		manager can allocate block to memory that is out of memory segment, using
		of this block can bring access violation error and/or memory corruption
		*/
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );

		try
		{
			memory_mgr::manager_traits< memmgr_type >::offset_type ptr;
			void* upper_bound = memory_mgr::detail::char_cast( mgr.get_segment_base() )
				+ memory_mgr::manager_traits< memmgr_type >::memory_size;
			for( size_t i = 0; i <  memory_mgr::manager_traits< memmgr_type >::num_chunks; ++i )
			{
				ptr = mgr.allocate( memory_mgr::manager_traits< memmgr_type >::chunk_size );
				void* p = memory_mgr::detail::offset_to_pointer( ptr, mgr );
				if( p >= upper_bound )
				{
					BOOST_ERROR( "returned pointer is out of upper segment bound" );	
				}

			}
		}
		catch( std::bad_alloc& )
		{
		}
	}


	BOOST_AUTO_TEST_CASE( test_null_ptr )
	{
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );

		offset_type null_ptr = memory_mgr::offset_traits<offset_type>::invalid_offset;

		BOOST_CHECKPOINT( "before deallocation of null ptr" );
		mgr.deallocate( null_ptr, 0 );
		BOOST_CHECKPOINT( "after deallocation of null ptr" );
	}

BOOST_AUTO_TEST_SUITE_END();

