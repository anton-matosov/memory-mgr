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
#include "heap_memory.h"
#include "simple_ptr.h"
#include "TestClass.h"

class DerivedTestClass : public TestClass
{	
};

typedef int builtin_type;


typedef memory_mgr::def_heap_mgr pointers_memory_mgr;
typedef memory_mgr::simple_ptr< builtin_type, pointers_memory_mgr > builtin_ptr;
typedef memory_mgr::simple_ptr< TestClass, pointers_memory_mgr > base_class_ptr;
typedef memory_mgr::simple_ptr< DerivedTestClass, pointers_memory_mgr > derived_class_ptr;

template builtin_ptr;
template base_class_ptr;
template derived_class_ptr;

bool test_construction()
{
	SUBTEST_START( L"construction/destruction" );	
	using memory_mgr::object_name;
	derived_class_ptr derived_ptr( new(object_name(L"Derived")) DerivedTestClass() );
	base_class_ptr base_ptr( derived_ptr );

	SUBTEST_END( pointers_memory_mgr::instance().free() );
}

bool test_simple_ptr()
{

	TEST_START( L"simple_ptr" );

	//sgi_map map;
	//map[1]++;
	TEST_END( test_construction()/*
		 &&
				test_size_tracking() &&
				test_out_of_memory() &&
				test_managed_base()*/
		
		);
}