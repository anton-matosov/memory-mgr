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

#ifndef MGR_TEST_MT_HEADER
#define MGR_TEST_MT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "test.h"
#include <numeric>

#include <boost/thread.hpp>

namespace perf_tests
{
	namespace detail
	{
		struct test_threads_holder
		{
			test_threads_holder( const int op_repeat, const int per_alloc )
				:op_repeat_( op_repeat ),
				per_alloc_( per_alloc )
			{

			}

			template<class FuncT>
			void create_thread( FuncT func )
			{
				threads_.create_thread( boost::bind( func, op_repeat_, per_alloc_,
					boost::ref( results_ ) ) );
			}

			void join_all()
			{
				threads_.join_all();
			}

			std::vector<long double> results_;
			boost::thread_group threads_;

			const int op_repeat_;
			const int per_alloc_;

			long double average_time()
			{
				return std::accumulate( results_.begin(), results_.end(), 0.L ) / results_.size();
			}
		};
	}
}

#define  TEST_START_LOOP_MT( op_repeat_count, alloc_count, pointer_type )\
	::perf_tests::detail::test_threads_holder threads_holder__( op_repeat_count, alloc_count );\
	struct test_thread {\
		static long double do_test( const int op_repeat, const int per_alloc, std::vector<long double>& results__ ) {\
			TEST_START_LOOP( op_repeat, per_alloc, pointer_type );

#define  TEST_START_LOOP_MT_2( op_repeat_count, alloc_count, pointer_type, custom_param )\
	::perf_tests::detail::test_threads_holder threads_holder__( op_repeat_count, alloc_count );\
	struct test_thread {\
		static long double do_test2( const int op_repeat, const int per_alloc, std::vector<long double>& results__, custom_param ) {\
			TEST_START_LOOP( op_repeat, per_alloc, pointer_type );

#define  TEST_END_LOOP_MT\
			TEST_END_LOOP_NO_PRINT\
			static memory_mgr::sync::object_level_lockable<memory_mgr::sync::critical_section> cs__;\
			memory_mgr::sync::object_level_lockable<memory_mgr::sync::critical_section>::lock_type lock__( cs__ );\
			results__.push_back( TEST_ELAPCED_MCS );\
			return TEST_ELAPCED_MCS;\
			} };

#define TEST_START_MT_TESTS_IMPL( num_threads, start_delay, test_func__ )					\
	for( int i = 0; i < num_threads; ++i )													\
	{																						\
		threads_holder__.create_thread( test_func__ );										\
		__pragma(warning(suppress:4127))													\
		if( start_delay )																	\
		{																					\
			boost::this_thread::sleep( boost::posix_time::milliseconds( start_delay ) );	\
		}																					\
	}																						\
	threads_holder__.join_all();

#define TEST_START_MT_TESTS( num_threads, start_delay )\
	TEST_START_MT_TESTS_IMPL( num_threads, start_delay, &test_thread::do_test )

#define TEST_START_MT_TESTS_2( num_threads, start_delay, custom_param )\
	TEST_START_MT_TESTS_IMPL( num_threads, start_delay, (boost::bind( &test_thread::do_test2, _1, _2, _3, boost::ref( custom_param ) ) ) )

#define TEST_ELAPCED_MCS_MT threads_holder__.average_time()


#endif //MGR_TEST_MT_HEADER
