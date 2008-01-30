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

template class memory_mgr::offset_pointer<int, def_heap_mgr>;

template
<
	class BlockType, 
	size_t MemorySize,
	size_t ChunkSize, 
	class OffsetType,
	class SyncObj,
	template <class,size_t,size_t,class,class> class MgrType
>
void* operator new( size_t size, memory_mgr::pointer_convert< MgrType<BlockType,
MemorySize, ChunkSize, OffsetType, SyncObj> >& mgr )
{
	return mgr.allocate( size );
}


bool test_offset_pointer()
{
	int* pi = new( def_heap_mgr::instance() ) int;
	TEST_START( L"offset_pointer" );

	memory_mgr::manager_traits<def_heap_mgr>::manager_category tag;

	return true;
	
//	TEST_END( test_construction()/*
//		 &&
//				test_size_tracking() &&
//				test_out_of_memory() &&
//				test_managed_base()*/
//		
//		);
}
