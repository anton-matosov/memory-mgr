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
#include <limits>
#include "detail/helpers.h"
#include "detail/singleton.h"
#include "../perf_timer.h"

template<class T1, class T2>
static inline bool less_second( const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs )
{ return lhs.second < rhs.second; }

static inline bool is_null( long double val )
{
	const long double epsilon = 0.00001L;
	return ( val - epsilon ) <= epsilon;
}

struct progress_bar
{
	size_t m_bar;
	progress_bar( long double value, long double max_value, const size_t bar_length )
		:m_bar( 0 )
	{
		if( is_null( max_value ) )
		{
			throw std::runtime_error( "progress_bar: max value must be greater then null" );
		}
		m_bar = static_cast<size_t>( (value/max_value) * bar_length );
	}

	
};

template<class Ch, class Tr>
std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& stream, const progress_bar& bar )
{
	stream.width( bar.m_bar );	
	return stream << std::basic_string<Ch, Tr>( bar.m_bar, '|' );
}


class perf_test_manager: public memory_mgr::singleton<perf_test_manager>
{
	typedef std::pair<long double, size_t> test_entry_type;
	typedef std::vector<test_entry_type> test_series;
	typedef std::map< std::string, test_series > test_results_type;

	test_results_type m_test_results;
	void print_entry( const test_entry_type& entry );

	typedef std::pair<std::string, long double> cmp_test_entry_type;
	typedef std::vector<cmp_test_entry_type> cmp_test_series;

	bool m_results_printed;

	enum { graph_length = 79 };
public:
	void add_result( const std::string& test_name, long double test_time, size_t count );

	void print_results();

	perf_test_manager();
	~perf_test_manager();
};

#define  TEST_START_LOOP( repeate_count )\
	memory_mgr::perf_timer timer__;\
	int loop__ = repeate_count;\
	int repeate_count__ = repeate_count;\
	timer__.start();\
	while( --loop__ ){

#define  TEST_END_LOOP( out_stream )\
	}timer__.stop();\
	out_stream << count << L"Full time: " << std::fixed << timer__.elapsed_mcs() << L" mcs\n";\
	out_stream << L"Operation time = " << std::fixed << timer__.elapsed_mcs() / repeate_count__ << L" mcs\n";\

#define TEST_ELAPCED_MCS timer__.elapsed_mcs();

template<class TestOp>
void run_perf_test( const std::string& test_name, TestOp test, const int op_repeat, const int test_repeat )
{
	std::wcout << L"\n" << test_name.c_str() << L"\n";
	int i = test_repeat;
	while( i-- )
	{
		perf_test_manager::instance().add_result( test_name, test( op_repeat ), op_repeat );
	}
}

#define MGR_PRINT_RESULTS perf_test_manager::instance().print_results();

#endif
