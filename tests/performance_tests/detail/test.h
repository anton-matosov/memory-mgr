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

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <limits>
#include <iterator>
#include <memory-mgr/allocator.h>
#include <memory-mgr/detail/helpers.h>
#include <memory-mgr/detail/singleton.h>
#include "../perf_timer.h"

typedef memory_mgr::singleton_manager
< 
	memory_mgr::size_tracking
	<
		memory_mgr::heap_segment
		< 
			memory_mgr::memory_manager<size_t, 35 * 1024 * 1024, 4> 
		> 
	>
> alloc_mgr;

MGR_WRAP_SINGLETON_MANAGER_CLASS(heap_manager, alloc_mgr);

typedef memory_mgr::allocator<char, heap_manager> char_allocator;

//typedef std::basic_string<char, std::char_traits<char>, char_allocator > string_type;
typedef std::string string_type;

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
	long double m_percent;
	progress_bar( long double value, long double max_value, const size_t bar_length );
};

template<class Ch, class Tr>
static inline std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& stream, const progress_bar& bar )
{
	stream.width( static_cast<std::streamsize>( bar.m_bar ) );	
	stream << std::basic_string<Ch, Tr>( bar.m_bar, '|' );
	return stream << ' ' << static_cast<size_t>(bar.m_percent * 100) << '%';
}


class perf_test_manager: public memory_mgr::singleton<perf_test_manager>
{
	typedef std::pair<long double, size_t> test_entry_type;
	typedef std::vector<test_entry_type> test_series;
	typedef std::map< string_type, test_series > test_named_results_type;
	typedef std::map< string_type, test_named_results_type > test_results_type;

	test_results_type m_test_results;
	void print_entry( const test_entry_type& entry );

	typedef std::pair<string_type, long double> cmp_test_entry_type;
	typedef std::vector<cmp_test_entry_type> cmp_test_series;

	bool m_results_printed;

	enum { graph_length = 79 };
public:
	void add_result( const string_type& category_name, const string_type& test_name,
		long double test_time, size_t count );

	void print_results();

	perf_test_manager();
	~perf_test_manager();
};

#define TEST_PRINT_MEM_INFO( op_repeat_count, alloc_count, memmgr_type, object_type )					\
	std::wcout << L"Memory size: " << memory_mgr::manager_traits<memmgr_type>::memory_size << L"\n";	\
	std::wcout << L"Required memory: " << op_repeat_count * alloc_count * (sizeof( object_type )		\
				+ memory_mgr::manager_traits<memmgr_type>::memory_overhead ) << L"\n";


#define  TEST_START_LOOP( op_repeat_count, alloc_count, pointer_type )				\
	memory_mgr::perf_timer timer__;													\
	size_t op_repeat_count__ = op_repeat_count;										\
	size_t loop_outer__ = op_repeat_count__;										\
	size_t alloc_count__ = alloc_count;												\
	size_t full_repeat_count__ = alloc_count__ * op_repeat_count__;					\
	(void)full_repeat_count__;														\
	double accum__ = 0.0;															\
	pointer_type *ptr_storage__ = new pointer_type[alloc_count__ + 1];ptr_storage__;\
	while( loop_outer__-- ){														\
		timer__.start(true);														\
		size_t loop_inner__ = alloc_count__;										\
		while( loop_inner__-- ){

#define TEST_GET_TRACKED_PTR ptr_storage__[loop_inner__]
#define TEST_TRACK_PTR TEST_GET_TRACKED_PTR
#define TEST_ITERATION_NUMBER loop_inner__


#define TEST_SPLIT_LOOP } loop_inner__ = alloc_count__; while( --loop_inner__ ){

#define TEST_SPLIT_LOOP_STOP_TIMER } timer__.stop();{ TEST_SPLIT_LOOP
#define TEST_SPLIT_LOOP_RESTART_TIMER } timer__.stop();timer__.start(true);{ TEST_SPLIT_LOOP

#define  TEST_END_LOOP_NO_PRINT			\
		}								\
		timer__.stop();					\
		accum__ += timer__.elapsed_mcs();\
	}									\
	delete[] ptr_storage__;

#define  TEST_END_LOOP( out_stream )										\
	TEST_END_LOOP_NO_PRINT													\
	out_stream << L"Full time: " << std::fixed << accum__ << L" mcs";		\
	out_stream << L"\tRepeat count: " << alloc_count__;						\
	out_stream << L"\tOperation time: " <<									\
		std::fixed << accum__ * 1000.0 / full_repeat_count__ << L" Ns\n";	\

#define TEST_ACCUMULATE_RESULT accum__ = timer__.elapsed_mcs();
#define TEST_ELAPCED_MCS accum__ / op_repeat_count__

template<class TestOp>
void run_perf_test( const string_type& category_name,
				   const string_type& test_name, TestOp test,
				   const int op_repeat, const int per_alloc, const int test_repeat )
{
	std::wcout << L"\n" << test_name.c_str() << L"\n";
	int i = test_repeat;
	while( i-- )
	{
		perf_test_manager::instance().add_result( category_name, test_name, test( op_repeat, per_alloc ), per_alloc );
	}
}

template<class MemMgr>
void print_perf_test_header( const std::wstring& name,
							const int op_repeat, const int per_alloc, const int test_repeat )
{
	typedef MemMgr memmgr_type;
	memory_mgr::perf_timer timer;

	const size_t fill_count = 60;
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), fill_count, L'=' );
	
	std::wcout << L'\n';
	size_t name_fill = ( fill_count - (name.length() + 4) )/2;
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), name_fill, L'-' );
	std::wcout << L"  " << name << L"  ";
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), name_fill, L'-' );
	std::wcout << L'\n';

	std::wcout << L"Test repeated: " << test_repeat << L"\n";
	std::wcout << L"Allocations repeated: " << op_repeat << L"\n";
	std::wcout << L"Allocated objects: " << per_alloc << L"\n";
	std::wcout << L"Memory size: " << memory_mgr::manager_traits<memmgr_type>::memory_size << L"\n";
	std::wcout << L"Timer resolution: " << std::fixed << timer.resolution_mcs() << L" mcs\n";
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), fill_count, L'=' );

}

#define MGR_PRINT_RESULTS perf_test_manager::instance().print_results();
