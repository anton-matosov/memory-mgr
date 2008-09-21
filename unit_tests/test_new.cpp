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


BOOST_AUTO_TEST_SUITE( test_new )

	const char* name1 = "name1";
	const char* name2 = "name2";

	const char* name1_arr = "name1 arr";
	const char* name2_arr = "name2 arr";

	typedef int builtin_type;

	typedef boost::mpl::list< sing_name_seg_heap_pt_mgg > managers_list;


	BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete, mgr_type, managers_list )
	{
		//int* p = memory_mgr::new_<int, mgr_type>();
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_data_validness, mgr_type, managers_list )
	{
		// 	typedef typename boost::mpl::if_c< 
		// 		is_category_supported< MemMgr, pointer_conversion_tag>::value,
		// 		void*,
		// 		typename manager_traits<MemMgr>::offset_type>::type objects_type;

		typedef std::map<int*, int> ptrval_map_type;
		ptrval_map_type real_vals;

		for( int i = 0; i < 1000; ++i )
		{
			int* p = new( mem_mgr<mgr_type>() ) int();
			BOOST_CHECK( p != 0 );
			int val = rand();
			*p = val;
			BOOST_CHECK( real_vals.find( p ) == real_vals.end() );
			real_vals[p] = val;
		}

		for( ptrval_map_type::const_iterator it = real_vals.begin(); it != real_vals.end(); ++it )
		{
			BOOST_CHECK_EQUAL( *it->first, it->second );
			//mgr.deallocate( memory_mgr::detail::to_offset( it->first, mgr ), sizeof(int) );
			delete_( it->first, mem_mgr<mgr_type>() );
		}
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_data_validness_arr, mgr_type, managers_list )
	{
		// 	typedef typename boost::mpl::if_c< 
		// 		is_category_supported< MemMgr, pointer_conversion_tag>::value,
		// 		void*,
		// 		typename manager_traits<MemMgr>::offset_type>::type objects_type;

		typedef std::map<int*, int> ptrval_map_type;
		ptrval_map_type real_vals;

		for( int i = 0; i < 1000; ++i )
		{
			int* p = new( mem_mgr<mgr_type>() ) int[3];
			BOOST_CHECK( p != 0 );
			int val = rand();
			*p = val;
			BOOST_CHECK( real_vals.find( p ) == real_vals.end() );
			real_vals[p] = val;
		}

		for( ptrval_map_type::const_iterator it = real_vals.begin(); it != real_vals.end(); ++it )
		{
			BOOST_CHECK_EQUAL( *it->first, it->second );
			//mgr.deallocate( memory_mgr::detail::to_offset( it->first, mgr ), sizeof(int) );
			delete_array( it->first, mem_mgr<mgr_type>() );
		}
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete_named, mgr_type, managers_list )
	{
		const builtin_type* null_ptr = 0;
		const builtin_type val_1 = 0xF0F0F0F0;
		const builtin_type val_2 = 0x0F0F0F0F;

		using memory_mgr::object_name;
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), false );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), false );
		builtin_type* ptr1( new( mem_mgr<mgr_type>(), object_name( name1 ) ) builtin_type );
		builtin_type* ptr2( new( mem_mgr<mgr_type>(), object_name( name2 ) ) builtin_type );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), true );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), true );
		
		*ptr1 = val_1;
		*ptr2 = val_2;

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

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, mgr_type, managers_list )
	{
		int* null_ptr = 0;

		BOOST_CHECKPOINT( "before deletion of null ptr" );
		delete_( null_ptr, mem_mgr<mgr_type>() );
		BOOST_CHECKPOINT( "after deletion of null ptr" );


		BOOST_CHECKPOINT( "before deletion of null array" );
		delete_array( null_ptr, mem_mgr<mgr_type>() );
		BOOST_CHECKPOINT( "after deletion of null array" );
	}

BOOST_AUTO_TEST_SUITE_END();





