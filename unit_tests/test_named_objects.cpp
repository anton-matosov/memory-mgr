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
#include <memory-mgr/offset_pointer.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/singleton_manager.h>

typedef 
//memory_mgr::singleton_manager
//< 
	memory_mgr::named_objects
	<
		memory_mgr::size_tracking
		<
			memory_mgr::pointer_convert
			< 
				memory_mgr::shared_segment
				< 
					memory_mgr::memory_manager<size_t, 1024 * 1024, 4> 
				> 
			>
		>
	>
//> 
name_sz_pt_heap_mgr_type;

MGR_DECLARE_MANAGER_CLASS( name_sz_pt_heap_mgr, name_sz_pt_heap_mgr_type );

BOOST_AUTO_TEST_SUITE( test_named_objects )

typedef boost::mpl::list< name_sz_pt_heap_mgr > managers_list;

namespace
{
	const char* name1 = "name1";
	const char* name2 = "name2";
}

BOOST_AUTO_TEST_CASE_TEMPLATE( alloc_dealloc, mgr_type, managers_list )
{
	typedef mgr_type::offset_type offset_type;
	typedef memory_mgr::manager_traits<mgr_type>::size_type	size_type;

	const offset_type inv_off = memory_mgr::offset_traits<offset_type>::invalid_offset;
	const size_type obj_size = 4;

	mgr_type mgr1;
	//BOOST_CHECK( mgr1.is_free() );

	offset_type p1, p2, p11, p22;

	BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), false );
	p1 = mgr1.allocate( obj_size, name1 );
	BOOST_CHECK_NE( p1, inv_off );
	BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), true );
	
	BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), false );
	p2 = mgr1.allocate( obj_size, name2 );
	BOOST_CHECK_NE( p2, inv_off );
	BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), true );

	p11 = mgr1.allocate( obj_size, name1 );
	BOOST_CHECK_NE( p11, inv_off );

	p22 = mgr1.allocate( obj_size, name2 );
	BOOST_CHECK_NE( p22, inv_off );

	BOOST_CHECK_EQUAL( p1, p11 );
	BOOST_CHECK_EQUAL( p2, p22 );
	BOOST_CHECK_NE( p1, p22 );
	BOOST_CHECK_NE( p2, p11 );

	mgr1.deallocate( p11, obj_size );
	BOOST_CHECK_EQUAL( mgr1.is_exists( name1 ), false );
	mgr1.deallocate( p22, obj_size );
	BOOST_CHECK_EQUAL( mgr1.is_exists( name2 ), false );

	//BOOST_CHECK( mgr1.is_free() );
}

// BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete, mgr_type, managers_list )
// {
// 	mgr_type mgr1;
// 
// 
// 	mgr_type mgr2;
// }

BOOST_AUTO_TEST_SUITE_END();








