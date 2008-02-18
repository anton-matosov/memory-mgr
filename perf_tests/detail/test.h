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

#ifndef MGR_TEST_HEADER
#define MGR_TEST_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "detail/helpers.h"
#include "../perf_timer.h"

template<class T1, class T2>
static inline bool less_second( const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs )
{ return lhs.second < rhs.second; }

class perf_test_manager
{
	typedef std::pair<long double, size_t> test_entry_type;
	typedef std::vector<test_entry_type> test_series;
	typedef std::map< std::string, test_series > test_results_type;

	test_results_type m_test_results;
	void print_entry( const test_entry_type& entry )
	{
		std::wcout << L"Full time: " << std::fixed << entry.first
		 			<< L"\tRepeat count: " << entry.second 
					<< L"\tOperation time: "<< std::fixed << entry.first / entry.second << std::endl ;
	}

	typedef std::pair<std::string, long double> cmp_test_entry_type;
	typedef std::vector<cmp_test_entry_type> cmp_test_series;

	std::wstring progress_bar( long double value, long double max_value, const size_t bar_length )
	{
		return std::wstring( (value/max_value) * bar_length, L'|' );
	}

	bool m_results_printed;
public:
	void add_result( const std::string& test_name, long double test_time, size_t count )
	{
		m_test_results[test_name].push_back( std::make_pair( test_time, count ) );
	}

	void print_results()
	{
		m_results_printed = true;
		std::wcout << L"\nTesting results:\n";
		typedef test_results_type::iterator res_iter_type;
		cmp_test_series cmp_results;
		for( res_iter_type res = m_test_results.begin(); res != m_test_results.end(); ++res )		
		{
			std::string test_name = res->first;
			test_series& tests = res->second;
			typedef test_series::const_iterator test_iter;
			std::wcout << L"Test '" << test_name.c_str() << L"'\n";
			std::sort( tests.begin(), tests.end() );
			print_entry( *tests.begin() );

			cmp_results.push_back( std::make_pair( test_name, tests.begin()->first / tests.begin()->second) );
// 			for( test_iter it = tests.begin(); it != tests.end(); ++it )
// 			{
// 				print_entry( *it );
// 			}

//			m_test_results.erase( m_test_results.begin() );
		}

		std::sort( cmp_results.begin(), cmp_results.end() );
		const long double max_val = std::max_element( cmp_results.begin(), cmp_results.end(), &less_second<std::string, long double> )->second;
		typedef cmp_test_series::const_iterator cmp_iter_type;		
		for( cmp_iter_type cmp = cmp_results.begin(); cmp != cmp_results.end(); ++cmp )
		{
			std::wcout.width( 40 );
			std::wcout << std::left << progress_bar( cmp->second, max_val, 40 ) << " - " << cmp->first.c_str() << L"\n";
		} 
	}

	perf_test_manager()
		:m_results_printed(false)
	{}

	~perf_test_manager()
	{
		if( !m_results_printed )
		{
			print_results();
		}
	}
};

#define MGR_TEST_IMPL( TEST_OPERATION, TEST_ID, BEFORE_LOOP, AFTER_LOOP )\
template<class MemMgr>\
long double run_test_##TEST_ID( const int count )\
{\
	memory_mgr::perf_timer timer;\
	int i = count;\
	BEFORE_LOOP;\
	\
	timer.start();\
	while( --i )\
	{\
		TEST_OPERATION;\
	}\
	timer.stop();\
	AFTER_LOOP;\
	\
	std::wcout << count << L"Full time: " << std::fixed << timer.elapsed_mcs() << L" mcs\n";\
	std::wcout << L"Operation time = " << std::fixed << timer.elapsed_mcs() / count << L" mcs\n";\
	return timer.elapsed_mcs();\
}

#define MGR_DECLARE_TEST( TEST_OPERATION, TEST_ID )\
	MGR_TEST_IMPL( TEST_OPERATION, TEST_ID,	MemMgr mgr, memory_mgr::helpers::do_nothing() )

extern perf_test_manager __test_manager__;
#define MGR_RUN_TEST( ID, MemMgr, op_repeat, test_repaet )\
{\
	const char* test_name = #ID" mgr: "#MemMgr;\
	std::wcout << L"\n" << test_name << L"\n";\
	int i = test_repaet;\
	while( i--)\
	{\
	__test_manager__.add_result( test_name, run_test_##ID<MemMgr>( op_repeat ), op_repeat );\
	}\
}

#define MGR_PRINT_RESULTS __test_manager__.print_results();

#endif
