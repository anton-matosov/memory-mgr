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
#include "managers.h"
#include <memory-mgr/new.h>
#include <memory-mgr/named_objects.h>

typedef 
memory_mgr::singleton_manager
< 
	memory_mgr::named_objects
	<
		seg_heap_sz_pt_mgr
	>
>	sing_name_seg_heap_pt_mgr_type;

MGR_DECLARE_MANAGER_CLASS( sing_name_seg_heap_pt_mgg, sing_name_seg_heap_pt_mgr_type );

typedef int builtin_type;


namespace
{
	const char* name1 = "name1";
	const char* name2 = "name2";

	const char* name1_arr = "name1 arr";
	const char* name2_arr = "name2 arr";
}

BOOST_AUTO_TEST_SUITE( test_new )

	typedef boost::mpl::list< sing_name_seg_heap_pt_mgg > managers_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete, mgr_type, managers_list )
	{
		const builtin_type* null_ptr = 0;
		using memory_mgr::object_name;
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), false );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), false );
		builtin_type* ptr1( new( mem_mgr<mgr_type>(), object_name( name1 ) ) builtin_type );
		builtin_type* ptr2( new( mem_mgr<mgr_type>(), object_name( name2 ) ) builtin_type );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), true );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), true );
		

		BOOST_CHECK_NE( ptr1, null_ptr );
		BOOST_CHECK_NE( ptr2, null_ptr );
		BOOST_CHECK_NE( ptr1, ptr2 );

		builtin_type* ptr11( new( mem_mgr<mgr_type>(), object_name( name1 ) ) builtin_type );
		builtin_type* ptr22( new( mem_mgr<mgr_type>(), object_name( name2 ) ) builtin_type );

		BOOST_CHECK_EQUAL( ptr1, ptr11 );
		BOOST_CHECK_EQUAL( ptr2, ptr22 );
		BOOST_CHECK_NE( ptr11, ptr22 );

		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1_arr ), false );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2_arr ), false );
		builtin_type* arr1( new( mem_mgr<mgr_type>(), object_name( name1_arr ) ) builtin_type[5] );
		builtin_type* arr2( new( mem_mgr<mgr_type>(), object_name( name2_arr ) ) builtin_type[5] );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1_arr ), true );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2_arr ), true );

		BOOST_CHECK_NE( arr1, null_ptr );
		BOOST_CHECK_NE( arr2, null_ptr );
		BOOST_CHECK_NE( arr1, ptr1 );
		BOOST_CHECK_NE( arr1, ptr2 );
		BOOST_CHECK_NE( arr2, ptr1 );
		BOOST_CHECK_NE( arr2, ptr2 );

		builtin_type* arr11( new( mem_mgr<mgr_type>(), object_name( name1_arr ) ) builtin_type[5] );
		builtin_type* arr22( new( mem_mgr<mgr_type>(), object_name( name2_arr ) ) builtin_type[5] );

		BOOST_CHECK_EQUAL( arr1, arr11 );
		BOOST_CHECK_EQUAL( arr2, arr22 );
		BOOST_CHECK_NE( arr11, arr22 );

		delete_( ptr11, mem_mgr<mgr_type>() );
		delete_( ptr2, mem_mgr<mgr_type>() );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), false );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), false );
	}

BOOST_AUTO_TEST_SUITE_END();





