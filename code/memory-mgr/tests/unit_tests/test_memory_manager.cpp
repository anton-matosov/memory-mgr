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


#include <vector>
#include "memory-mgr/memory_manager.h"
#include "memory-mgr/heap_segment.h"
#include "memory-mgr/detail/ptr_helpers.h"
#include "common_manager_tests.h"
#include <boost/dynamic_bitset.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_memory_manager )

	typedef unsigned int chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 200 * 1024; // 20 * 1024 causes build error

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
	typedef void* pointer;

	BOOST_AUTO_TEST_CASE( test_alloc_dealloc )
	{	
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );
		const size_t allocable_memory = memory_mgr::manager_traits<memmgr_type>::allocable_memory;
		const memmgr_type::size_type obj_size = sizeof( chunk_type ) * 8 * chunk_size * 2;

		BOOST_REQUIRE_GT( allocable_memory, obj_size * 5 );

		pointer p1 = mgr.allocate( obj_size );
		pointer p2 = mgr.allocate( obj_size );
		pointer p3 = mgr.allocate( obj_size );
		pointer p4 = mgr.allocate( obj_size );
		pointer p5 = mgr.allocate( obj_size );

		test::check_pointers( p1, p2, p3, p4, p5, obj_size );

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
		BOOST_CHECK_THROW( mgr.allocate( allocable_memory ), std::bad_alloc );
		BOOST_CHECK_THROW( mgr.allocate( allocable_memory ), std::bad_alloc );
	}

	BOOST_AUTO_TEST_CASE( test_out_of_memory_nothrow )
	{
		std::vector<chunk_type> memory( memory_size );
		memmgr_type mgr( &*memory.begin() );

		size_t allocable_memory = memory_mgr::manager_traits<memmgr_type>::allocable_memory;
		pointer null_ptr = memory_mgr::offset_traits<pointer>::invalid_offset;
					
		pointer p_inval1 = mgr.allocate( allocable_memory + 1, std::nothrow_t() );
		BOOST_CHECK( p_inval1 == null_ptr );
		
		pointer p_valid = mgr.allocate( allocable_memory, std::nothrow_t() );
		BOOST_CHECK( p_valid != null_ptr );
		
		pointer p_inval2 = mgr.allocate( 1, std::nothrow_t() );
		BOOST_CHECK( p_inval2 == null_ptr );

		BOOST_CHECK( p_valid != null_ptr
			&& p_inval1 == null_ptr
			&& p_inval2 == null_ptr );
		
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
			void* ptr;
			void* upper_bound = memory_mgr::detail::char_cast( mgr.get_segment_base() )
				+ memory_mgr::manager_traits< memmgr_type >::memory_size;
			for( size_t i = 0; i <  memory_mgr::manager_traits< memmgr_type >::num_chunks; ++i )
			{
				ptr = mgr.allocate( memory_mgr::manager_traits< memmgr_type >::chunk_size );
				BOOST_CHECK_MESSAGE( ptr < upper_bound, "returned pointer is out of upper segment bound" );
			}
		}
		catch( std::bad_alloc& )
		{
		}
	}

	BOOST_AUTO_TEST_CASE( test_data_validness )
	{
		enum{ mem_size = 64 * 1024 };
		test::test_data_validness< memory_mgr::memory_manager<chunk_type, mem_size, chunk_size > >();
	}

	BOOST_AUTO_TEST_CASE( test_null_ptr )
	{
		test::test_null_pointer_dealloc<memmgr_type>();
	}

BOOST_AUTO_TEST_SUITE_END();

