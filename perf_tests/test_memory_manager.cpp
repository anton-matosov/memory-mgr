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

#include <iostream>
#include "perf_timer.h"
#include "memory_manager.h"
#include "heap_segment.h"
#include "new.h"
#include "size_tracking.h"
#include "detail/test.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4
;
static const size_t memory_size = 32 * 1024 * 1024;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

template class memory_mgr::memory_segment< memory_mgr::vector_as_allocator, memmgr_type >;

//typedef memory_mgr::memory_segment< memory_mgr::vector_as_allocator, memmgr_type > segmmgr_type;
typedef memmgr_type::offset_type offset_type;

typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memory_mgr::pointer_convert
		< 
			memory_mgr::memory_manager<size_t, memory_size, chunk_size> 
		> 
	>
> segmmgr_type;

#include "shared_segment.h"
#include "managed_base.h"
#include "singleton_manager.h"


typedef memory_mgr::singleton_manager
< 
	memory_mgr::shared_segment
	< 
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memory_mgr::memory_manager<size_t, memory_size, chunk_size> 
			> 
		>
	>
> def_heap_mgr;

// typedef memory_mgr::singleton_manager
// < 
// 	memory_mgr::heap_segment
// 	< 
// 		memory_mgr::pointer_convert
// 		< 
// 			memory_mgr::memory_manager<size_t, memory_size, chunk_size> 
// 		> 
// 	>
// > def_heap_mgr;

class test_class: public memory_mgr::managed_base< def_heap_mgr >
{
	int m_i;
};

class test_class2
{
	int m_i;
};

// template<class MemMgr>
// void run_test( const int count )
// {	
// 	memory_mgr::perf_timer timer;
// 	int i = count;
// 	MemMgr mgr;
// 	
// 	timer.start();
// 	while( --i )
// 	{
// 		mgr.allocate( chunk_size );		
// 	}
// 	timer.stop();
// 
// 	std::wcout << L"Resolution = " << std::fixed << timer.resolution_ms() << L"\n";
// 	std::wcout << count << L" allocations time = " << std::fixed << timer.elapsed_ms() << L"\n";
// 	std::wcout << L"One allocation time = " << std::fixed << timer.elapsed_ms() / count << L"\n";
// 	return timer.elapsed_ms();
// }


MGR_DECLARE_TEST( mgr.allocate( chunk_size ), alloc_test );
MGR_DECLARE_TEST( new( mem_mgr(mgr) ) int, mgr_new_int_test );
MGR_DECLARE_TEST( HeapAlloc( GetProcessHeap(), 0, chunk_size), heap_test );
MGR_DECLARE_TEST( new int, heap_new_int_test );

bool test_memory_manager()
{	
	const int count = 500000;
	memory_mgr::perf_timer timer;

	std::wcout << L"Items count: " << count << L"\n";
	std::wcout << L"Memory size: " << memory_mgr::manager_traits<segmmgr_type>::memory_size << L"\n";
	std::wcout << L"Required memory: " << count * chunk_size << L"\n";
	std::wcout << L"Timer resolution: " << std::fixed << timer.resolution_mcs() << L" mcs\n";

	MGR_RUN_TEST( alloc_test, segmmgr_type, count, 5 );
	MGR_RUN_TEST( mgr_new_int_test, segmmgr_type, count, 5 );
	MGR_RUN_TEST( heap_test, segmmgr_type, count, 5 );
	MGR_RUN_TEST( heap_new_int_test, segmmgr_type, count, 5 );
	
 	return false;
}
