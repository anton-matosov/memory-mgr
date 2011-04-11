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
#include <memory-mgr/named_objects.h>
#include <memory-mgr/shared_segment.h>
//#include <memory-mgr/segment_manager.h>
#include <memory-mgr/offset_ptr.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/detail/offset_traits.h>

typedef  memory_mgr::named_objects
<
	memory_mgr::size_tracking
	<
		memory_mgr::shared_segment
		< 
			memory_mgr::memory_manager<size_t, 1024, 4> 
		>		
	>
> name_sz_shared_mgr_type;

typedef  memory_mgr::named_objects
<
	memory_mgr::shared_segment
	< 
		memory_mgr::memory_manager<size_t, 1024 * 1024, 4> 
	> 
	
> name_shared_mgr_type;


MGR_DECLARE_MANAGER_CLASS( name_sz_shared_mgr, name_sz_shared_mgr_type );
MGR_DECLARE_MANAGER_CLASS( name_shared_mgr, name_shared_mgr_type );

BOOST_AUTO_TEST_SUITE( test_named_objects )

	typedef boost::mpl::list< name_sz_shared_mgr, name_shared_mgr > managers_list;

	const char* name1 = "name1";
	const char* name2 = "name2";


	BOOST_AUTO_TEST_CASE_TEMPLATE( alloc_dealloc, mgr_type, managers_list )
	{
		//typedef mgr_type::object_type object_type;
		typedef typename memory_mgr::manager_traits<mgr_type>::size_type	size_type;

		const void* null = NULL;
		const size_type obj_size = 4;

		mgr_type mgr1;
		//BOOST_CHECK( mgr1.is_free() );

		void* p1_m1;
		void* p1_m2;

		void* p2_m2;
		void* p2_m1;

		BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), false );
		p1_m1 = mgr1.allocate( obj_size, name1 );
		BOOST_CHECK_NE( p1_m1, null );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), true );
		

		mgr_type mgr2;
		BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), false );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name2 ), false );
		p2_m2 = mgr2.allocate( obj_size, name2 );
		BOOST_CHECK_NE( p2_m2, null );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name2 ), true );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), true );

		p1_m2 = mgr2.allocate( obj_size, name1 );
		BOOST_CHECK_NE( p1_m2, null );

		p2_m1 = mgr1.allocate( obj_size, name2 );
		BOOST_CHECK_NE( p2_m1, null );

		//BOOST_CHECK_EQUAL( p1, p11 );
		//BOOST_CHECK_EQUAL( p2, p22 );
		BOOST_CHECK_NE( p1_m1, p2_m1 );
		BOOST_CHECK_NE( p2_m2, p1_m2 );

		//Object points to the first mapped segment, not to the second one that is validated.
		mgr2.deallocate( p1_m2, obj_size, name1 );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name1 ), true );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), true );

		mgr1.deallocate( p1_m1, obj_size, name1 );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name1 ), false );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), false );


		mgr1.deallocate( p2_m1, obj_size, name2 );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), true );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name2 ), true );

		mgr2.deallocate( p2_m2, obj_size, name2 );
		BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), false );
		BOOST_CHECK_EQUAL( mgr2.is_exists( name2 ), false );
		//BOOST_CHECK( mgr1.is_free() );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, mgr_type, managers_list )
	{
		test::test_null_pointer_dealloc_seg<mgr_type>();
	}

// BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete, mgr_type, managers_list )
// {
// 	mgr_type mgr1;
// 
// 
// 	mgr_type mgr2;
// }

BOOST_AUTO_TEST_SUITE_END();








