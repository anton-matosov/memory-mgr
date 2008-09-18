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
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/pointer_convert.h>
#include <memory-mgr/size_tracking.h>

namespace
{

	typedef unsigned char chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
	typedef memory_mgr::pointer_convert<memmgr_type> pconv_type;

	typedef memory_mgr::size_tracking< pconv_type > sz_pconv_track_mgr;
	typedef memory_mgr::size_tracking< memmgr_type > sz_track_mgr;

	//template class memory_mgr::size_tracking< memmgr_type >;
	//template class memory_mgr::size_tracking< pconv_type >;

	template<class MgrType, class ptr_type>
	bool test_size_tracking_impl( const wchar_t* name )
	{
		typedef MgrType mgr_type;
		SUBTEST_START( name );
		std::vector<chunk_type> memory( memory_size );
		mgr_type track_mgr( &*memory.begin() );

		const typename memory_mgr::manager_traits<mgr_type>::size_type obj_size = 4;
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

		SUBTEST_END( track_mgr.is_free() );
	}

	template<class MgrType, class ptr_type>
	bool test_size_tracking_data( const wchar_t* name )
	{
		typedef MgrType mgr_type;
		SUBTEST_START( name );
		std::vector<chunk_type> memory( memory_size );
		mgr_type track_mgr( &*memory.begin() );

		const typename memory_mgr::manager_traits<mgr_type>::size_type obj_size = 4;
		ptr_type p1 = track_mgr.allocate( obj_size );
		char* c1 = memory_mgr::detail::char_cast( p1 );
		*c1 = 1;

		ptr_type p2 = track_mgr.allocate( obj_size );
		char* c2 = memory_mgr::detail::char_cast( p2 );
		*c2 = 2;


		ptr_type p3 = track_mgr.allocate( obj_size );
		char* c3 = memory_mgr::detail::char_cast( p3 );
		*c3 = 3;

		ptr_type p4 = track_mgr.allocate( obj_size );
		char* c4 = memory_mgr::detail::char_cast( p4 );
		*c4 = 4;

		ptr_type p5 = track_mgr.allocate( obj_size );
		char* c5 = memory_mgr::detail::char_cast( p5 );
		*c5 = 5;

		TEST_CHECK_MSG( (*c1 == 1) && (*c2 == 2) && (*c3 == 3) && (*c4 == 4) && (*c5 == 5), L"allocated memory data is corrupted" );

		track_mgr.deallocate( p3 );
		TEST_CHECK_MSG( (*c1 == 1) && (*c2 == 2) && (*c4 == 4) && (*c5 == 5), L"allocated memory data is corrupted" );

		track_mgr.deallocate( p5 );
		TEST_CHECK_MSG( (*c1 == 1) && (*c2 == 2) && (*c4 == 4), L"allocated memory data is corrupted" );

		track_mgr.deallocate( p1 );
		TEST_CHECK_MSG( (*c2 == 2) && (*c4 == 4), L"allocated memory data is corrupted" );

		track_mgr.deallocate( p2 );
		TEST_CHECK_MSG( *c4 == 4, L"allocated memory data is corrupted" );

		track_mgr.deallocate( p4 );

		SUBTEST_END( track_mgr.is_free() );
	}

	bool test_null_ptr()
	{
		SUBTEST_START( L"deallocation of null ptr" );
		std::vector<chunk_type> memory( memory_size );
		sz_pconv_track_mgr mgr( &*memory.begin() );

		mgr.deallocate( 0, 0 );

		SUBTEST_SUCCEDED;
	}

	bool test_inv_offset()
	{
		SUBTEST_START( L"deallocation of null ptr" );
		std::vector<chunk_type> memory( memory_size );
		sz_track_mgr mgr( &*memory.begin() );

		typedef memory_mgr::manager_traits<sz_track_mgr>::offset_type offset_type;

		offset_type null_ptr = memory_mgr::offset_traits<offset_type>::invalid_offset;
		mgr.deallocate( null_ptr, 0 );

		SUBTEST_SUCCEDED;
	}

}
BOOST_AUTO_TEST_SUITE( test_size_tracking )

	BOOST_AUTO_TEST_CASE( test_size_tracking )
	{
		BOOST_CHECK( (test_size_tracking_impl<sz_pconv_track_mgr, void*>( L"with pointer_convert" ) )
			&& (test_size_tracking_impl<sz_track_mgr, 
			memory_mgr::manager_traits<sz_track_mgr>::offset_type >( L"w/o pointer_convert" )
			&& test_size_tracking_data<sz_pconv_track_mgr, void*>( L"memory data validation" ))
			&& test_null_ptr()
			&& test_inv_offset()
			);
	}

BOOST_AUTO_TEST_SUITE_END();

