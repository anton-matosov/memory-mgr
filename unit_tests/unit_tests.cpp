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

#include "stdafx.h"
#include <memory-mgr/config/config.h>
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/allocator.h>

#include "unit_tests_manager.h"

bool test_temp_buffer();
bool test_type_manip();
bool test_static_bitset();
bool test_bit_manager();
bool test_memory_manager();
bool test_size_tracking();
bool test_managed_base();
bool test_pointer_convert();
bool test_offset_pointer();
bool test_shared_segment();
bool test_allocator();
bool test_segment_manager();


int main(int /*argc*/, char* /*argv*/[])
{
	unit_tests_manager TestMgr;
		
	std::wcout << L"Unit tests for memory_mgr library\n";
	std::wcout << L"Exe path: " << memory_mgr::osapi::get_executable_path().c_str() << L"\n";
	std::wcout << L"Exe dir: " << memory_mgr::osapi::get_exe_dir().c_str() << L"\n";
	TestMgr.add_result( test_segment_manager(),	L"test_segment_manager" );
 	TestMgr.add_result( test_temp_buffer(), 	L"test_temp_buffer" );
 	TestMgr.add_result( test_type_manip(), 		L"test_type_manip" );
 	TestMgr.add_result( test_static_bitset(), 	L"test_static_bitset" );
 	TestMgr.add_result( test_bit_manager(), 	L"test_bit_manager" );
 	TestMgr.add_result( test_memory_manager(),	L"test_memory_manager" );
 	TestMgr.add_result( test_pointer_convert(),	L"test_pointer_convert" );
 	TestMgr.add_result( test_size_tracking(),	L"test_size_tracking" );
 	TestMgr.add_result( test_managed_base(),	L"test_managed_base" );
 	TestMgr.add_result( test_offset_pointer(),	L"test_offset_pointer" );
 	TestMgr.add_result( test_shared_segment(),	L"test_shared_segment" );
 	TestMgr.add_result( test_allocator(),		L"test_allocator" );

	TestMgr.print_results();
	return std::cin.get();
}
