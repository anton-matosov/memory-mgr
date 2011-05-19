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
#include <memory-mgr/heap_segment.h>
#include "common_manager_tests.h"


BOOST_AUTO_TEST_SUITE( test_memory_segment )

	typedef unsigned char chunk_type;
	static const size_t chunk_size = 4;
	static const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;


	typedef memory_mgr::memory_segment< memory_mgr::detail::malloc_allocator, memmgr_type > segmmgr_type;

	typedef boost::mpl::list< segmmgr_type > managers_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( alloc_dealloc, mgr_type, managers_list )
	{
		typedef void*	pointer_type;
		typedef typename memory_mgr::manager_traits<mgr_type>::size_type	size_type;

		const size_type obj_size = 4;

		mgr_type mgr;
		BOOST_CHECK( mgr.is_free() );

		
		pointer_type p1 = mgr.allocate( obj_size );
		pointer_type p2 = mgr.allocate( obj_size );
		pointer_type p3 = mgr.allocate( obj_size );
		pointer_type p4 = mgr.allocate( obj_size );
		pointer_type p5 = mgr.allocate( obj_size );


		test::check_pointers( p1, p2, p3, p4, p5 );

		mgr.deallocate( p3, obj_size );
		mgr.deallocate( p5, obj_size );
		mgr.deallocate( p1, obj_size );
		mgr.deallocate( p2, obj_size );
		mgr.deallocate( p4, obj_size );

		BOOST_CHECK( mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, mgr_type, managers_list )
	{
		test::test_null_pointer_dealloc_seg<mgr_type>();
	}
BOOST_AUTO_TEST_SUITE_END();



