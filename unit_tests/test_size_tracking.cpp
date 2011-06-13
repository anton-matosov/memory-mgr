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
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/size_tracking.h>
#include "common_manager_tests.h"

namespace
{

	typedef unsigned int chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

	typedef memory_mgr::size_tracking< memmgr_type >		sz_track_mgr;

	typedef boost::mpl::list< sz_track_mgr >		managers_list;

}
BOOST_AUTO_TEST_SUITE( test_size_tracking )


	BOOST_AUTO_TEST_CASE_TEMPLATE( test_size_tracking, mgr_type, managers_list )
	{
		typedef void* ptr_type;

		std::vector<chunk_type> memory( memory_size );
		mgr_type track_mgr( &*memory.begin() );

		const typename memory_mgr::manager_traits<mgr_type>::size_type obj_size = 4;
		ptr_type p1 = track_mgr.allocate( obj_size );
		ptr_type p2 = track_mgr.allocate( obj_size );
		ptr_type p3 = track_mgr.allocate( obj_size );
		ptr_type p4 = track_mgr.allocate( obj_size );
		ptr_type p5 = track_mgr.allocate( obj_size );

		test::check_pointers( p1, p2, p3, p4, p5 );

		track_mgr.deallocate( p3 );
		track_mgr.deallocate( p5 );
		track_mgr.deallocate( p1 );
		track_mgr.deallocate( p2 );
		track_mgr.deallocate( p4 );

		BOOST_CHECK( track_mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_null_ptr )
	{
		test::test_null_pointer_dealloc<sz_track_mgr>();
	}

	BOOST_AUTO_TEST_CASE( test_size_tracking_data )
	{
		typedef sz_track_mgr		mgr_type;
		typedef void*				ptr_type;

		std::vector<chunk_type> memory( memory_size );
		mgr_type track_mgr( &*memory.begin() );

		const memory_mgr::manager_traits<mgr_type>::size_type obj_size = 4;
		ptr_type p1 = track_mgr.allocate( obj_size );
		char* c1 = memory_mgr::detail::char_cast( p1 );
		*c1 = 1;
		BOOST_CHECK_EQUAL( *c1, 1 );

		ptr_type p2 = track_mgr.allocate( obj_size );
		char* c2 = memory_mgr::detail::char_cast( p2 );
		*c2 = 2;
		BOOST_CHECK_EQUAL( *c2, 2 );

		ptr_type p3 = track_mgr.allocate( obj_size );
		char* c3 = memory_mgr::detail::char_cast( p3 );
		*c3 = 3;
		BOOST_CHECK_EQUAL( *c3, 3 );

		ptr_type p4 = track_mgr.allocate( obj_size );
		char* c4 = memory_mgr::detail::char_cast( p4 );
		*c4 = 4;
		BOOST_CHECK_EQUAL( *c4, 4 );

		ptr_type p5 = track_mgr.allocate( obj_size );
		char* c5 = memory_mgr::detail::char_cast( p5 );
		*c5 = 5;
		BOOST_CHECK_EQUAL( *c5, 5 );
		BOOST_CHECK_EQUAL( *c4, 4 );
		BOOST_CHECK_EQUAL( *c3, 3 );
		BOOST_CHECK_EQUAL( *c2, 2 );
		BOOST_CHECK_EQUAL( *c1, 1 );

		test::check_pointers( p1, p2, p3, p4, p5 );

		track_mgr.deallocate( p3 );
		BOOST_CHECK_EQUAL( *c5, 5 );
		BOOST_CHECK_EQUAL( *c4, 4 );
		BOOST_CHECK_EQUAL( *c2, 2 );
		BOOST_CHECK_EQUAL( *c1, 1 );
		
		track_mgr.deallocate( p5 );
		BOOST_CHECK_EQUAL( *c4, 4 );
		BOOST_CHECK_EQUAL( *c2, 2 );
		BOOST_CHECK_EQUAL( *c1, 1 );
		
		track_mgr.deallocate( p1 );
		BOOST_CHECK_EQUAL( *c4, 4 );
		BOOST_CHECK_EQUAL( *c2, 2 );
		
		track_mgr.deallocate( p2 );
		BOOST_CHECK_EQUAL( *c4, 4 );
		
		track_mgr.deallocate( p4 );

		BOOST_CHECK( track_mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_data_validness )
	{
		enum{ memory_size = 64 * 1024 };

		typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
		
		test::test_data_validness<memory_mgr::size_tracking< memmgr_type > >();
	}
BOOST_AUTO_TEST_SUITE_END();

