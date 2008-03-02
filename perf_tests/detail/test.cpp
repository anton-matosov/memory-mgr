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

#include "test.h"
#include <fstream>

void perf_test_manager::print_entry( const test_entry_type& entry )
{
	std::wcout << L"Full time: " << std::fixed << entry.first
		<< L"\tRepeat count: " << entry.second 
		<< L"\tOperation time: "<< std::fixed << entry.first / entry.second << std::endl ;
}

void perf_test_manager::add_result( const std::string& test_name, long double test_time, size_t count )
{
	m_test_results[test_name].push_back( std::make_pair( test_time, count ) );
}

void perf_test_manager::print_results()
{
	if( !m_test_results.empty() )
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
			
			//Sort test series by time (first-->the fastest)
			std::sort( tests.begin(), tests.end() );
			//print test with best result
			print_entry( *tests.begin() );

			//add test with best result to independent storage
			cmp_results.push_back( std::make_pair( test_name, tests.begin()->first / tests.begin()->second) );
		}

		std::ofstream result_file( "" );

		if( !cmp_results.empty() )
		{
			std::sort( cmp_results.begin(), cmp_results.end() );
			const long double max_val = std::max_element( cmp_results.begin(), cmp_results.end(), &less_second<std::string, long double> )->second;
			typedef cmp_test_series::const_iterator cmp_iter_type;		
			for( cmp_iter_type cmp = cmp_results.begin(); cmp != cmp_results.end(); ++cmp )
			{
				std::wcout << L"Test '" << cmp->first.c_str() << L"' time:" << cmp->second << L"\n";	
				std::wcout << std::left << progress_bar( cmp->second, max_val, graph_length ) << L"\n";
			} 
		}
	}
}

perf_test_manager::perf_test_manager() :m_results_printed(false)
{

}

perf_test_manager::~perf_test_manager()
{
	if( !m_results_printed )
	{
		print_results();
	}
}

