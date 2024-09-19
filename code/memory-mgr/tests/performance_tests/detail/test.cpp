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

#include "test.h"

#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>

progress_bar::progress_bar( long double value, long double max_value, const size_t bar_length )
:m_bar( 0 )
{
	if( is_null( max_value ) && is_null( value ) )
	{
		m_percent = 1.;
	}
	else if( is_null( max_value ) )
	{
		throw std::runtime_error( "progress_bar: max value must be greater then null" );
	}
	else
	{
		m_percent = value/max_value;
	}

	m_bar = static_cast<size_t>( m_percent * bar_length );
}

void perf_test_manager::print_entry( const test_entry_type& entry )
{
	std::wcout << L"Full time: " << std::fixed << entry.first
		<< L"\tRepeat count: " << entry.second
		<< L"\tOperation time: "<< std::fixed << entry.first / entry.second << std::endl ;
}

void perf_test_manager::add_result( const string_type& category_name, const string_type& test_name, long double test_time, size_t count )
{
	m_test_results[category_name][test_name].push_back( std::make_pair( test_time, count ) );
}

std::string get_current_time_as_string() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &currentTime); // Windows-specific
#else
    localtime_r(&currentTime, &localTime); // POSIX-compliant (Linux, macOS)
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void perf_test_manager::print_results()
{
	if( !m_test_results.empty() )
	{
		m_results_printed = true;

		string_type date_time_stamp;
		{



			//  std::put_time(std::localtime(&t), "%c %Z")
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);
			// std::tm tm = *std::gmtime(&in_time_t);

			std::stringstream date_time;
			date_time << std::put_time(std::localtime(&in_time_t), "%c %Z");
			// date_time << 1900 + tm.tm_year << '.'
			// 	<< tm.tm_mon + 1 << '.'
			// 	<< tm.tm_mday << ' '
			// 	<< tm.tm_hour << ';'
			// 	<< tm.tm_min << ';'
			// 	<< tm.tm_sec;
			date_time_stamp = date_time.str();
		}


		std::wcout << L"\nTesting results: " << date_time_stamp.c_str() << L"\n";
		typedef test_results_type::iterator res_iter_type;
		cmp_test_series cmp_results;
		string_type results_file_path = memory_mgr::osapi::get_exe_dir().c_str();
		memory_mgr::helpers::add_trailing_slash( results_file_path );
		results_file_path += "perf_test_results ";

		results_file_path += date_time_stamp;
		results_file_path += ".txt";
		std::ofstream result_file(  results_file_path.c_str()  );

		//Print header to file
		result_file << "Test name:\tTime:\tGraph:\n";

		typedef test_results_type::iterator test_cat_iter_type;
		typedef test_named_results_type::iterator named_test_iter_type;

		for( test_cat_iter_type cat_test = m_test_results.begin(); cat_test != m_test_results.end(); ++cat_test )
		{
			string_type category_name = cat_test->first;
			test_named_results_type& test = cat_test->second;

			cmp_results.clear();
			for( named_test_iter_type test_res = test.begin(); test_res != test.end(); ++test_res )
			{
				string_type test_name = test_res->first;
				test_series& tests = test_res->second;
				//typedef test_series::const_iterator test_iter;
				//std::wcout << L"Test '" << test_name.c_str() << L"'\n";

				//Sort test series by time (first-->the fastest)
				std::sort( tests.begin(), tests.end() );
				//print test with best result
				//print_entry( *tests.begin() );

				//add test with best result to independent storage
				cmp_results.push_back( std::make_pair( test_name, tests.begin()->first / tests.begin()->second ) );
			}

			if( !cmp_results.empty() )
			{
				size_t fill_count = graph_length;
				//Print category name
				std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), fill_count, L'-' );
				std::wcout << L"\nCategory: " << category_name.c_str() << L"\n";
				result_file << "Category: " << category_name << "\n";

				std::sort( cmp_results.begin(), cmp_results.end() );
				const long double max_val = std::max_element( cmp_results.begin(), cmp_results.end(), &less_second<string_type, long double> )->second;
				typedef cmp_test_series::const_iterator cmp_iter_type;
				for( cmp_iter_type cmp = cmp_results.begin(); cmp != cmp_results.end(); ++cmp )
				{
					auto pb = progress_bar( cmp->second, max_val, graph_length );
					std::wcout << L"Test '" << cmp->first.c_str() << L"' time:" << cmp->second << L"\n";
					std::wcout << std::left << pb << L"\n";

					result_file << cmp->first << "\t" << std::left << pb << "\t( " << cmp->second << " )\n";
				}
				result_file << "\n\n";
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

