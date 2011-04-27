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
#include "test_class.h"
#include "managers.h"
#include <memory-mgr/new.h>
#include <memory-mgr/named_objects.h>

typedef 
memory_mgr::singleton_manager
< 
	memory_mgr::named_objects
	<
		heap_sz_mgr
	>
>	sing_name_heap_mgr_type;

MGR_DECLARE_MANAGER_CLASS( sing_name_heap_mgr, sing_name_heap_mgr_type );


BOOST_AUTO_TEST_SUITE( test_new )

	const char* name1 = "name1";
	const char* name2 = "name2";

	const char* name1_arr = "name1 arr";
	const char* name2_arr = "name2 arr";

	typedef int builtin_type;

	typedef boost::mpl::list< sing_name_heap_mgr > managers_list;

	using memory_mgr::new_;

	BOOST_AUTO_TEST_CASE_TEMPLATE( new_delete, mgr_type, managers_list )
	{
		typename memory_mgr::manager_traits< mgr_type >::base_manager_type& mgr = mgr_type::instance();
		using memory_mgr::mem_mgr;

		int* p1 = new_<int>( mgr, "" )();
		const int* p2 = new_<int>( mgr )( 2 );

		int* p3 = new_<int, mgr_type>()();
		const int* p4 = new_<int, mgr_type>()( 4 );

		int* p5 = new_<int>( mgr )( 5 );

		const int* arr_ptr = new_<int, mgr_type>()[5]();
		const void* void_arr_ptr = new_<int, mgr_type>()[15]( 12345 );
		const test_class* class_arr_ptr = new_<test_class, mgr_type>()[15]( 12345, 123, *p1 );

		test::check_pointers( p1, p2, p3, p4, p5 );

		delete_( p1, mem_mgr(mgr) );
		delete_( p2, mgr );
		delete_( p3, mem_mgr<mgr_type>() );
		delete_<mgr_type>( p4 );
		delete_( p5, mgr );

		//const int* p4 = mgr_type::new_<int>()( 4 );
		//mgr_type::delete_( p4 );

// 		class memory_object
// 		{
// 			offset_ptr<char> name_;
// 			size_t objectSize_;
// 		};
 		struct size_and_flags
 		{
 			enum flags{ unnamed = 0, named = 1, size_tracked = 1 << 1 };
 			flags m_flags: 3;
 			size_t m_size: 29;
 		};

		::delete_array( arr_ptr, mem_mgr(mgr) );
		::delete_array<mgr_type>( void_arr_ptr );
		::delete_array<mgr_type>( class_arr_ptr );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_delete_from_base, mgr_type, managers_list )
	{
		const base_test_class* base_ptr = new_<derived_test_class, mgr_type>()( 2 );

		::delete_<mgr_type>( base_ptr );
		BOOST_REQUIRE_MESSAGE( (! memory_mgr::is_category_supported<
			mgr_type, memory_mgr::memory_debugging_tag>::value),
			"Deletion validation can't be performed when memory debbuging is enabled" );
		
		int* base_class_data = ((int*)base_ptr) + 1 /*vmptr_ size*/;
		BOOST_CHECK_EQUAL( *base_class_data, 0xB );

		//Derived class data
		BOOST_CHECK_EQUAL( *(base_class_data + 1), 0xD );
		BOOST_CHECK_EQUAL( *(base_class_data + 2), 0xD );

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
			int* p =  new_<int, mgr_type>()();
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
			int* p = new_<int, mgr_type>()[3]();
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

#if 0 //Named new is no longer supported
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

		//delete_named( ptr11, mem_mgr<mgr_type>() );
		delete_( ptr2, mem_mgr<mgr_type>() );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name1 ), false );
		BOOST_CHECK_EQUAL( mgr_type::instance().is_exists( name2 ), false );
	}
#endif

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





