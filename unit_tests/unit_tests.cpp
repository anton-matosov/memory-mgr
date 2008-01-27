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

#include "stdafx.h"

#include <deque>
#include <string>

bool test_type_manip();
bool test_static_bitset();
bool test_bit_manager();
bool test_memory_manager();
bool test_simple_ptr();

class tests_manager
{
	typedef std::pair< bool, std::wstring > test_entry_type;
	typedef std::deque< test_entry_type > test_results_type;
	
	test_results_type m_test_results;
public:
	void add_result( bool result, const std::wstring& test_name )
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
		print_results();
	}
};

#define RUN_TEST( TestMgr, TestFunc ) TestMgr.add_result( TestFunc(), L"TestFunc" )



int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
	tests_manager TestsMgr;
	
	
	
	RUN_TEST( TestsMgr, test_type_manip );
	RUN_TEST( TestsMgr, test_static_bitset );
	RUN_TEST( TestsMgr, test_bit_manager );
	RUN_TEST( TestsMgr, test_memory_manager );
	RUN_TEST( TestsMgr, test_simple_ptr );

	TestsMgr.print_results();
	return std::cin.get();
}
