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
#include "config/config.h"
#include "memory_manager.h"
#include "singleton_manager.h"
#include "size_tracking.h"
#include "heap_segment.h"
#include "allocator.h"
#include <deque>
#include <string>

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

typedef memory_mgr::singleton_manager
< 
	memory_mgr::size_tracking
	<
		memory_mgr::pointer_convert
		< 
			memory_mgr::heap_segment
			< 
				memory_mgr::memory_manager<size_t, 2 * 1024 * 1024, 4> 
			> 
		>
	>
> sz_heap_mgr_type;

MGR_DECLARE_MANAGER_CLASS(heap_manager, sz_heap_mgr_type);


typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, memory_mgr::allocator<wchar_t, 
heap_manager> > string_type;

class tests_manager
{
	typedef std::pair< bool, string_type > test_entry_type;
	typedef std::deque< test_entry_type > test_results_type;
	
	test_results_type m_test_results;
public:
	void add_result( bool result, const string_type& test_name )
	{
		m_test_results.push_back( std::make_pair( result, test_name ) );
	}

	void print_results()
	{
		std::wcout << L"\nTesting results:\n";
		test_entry_type test_entry;
		while( !m_test_results.empty() )
		{
			test_entry = m_test_results.front();
			m_test_results.pop_front();
			std::wcout << L"Test '" << test_entry.second << L"'\t" << ( test_entry.first ? L"succeeded" : L"failed" ) << std::endl;
		}
	}

	~tests_manager()
	{
		if( !m_test_results.empty() )
		{
			print_results();
		}
	}
};

int main(int /*argc*/, char* /*argv*/[])
{
	tests_manager TestMgr;
		
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
