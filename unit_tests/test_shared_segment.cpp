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
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/shared_segment.h>


BOOST_AUTO_TEST_SUITE( test_shared_segment )

	typedef unsigned char chunk_type;
	static const size_t chunk_size = 4;
	static const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

	template class memory_mgr::shared_segment< memmgr_type >;

	MGR_DECLARE_SEGMENT_NAME( test_segment, "test segment" );

	typedef memory_mgr::shared_segment< memmgr_type, MGR_SEGMENT_NAME(test_segment) > shared_mgr_type;

	typedef boost::mpl::list< shared_mgr_type > managers_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( shared_segment_alloc_dealloc, mgr_type, managers_list )
	{
		typedef mgr_type::offset_type offset_type;
		typedef memory_mgr::manager_traits<mgr_type>::size_type	size_type;

		const offset_type inv_off = memory_mgr::offset_traits<offset_type>::invalid_offset;
		const size_type obj_size = 4;

		mgr_type mgr;
		BOOST_CHECK( mgr.is_free() );

		offset_type p1, p2, p3, p4, p5;
		{
			mgr_type mgr2;
			BOOST_CHECK( mgr2.is_free() );

			p1 = mgr2.allocate( obj_size );
			BOOST_CHECK_NE( p1, inv_off );

			p2 = mgr2.allocate( obj_size );
			BOOST_CHECK_NE( p2, inv_off );
			BOOST_CHECK_NE( p1, p2 );

			mgr_type mgr3;
			BOOST_CHECK( !mgr3.is_free() );

			p3 = mgr3.allocate( obj_size );
			BOOST_CHECK_NE( p3, inv_off );

			p4 = mgr3.allocate( obj_size );
			BOOST_CHECK_NE( p4, inv_off );

			p5 = mgr3.allocate( obj_size );
			BOOST_CHECK_NE( p5, inv_off );

			test::check_pointers( p1, p2, p3, p4, p5 );
			mgr2.deallocate( p3, obj_size );
			mgr3.deallocate( p1, obj_size );
		}
		mgr.deallocate( p2, obj_size );
		mgr.deallocate( p4, obj_size );
		mgr.deallocate( p5, obj_size );

		BOOST_CHECK( mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, mgr_type, managers_list )
	{
		test::test_null_pointer_dealloc_seg<mgr_type>();
	}
BOOST_AUTO_TEST_SUITE_END();



