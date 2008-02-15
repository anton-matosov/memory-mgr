/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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
#include "test_case.h"
#include "heap_segment.h"
#include "offset_pointer.h"
#include "test_class.h"
#include "size_tracking.h"
#include "offset_pointer.h"

#include "new.h"



typedef int builtin_type;



typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	<
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memory_mgr::memory_manager<size_t, 1024 * 1024, 4> 
			> 
		>
	>
> sz_heap_mgr;

typedef sz_heap_mgr ptr_mem_mgr;

class BaseTestClass : public memory_mgr::managed_base<ptr_mem_mgr>
{
	int i_;
public:
	BaseTestClass( int i = 0 )
		:i_(i)
	{}
};

class DerivedTestClass : public BaseTestClass
{
	int i2_;
public:
	DerivedTestClass( int i = 0 )
		:BaseTestClass(i),
		i2_(i + 1)
	{}
};

typedef memory_mgr::offset_pointer< builtin_type, ptr_mem_mgr > builtin_ptr;
typedef memory_mgr::offset_pointer< BaseTestClass, ptr_mem_mgr > base_class_ptr;
typedef memory_mgr::offset_pointer< DerivedTestClass, ptr_mem_mgr > derived_class_ptr;

template class memory_mgr::offset_pointer< builtin_type, ptr_mem_mgr >;
//template class memory_mgr::offset_pointer< test_class, ptr_mem_mgr >;
//template class memory_mgr::offset_pointer< DerivedTestClass, ptr_mem_mgr >;


bool test_construction()
{
	SUBTEST_START( L"construction/destruction" );
	
	derived_class_ptr derived_ptr( new DerivedTestClass() );
	derived_class_ptr derived_ptr2( new DerivedTestClass(1) );
	builtin_ptr ptr1( new( mem_mgr(ptr_mem_mgr::instance()) ) builtin_type() );

	base_class_ptr base_ptr( derived_ptr );
	base_class_ptr base_ptr2;

	TEST_CHECH( derived_ptr != derived_ptr2 );

	base_ptr2 = base_ptr;
	TEST_CHECH( base_ptr2 == base_ptr );

	base_class_ptr base_ptr3;

	base_ptr3 = base_ptr2;
	TEST_CHECH( base_ptr2 == base_ptr3 );

	base_class_ptr base_ptr4;

	base_ptr4 = derived_ptr2;
	TEST_CHECH( base_ptr4 == derived_ptr2 );

 	do_delete( base_ptr );// points to derived_ptr
	do_delete( base_ptr4 );// points to derived_ptr2

	do_delete( ptr1 );
	SUBTEST_END( ptr_mem_mgr::instance().free() );
}

bool test_dereferencing()
{
	SUBTEST_START( L"dereferencing" );	
	derived_class_ptr derived_ptr( new DerivedTestClass[5] );
	base_class_ptr base_ptr( derived_ptr );

	base_class_ptr ptr1 = 1 + base_ptr;
	base_class_ptr ptr2 = base_ptr + 1;

	do_delete_arr( base_ptr );// points to derived_ptr
	SUBTEST_END( ptr_mem_mgr::instance().free() );
}

bool test_operators()
{
	SUBTEST_START( L"operators" );	
	//using memory_mgr::object_name;
	builtin_ptr ptr( new( mem_mgr(ptr_mem_mgr::instance()) ) builtin_type[5] );

	builtin_ptr ptr1 = 1 + ptr;
	builtin_ptr ptr2 = ptr + 2;
	builtin_ptr ptr3 = &ptr[3];
	builtin_ptr ptr4 = ptr;//0
	++ptr4;//1
	TEST_CHECH( ptr1 == ptr4 );
	ptr4++;//2
	TEST_CHECH( ptr2 == ptr4 );
	ptr4 += 1;//3
	TEST_CHECH( ptr3 == ptr4 );
	ptr4 = ptr4 + 1;//4
	TEST_CHECH( ptr3 + 1 == ptr4 );
	--ptr4;//3
	TEST_CHECH( ptr3 == ptr4 );
	ptr4--;//2
	TEST_CHECH( ptr2 == ptr4 );
	ptr4 -= 1;//1
	TEST_CHECH( ptr1 == ptr4 );
	ptr4 = ptr4 - 1;//0
	TEST_CHECH( ptr == ptr4 );

	TEST_CHECH( ptr1 != ptr2 );
	TEST_CHECH( ptr2 != ptr3 );
	TEST_CHECH( ptr3 != ptr4 );
	TEST_CHECH( ptr1 != ptr4 );
	TEST_CHECH( ptr2 != ptr4 );

	TEST_CHECH( ptr1 < ptr2 );
	TEST_CHECH( ptr3 > ptr2 );
	TEST_CHECH( ptr3 >= ptr3 );
	TEST_CHECH( ptr3 >= ptr2 );
	TEST_CHECH( ptr1 <= ptr2 );
	TEST_CHECH( ptr <= ptr4 );

	TEST_CHECH( (ptr1 - ptr) == 1 );
	TEST_CHECH( (ptr2 - ptr1) == 1 );
	TEST_CHECH( (ptr3 - ptr2) == 1 );
	TEST_CHECH( (ptr3 - ptr1) == 2 );
	TEST_CHECH( (ptr3 - ptr) == 3 );
	TEST_CHECH( (ptr2 - ptr3) == -1 );
	TEST_CHECH( (ptr1 - ptr3) == -2 );
	TEST_CHECH( (ptr - ptr3) == -3 );

	TEST_CHECH( (ptr3 - ptr2) == (ptr2 - ptr1) );
	TEST_CHECH( (ptr3 - ptr1) > (ptr2 - ptr1) );
	TEST_CHECH( (ptr1 - ptr) == (ptr3 - ptr2) );

	do_delete_arr( ptr );// points to derived_ptr
	SUBTEST_END( ptr_mem_mgr::instance().free() );

}

bool test_offset_pointer()
{
	int* pi = ::new( mem_mgr(sz_heap_mgr::instance()) ) int;
	do_delete( pi, mem_mgr(sz_heap_mgr::instance() ) );

	TEST_START( L"offset_pointer" );
	
	TEST_END( test_construction()
		&&		test_dereferencing() 
		&&		test_operators() 
//		&&		test_managed_base()*/
		);
}
