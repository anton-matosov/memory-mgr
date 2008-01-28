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
#include "simple_ptr.h"
#include "test_class.h"

//class DerivedTestClass : public test_class
//{	
//};
//
//typedef int builtin_type;
//
//
//typedef memory_mgr::def_heap_mgr pointers_memory_mgr;
//typedef memory_mgr::simple_ptr< builtin_type, pointers_memory_mgr > builtin_ptr;
//typedef memory_mgr::simple_ptr< test_class, pointers_memory_mgr > base_class_ptr;
//typedef memory_mgr::simple_ptr< DerivedTestClass, pointers_memory_mgr > derived_class_ptr;
//
//template class memory_mgr::simple_ptr< builtin_type, pointers_memory_mgr >;
////template class memory_mgr::simple_ptr< test_class, pointers_memory_mgr >;
//template class memory_mgr::simple_ptr< DerivedTestClass, pointers_memory_mgr >;
//
//
//bool test_construction()
//{
//	SUBTEST_START( L"construction/destruction" );	
//	using memory_mgr::object_name;
//	derived_class_ptr derived_ptr( new(object_name(L"Derived")) DerivedTestClass() );
//	base_class_ptr base_ptr( derived_ptr );
//
//	SUBTEST_END( pointers_memory_mgr::instance().free() );
//}
//

#include "size_tracking.h"

template
<
	class BlockType, 
	size_t MemorySize,
	size_t ChunkSize, 
	template <class> class PtrT,
	class SyncObj 
>
void* operator new( size_t size, memory_mgr::memory_manager<BlockType,
MemorySize, ChunkSize, PtrT, SyncObj>& mgr )
{
	return mgr.allocate( size ).get_ptr( mgr );
}


bool test_simple_ptr()
{
	int* pi = new(memory_mgr::def_heap_mgr::instance()) int;
	TEST_START( L"simple_ptr" );

	return true;
	
//	TEST_END( test_construction()/*
//		 &&
//				test_size_tracking() &&
//				test_out_of_memory() &&
//				test_managed_base()*/
//		
//		);
}
