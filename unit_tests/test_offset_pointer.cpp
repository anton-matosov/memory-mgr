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
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/offset_ptr.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/new.h>


// namespace
// {
// 	typedef memory_mgr::singleton_manager
// 	< 
// 		memory_mgr::size_tracking
// 		<
// 			memory_mgr::pointer_convert
// 			< 
// 				memory_mgr::heap_segment
// 				< 
// 					memory_mgr::memory_manager<size_t, 1024 * 1024, 4> 
// 				> 
// 			>
// 		>
// 	> sz_heap_mgr;
// 
// 
// }

MGR_DECLARE_MANAGER_CLASS( ptr_mem_mgr, def_heap_mgr );

template class memory_mgr::offset_ptr< builtin_type >;
template class memory_mgr::offset_ptr< base_test_class >;
template class memory_mgr::offset_ptr< derived_test_class >;

BOOST_AUTO_TEST_SUITE( test_offset_pointer )
	
	typedef memory_mgr::offset_ptr< builtin_type > builtin_ptr;
	typedef memory_mgr::offset_ptr< base_test_class > base_class_ptr;
	typedef memory_mgr::offset_ptr< derived_test_class > derived_class_ptr;

	typedef boost::mpl::list< builtin_ptr, base_class_ptr, derived_class_ptr > pointer_types;


	BOOST_AUTO_TEST_CASE( test_indirect_self_get )
	{
		struct self_link
		{
			typedef memory_mgr::offset_ptr<self_link> link_type;
			link_type link_;

			static void set_link( link_type node, link_type link )
			{
				node->link_ = link;
			}
		};
		self_link::link_type head = new( mem_mgr<ptr_mem_mgr>() ) self_link();


		self_link::set_link( head, head );

		*head->link_;

	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_null_ptr, ptr_type, pointer_types )
	{
		ptr_type null_ptr;

		BOOST_CHECKPOINT( "before deletion of null ptr" );
		delete_<ptr_mem_mgr>( null_ptr );
		BOOST_CHECKPOINT( "after deletion of null ptr" );


		BOOST_CHECKPOINT( "before deletion of null array" );
		delete_array<ptr_mem_mgr>( null_ptr );
		BOOST_CHECKPOINT( "after deletion of null array" );
	}

	BOOST_AUTO_TEST_CASE( test_dereferencing )
	{
		derived_class_ptr dptr( new derived_test_class() );
		base_class_ptr ptr = dptr;
		const base_class_ptr cptr = dptr;

		const int TestVal = 1;
		const int TestVal2 = 2;

		////TEST_OPERATOR_PRINT( L"->" );
		ptr->Set( TestVal );
		BOOST_CHECK_EQUAL( cptr->Get(), TestVal );

		////TEST_OPERATOR_PRINT( L"*" );
		(*dptr).Set( TestVal2 );
		BOOST_CHECK_EQUAL( (*cptr).Get(), TestVal2 );
		BOOST_CHECK_EQUAL( &*ptr, &*dptr );
		BOOST_CHECK_EQUAL( &*ptr, &*cptr );
		BOOST_CHECK_EQUAL( &*cptr, &*dptr );

		////TEST_OPERATOR_PRINT( L"[]" );
		dptr[0].Set( TestVal );
		BOOST_CHECK_EQUAL( cptr[0].Get(), TestVal );
		BOOST_CHECK_EQUAL( &ptr[0], &dptr[0] );
		BOOST_CHECK_EQUAL( &ptr[0], &cptr[0] );
		BOOST_CHECK_EQUAL( &cptr[0], &dptr[0] );



		delete_<ptr_mem_mgr>( ptr );// points to derived_ptr
		//BOOST_CHECK( ptr_mem_mgr::instance().is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_construction )
	{
		//TEST_PRINT( L"Creating DerivedTestClass()" );
		derived_class_ptr derived_ptr( new derived_test_class() );

		//TEST_PRINT( L"Creating DerivedTestClass(1)" );
		derived_class_ptr derived_ptr2( new derived_test_class(1) );

		//TEST_PRINT( L"Creating builtin_type()" );
		builtin_ptr ptr1( new( mem_mgr<ptr_mem_mgr>() ) builtin_type() );

		//TEST_PRINT( L"Constructing base_ptr from derived_ptr" );
		base_class_ptr base_ptr( derived_ptr );
		base_class_ptr base_ptr2;

		BOOST_CHECK_MESSAGE( derived_ptr != derived_ptr2, L"Pointers are the same" );

		////TEST_OPERATOR_PRINT( L"=, base_ptr2 = base_ptr" );
		base_ptr2 = base_ptr;
		BOOST_CHECK( base_ptr2 == base_ptr );

		base_class_ptr base_ptr3;

		////TEST_OPERATOR_PRINT( L"=, base_ptr3 = base_ptr2" );
		base_ptr3 = base_ptr2;
		BOOST_CHECK( base_ptr2 == base_ptr3 );

		base_class_ptr base_ptr4;

		////TEST_OPERATOR_PRINT( L"=, base_ptr4 = base_ptr2" );
		base_ptr4 = derived_ptr2;
		BOOST_CHECK( base_ptr4 == derived_ptr2  );

		delete_<ptr_mem_mgr>( base_ptr );// points to derived_ptr
		delete_<ptr_mem_mgr>( base_ptr4 );// points to derived_ptr2

		delete_<ptr_mem_mgr>( ptr1 );
		//BOOST_CHECK( ptr_mem_mgr::instance().is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_operators )
	{
		//SUBTEST_START( L"operators" );	
		builtin_ptr ptr( new( mem_mgr<ptr_mem_mgr>() ) builtin_type[5] );


		//TEST_OPERATOR_PRINT( L"+, non method" );
		builtin_ptr ptr1 = 1 + ptr;
		//TEST_OPERATOR_PRINT( L"+, method" );
		builtin_ptr ptr2 = ptr + 2;
		//TEST_OPERATOR_PRINT( L"[]" );
		builtin_ptr ptr3 = &ptr[3];
		builtin_ptr ptr4 = ptr;//0

		BOOST_CHECK( ptr1 != ptr2 );;
		BOOST_CHECK( ptr2 != ptr3 );
		BOOST_CHECK( ptr3 != ptr4 );
		BOOST_CHECK( ptr1 != ptr4 );
		BOOST_CHECK( ptr2 != ptr4 );

		//TEST_OPERATOR_PRINT( L"<, ptr1 < ptr2" );
		BOOST_CHECK( ptr1 < ptr2 );
		//TEST_OPERATOR_PRINT( L">, ptr3 > ptr2" );
		BOOST_CHECK( ptr3 > ptr2 );
		//TEST_OPERATOR_PRINT( L">=, ptr3 >= ptr3" );
		BOOST_CHECK( ptr3 >= ptr3 );
		//TEST_OPERATOR_PRINT( L">=, ptr3 >= ptr2" );
		BOOST_CHECK( ptr3 >= ptr2 );
		//TEST_OPERATOR_PRINT( L"<=, ptr1 <= ptr2" );
		BOOST_CHECK( ptr1 <= ptr2 );
		//TEST_OPERATOR_PRINT( L"<=, ptr <= ptr4" );
		BOOST_CHECK( ptr <= ptr4 );

		//TEST_OPERATOR_PRINT( L"++, ptr4" );
		++ptr4;//1
		BOOST_CHECK( ptr1 == ptr4 );

		//TEST_OPERATOR_PRINT( L"++, postfix, ptr4" );
		ptr4++;//2
		BOOST_CHECK( ptr2 == ptr4 );

		//TEST_OPERATOR_PRINT( L"+=, ptr4 += 1" );
		ptr4 += 1;//3
		BOOST_CHECK( ptr3 == ptr4 );

		//TEST_OPERATOR_PRINT( L"+, method, ptr4 + 1" );
		ptr4 = ptr4 + 1;//4
		BOOST_CHECK( ptr3 + 1 == ptr4 );

		//TEST_OPERATOR_PRINT( L"--, ptr4" );
		--ptr4;//3
		BOOST_CHECK( ptr3 == ptr4 );

		//TEST_OPERATOR_PRINT( L"--, postfix, ptr4" );
		ptr4--;//2
		BOOST_CHECK( ptr2 == ptr4 );

		//TEST_OPERATOR_PRINT( L"-=, ptr4 -= 1" );
		ptr4 -= 1;//1
		BOOST_CHECK( ptr1 == ptr4 );

		//TEST_OPERATOR_PRINT( L"-, ptr4 - 1" );
		ptr4 = ptr4 - 1;//0
		BOOST_CHECK( ptr == ptr4 );

		//TEST_OPERATOR_PRINT( L"!=, ptr1 != ptr2" );
		BOOST_CHECK( ptr1 != ptr2 );
		//TEST_OPERATOR_PRINT( L"!=, ptr2 != ptr3" );
		BOOST_CHECK( ptr2 != ptr3 );
		//TEST_OPERATOR_PRINT( L"!=, ptr3 != ptr4" );
		BOOST_CHECK( ptr3 != ptr4 );
		//TEST_OPERATOR_PRINT( L"!=, ptr1 != ptr4" );
		BOOST_CHECK( ptr1 != ptr4 );
		//TEST_OPERATOR_PRINT( L"!=, ptr2 != ptr4" );
		BOOST_CHECK( ptr2 != ptr4 );

		//TEST_OPERATOR_PRINT( L"<, ptr1 < ptr2" );
		BOOST_CHECK( ptr1 < ptr2 );
		//TEST_OPERATOR_PRINT( L">, ptr3 > ptr2" );
		BOOST_CHECK( ptr3 > ptr2 );
		//TEST_OPERATOR_PRINT( L">=, ptr3 >= ptr3" );
		BOOST_CHECK( ptr3 >= ptr3 );
		//TEST_OPERATOR_PRINT( L">=, ptr3 >= ptr2" );
		BOOST_CHECK( ptr3 >= ptr2 );
		//TEST_OPERATOR_PRINT( L"<=, ptr1 <= ptr2" );
		BOOST_CHECK( ptr1 <= ptr2 );
		//TEST_OPERATOR_PRINT( L"<=, ptr <= ptr4" );
		BOOST_CHECK( ptr <= ptr4 );

		//TEST_OPERATOR_PRINT( L"-, ptr1 - ptr" );
		BOOST_CHECK( (ptr1 - ptr) == 1 );

		//TEST_OPERATOR_PRINT( L"-, ptr2 - ptr1" );
		BOOST_CHECK( (ptr2 - ptr1) == 1 );

		//TEST_OPERATOR_PRINT( L"-, ptr3 - ptr2" );
		BOOST_CHECK( (ptr3 - ptr2) == 1 );

		//TEST_OPERATOR_PRINT( L"-, ptr3 - ptr1" );
		BOOST_CHECK( (ptr3 - ptr1) == 2 );

		//TEST_OPERATOR_PRINT( L"-, ptr3 - ptr" );
		BOOST_CHECK( (ptr3 - ptr) == 3 );

		//TEST_OPERATOR_PRINT( L"-, ptr2 - ptr3" );
		BOOST_CHECK( (ptr2 - ptr3) == -1 );

		//TEST_OPERATOR_PRINT( L"-, ptr1 - ptr3" );
		BOOST_CHECK( (ptr1 - ptr3) == -2 );

		//TEST_OPERATOR_PRINT( L"-, ptr - ptr3" );
		BOOST_CHECK( (ptr - ptr3) == -3 );

		//TEST_OPERATOR_PRINT( L"-, (ptr3 - ptr2) == (ptr2 - ptr1)" );
		BOOST_CHECK( (ptr3 - ptr2) == (ptr2 - ptr1) );

		//TEST_OPERATOR_PRINT( L"-, (ptr3 - ptr1) > (ptr2 - ptr1)" );
		BOOST_CHECK( (ptr3 - ptr1) > (ptr2 - ptr1) );

		//TEST_OPERATOR_PRINT( L"-, (ptr1 - ptr) == (ptr3 - ptr2)" );
		BOOST_CHECK( (ptr1 - ptr) == (ptr3 - ptr2) );

		delete_array<ptr_mem_mgr>( ptr );// points to derived_ptr
		//BOOST_CHECK( ptr_mem_mgr::instance().is_free() );

	}
BOOST_AUTO_TEST_SUITE_END();
