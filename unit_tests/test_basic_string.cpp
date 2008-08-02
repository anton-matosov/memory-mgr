/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#include "stdafx.h"

#include <gstl/detail/string.hpp>
#include <memory-mgr/allocator.h>
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/offset_allocator.h>

typedef memory_mgr::heap_segment
< 
	memory_mgr::memory_manager<size_t, 4 * 1024, 4> 
> heap_mgr;

typedef memory_mgr::singleton_manager
< 
	//memory_mgr::size_tracking
	//<
		memory_mgr::pointer_convert
		< 
			heap_mgr 
		>
	//>
> ptr_alloc_mgr;

typedef memory_mgr::singleton_manager
< 
	heap_mgr
> off_alloc_mgr;

typedef gstl::string gstl_string;

typedef gstl::basic_string<char, gstl::char_traits<char>,
	memory_mgr::allocator<char, ptr_alloc_mgr>, gstl::pointer_traits<char> > memory_mgr_string;

typedef gstl::basic_string<char, gstl::char_traits<char>,
	memory_mgr::offset_allocator<char, off_alloc_mgr> > memory_mgr_off_string;

//template gstl::string;
//template memory_mgr_string;
//template memory_mgr_off_string;

class basic_string_test_fixture
{
public:
	static const size_t sz_null = 0;
	static const char m_test_str[];
	static const size_t m_test_str_len;
};
const char basic_string_test_fixture::m_test_str[] = "Hello World!";
const size_t basic_string_test_fixture::m_test_str_len = STR_LEN( m_test_str );

BOOST_FIXTURE_TEST_SUITE( basic_string_test, basic_string_test_fixture )

typedef boost::mpl::list< std::string/*, gstl_string, memory_mgr_string, memory_mgr_off_string*/> t_list;
		
 	BOOST_AUTO_TEST_CASE_TEMPLATE( test_construction, string_type, t_list )
 	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		size_t test_len = 10;

 		string_type s;
 		BOOST_CHECK( s.data() != 0 );
 		BOOST_CHECK_EQUAL( s.size(), sz_null );

		string_type s2( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s2.size(), m_test_str_len );
		BOOST_CHECK_GE( s2.capacity(), m_test_str_len );

		//Throws: out_of_range if pos > str.size().
		BOOST_CHECK_THROW( string_type se(s2, s2.size() + 1 ), std::out_of_range );

		string_type s3( m_test_str );
		BOOST_CHECK_EQUAL( s3.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s3.size(), m_test_str_len );
		BOOST_CHECK_GE( s3.capacity(), m_test_str_len );

		string_type s4( s2 );
		BOOST_CHECK_EQUAL( s4.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s4.size(), s2.size() );
		BOOST_CHECK_GE( s4.capacity(), s2.size() );

		size_t s2_len_2 = s2.size() / 2;
		string_type s5( s2, 0, s2_len_2 );
		BOOST_CHECK_EQUAL( char_traits::compare( s5.c_str(), s2.c_str(), s2_len_2 - 1 ), 0 );
		BOOST_CHECK_EQUAL( s5.size(), s2_len_2 );
		BOOST_CHECK_GE( s5.capacity(), s2_len_2 );

		string_type s6( s2, s2_len_2 );
		BOOST_CHECK_EQUAL( s6.c_str(), s2.c_str() + s2_len_2 );
		BOOST_CHECK_EQUAL( s6.size(), s2_len_2 );
		BOOST_CHECK_GE( s6.capacity(), s2_len_2 );


		string_type s7( test_len, ch );
		test_compare_n_chars( s7.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s7.size(), test_len );
		BOOST_CHECK_GE( s7.capacity(), test_len );

		string_type s8( s2.begin(), s2.end() );
		BOOST_CHECK_EQUAL( s8.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s8.size(), s2.size() );
		BOOST_CHECK_GE( s8.capacity(), s2.size() );

		string_type s9( (int)test_len, (int)ch );
		test_compare_n_chars( s9.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s9.size(), test_len );
		BOOST_CHECK_GE( s9.capacity(), test_len );
	}

 	BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign_operator, string_type, t_list )
 	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		size_t sz_one = 1;

 
 		string_type s( m_test_str, m_test_str_len );

		string_type s2;
		s2 = s;
		BOOST_CHECK_EQUAL( s2.c_str(), s.c_str() );
		BOOST_CHECK_EQUAL( s2.size(), s.size() );
		BOOST_CHECK_GE( s2.capacity(), s.size() );

		string_type s3;
		s3 = m_test_str;
		BOOST_CHECK_EQUAL( s3.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s3.size(), m_test_str_len );
		BOOST_CHECK_GE( s3.capacity(), m_test_str_len );

		string_type s4;
		s4 = ch;
		BOOST_CHECK_EQUAL( *s4.c_str(), ch );
		BOOST_CHECK_EQUAL( s4.size(), sz_one );
		BOOST_CHECK_GE( s4.capacity(), sz_one );
 	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_begin_end, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type first = *m_test_str;
		char_type last = *(m_test_str + (m_test_str_len - 1));

		string_type s( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( *s.begin(), first );
		BOOST_CHECK_EQUAL( *(s.end() - 1), last );
		BOOST_CHECK_EQUAL( *s.rbegin(), last );
		BOOST_CHECK_EQUAL( *(s.rend() - 1), first );

		const string_type cs( s );
		BOOST_CHECK_EQUAL( *cs.begin(), first );
		BOOST_CHECK_EQUAL( *(cs.end() - 1), last );
		BOOST_CHECK_EQUAL( *cs.rbegin(), last );
		BOOST_CHECK_EQUAL( *(cs.rend() - 1), first );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_sizing, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		char_type ch_null = 0;
		size_t test_add_len = 10;
		size_t test_len = m_test_str_len + test_add_len;

		string_type s( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s.size(), m_test_str_len );
		BOOST_CHECK_EQUAL( s.length(), m_test_str_len );
		BOOST_CHECK_EQUAL( s.size(), s.length() );
		BOOST_CHECK_GE( s.capacity(), m_test_str_len );
		
		/*
		— If n <= size(), the function replaces the string designated by *this with a string of length n
		whose elements are a copy of the initial elements of the original string designated by *this.
		— If n > size(), the function replaces the string designated by *this with a string of length n whose
		first size() elements are a copy of the original string designated by *this, and whose remaining
		elements are all initialized to c.
		*/
		size_t s_len_2 = s.size() / 2;
		s.resize( s_len_2 );
		BOOST_CHECK_EQUAL( char_traits::compare( s.c_str(), m_test_str, s_len_2 - 1 ), 0 );
		BOOST_CHECK_EQUAL( s.size(), s_len_2 );
		BOOST_CHECK_GE( s.capacity(), s_len_2 );
		
		string_type s2( m_test_str, m_test_str_len );
		s2.resize( test_len, ch );
		BOOST_CHECK_EQUAL( s2.size(), test_len );
		BOOST_CHECK_GE( s2.capacity(), test_len );
		BOOST_CHECK_EQUAL( char_traits::compare( s2.c_str(), m_test_str, m_test_str_len - 1 ), 0 );
		test_compare_n_chars( s2.c_str() + m_test_str_len, ch, test_add_len );


		string_type s3;
		s3.resize( test_len, ch );
		BOOST_CHECK_EQUAL( s3.size(), test_len );
		BOOST_CHECK_GE( s3.capacity(), test_len );
		test_compare_n_chars( s3.c_str(), ch, test_len );

		string_type s4;
		s4.resize( test_len );
		BOOST_CHECK_EQUAL( s4.size(), test_len );
		BOOST_CHECK_GE( s4.capacity(), test_len );
		test_compare_n_chars( s4.c_str(), ch_null, test_len );

		


		//resize/reserve Throws: length_error if n > max_size().
		string_type st;
		BOOST_CHECK_THROW( st.resize( st.max_size() + 1 ), std::length_error );
		BOOST_CHECK_THROW( st.reserve( st.max_size() + 1 ), std::length_error );
		BOOST_CHECK_THROW( st.resize( st.max_size() + 1, ch ), std::length_error );

		/*
		Calling reserve() with a res_arg argument less than capacity() is in effect a non-binding shrink
		request. A call with res_arg <= size() is in effect a non-binding shrink-to-fit request.
		*/
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_empty, string_type, t_list )
	{
		string_type s;
		BOOST_CHECK_EQUAL( s.size(), sz_null );
		BOOST_CHECK( s.empty() );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign, string_type, t_list )
	{
		string_type s;
		BOOST_CHECK( s.data() != 0 );
		BOOST_CHECK_EQUAL( s.size(), sz_null );
		

		s.assign( m_test_str, m_test_str + m_test_str_len );
		BOOST_CHECK_EQUAL( s.c_str(), m_test_str );

		string_type s2;
		s2.assign( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );
	}
	

BOOST_AUTO_TEST_SUITE_END()

