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


#include "memory-mgr/memory_manager.h"
#include "memory-mgr/shared_segment.h"
#include "memory-mgr/size_tracking.h"
#include "memory-mgr/low_fragmentation_manager.h"
#include "memory-mgr/named_objects.h"
#include "common_manager_tests.h"

namespace
{
	typedef unsigned int chunk_type;
	static const size_t chunk_size = 4;
	static const size_t memory_size = 10 * 4 * 1024;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
}

template class memory_mgr::shared_segment< memmgr_type >;

BOOST_AUTO_TEST_SUITE( test_shared_segment )

	MGR_DECLARE_SEGMENT_NAME( test_segment, "test segment" );

typedef memory_mgr::shared_segment< memmgr_type, MGR_SEGMENT_NAME(test_segment) > shared_mgr_type;
typedef memory_mgr::size_tracking<shared_mgr_type > sz_shared_mgr_type;
	typedef memory_mgr::low_fragmentation_manager< memory_mgr::named_objects<sz_shared_mgr_type> > lfm_shared_mgr_type;

	typedef boost::mpl::list< shared_mgr_type, lfm_shared_mgr_type > managers_list;

#if 1
	BOOST_AUTO_TEST_CASE_TEMPLATE( shared_segment_alloc_dealloc, mgr_type, managers_list )
	{
		typedef memory_mgr::manager_traits<mgr_type> 	traits_type;
		typedef typename traits_type::size_type		size_type;

		const size_type obj_size = 4;

		mgr_type mgr;
		//BOOST_CHECK( mgr.is_free() );

		void* null = NULL;
		void* p1;
		void* p2;
		void* p3;
		void* p4;
		void* p5;
		{
			mgr_type mgr2;
			//BOOST_CHECK( mgr2.is_free() );

			p1 = mgr.allocate( obj_size );
			BOOST_CHECK_NE( p1, null );

			p2 = mgr2.allocate( obj_size );
			BOOST_CHECK_NE( p2, null );
			BOOST_CHECK_NE( p1, p2 );

			{
				mgr_type mgr3;
				//BOOST_CHECK( !mgr3.is_free() );

				p3 = mgr3.allocate( obj_size );
				BOOST_CHECK_NE( p3, null );

				p4 = mgr3.allocate( obj_size );
				BOOST_CHECK_NE( p4, null );

				p5 = mgr3.allocate( obj_size );
				BOOST_CHECK_NE( p5, null );

				mgr3.deallocate( p3, obj_size );
				mgr3.deallocate( p4, obj_size );
				mgr3.deallocate( p5, obj_size );
			}

			test::check_pointers( p1, p2, p3, p4, p5 );
			mgr2.deallocate( p2, obj_size );
		}
		mgr.deallocate( p1, obj_size );
	}
#endif

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, mgr_type, managers_list )
	{
		test::test_null_pointer_dealloc_seg<mgr_type>();
	}
BOOST_AUTO_TEST_SUITE_END();



