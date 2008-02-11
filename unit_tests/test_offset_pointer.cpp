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

class DerivedTestClass : public test_class
{	
};

typedef int builtin_type;

typedef def_heap_mgr pointers_memory_mgr;
typedef memory_mgr::offset_pointer< builtin_type, pointers_memory_mgr > builtin_ptr;
typedef memory_mgr::offset_pointer< test_class, pointers_memory_mgr > base_class_ptr;
typedef memory_mgr::offset_pointer< DerivedTestClass, pointers_memory_mgr > derived_class_ptr;

template class memory_mgr::offset_pointer< builtin_type, pointers_memory_mgr >;
//template class memory_mgr::offset_pointer< test_class, pointers_memory_mgr >;
//template class memory_mgr::offset_pointer< DerivedTestClass, pointers_memory_mgr >;


bool test_construction()
{
	SUBTEST_START( L"construction/destruction" );	
	//using memory_mgr::object_name;
	derived_class_ptr derived_ptr( new/*(object_name(L"Derived"))*/ DerivedTestClass() );
	base_class_ptr base_ptr( derived_ptr );

	SUBTEST_END( pointers_memory_mgr::instance().free() );
}


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

bool test_offset_pointer()
{
	int* pi = ::new( mem_mgr(sz_heap_mgr::instance()) ) int;
	pi;

	TEST_START( L"offset_pointer" );
	
	TEST_END( test_construction()/*
//		 &&
//				test_size_tracking() &&
//				test_out_of_memory() &&
//				test_managed_base()*/
		);
}
