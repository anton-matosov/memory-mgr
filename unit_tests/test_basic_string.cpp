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
	static const size_t sz_one = 1;
	static const size_t sz_two = 2;
	static const size_t sz_three = 3;
	static const size_t sz_four = 4;
	static const size_t sz_five = 5;
	static const size_t sz_six = 6;
	static const size_t sz_seven = 7;
	static const size_t sz_eight = 8;
	static const size_t sz_nine = 9;
	static const size_t sz_ten = 10;

	static const char m_test_str[];
	static const char m_test_str2[];
	static const size_t m_test_str_len;
	static const size_t m_test_str_len2;
};
const char basic_string_test_fixture::m_test_str[] = "Hello World!";
const size_t basic_string_test_fixture::m_test_str_len = STR_LEN( m_test_str );
const char basic_string_test_fixture::m_test_str2[] = "Goodbye!";
const size_t basic_string_test_fixture::m_test_str_len2 = STR_LEN( m_test_str2 );

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

	//21.3.3 basic_string capacity
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_capacity, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		char_type ch_null = char_type();
		size_t test_add_len = 10;
		size_t test_len = m_test_str_len + test_add_len;
		size_t test_len_x2 = test_len * 2;

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
		size_t s_len_2 = s.size() / 2; /* 1 */
		s.resize( s_len_2 );
		BOOST_CHECK_EQUAL( char_traits::compare( s.c_str(), m_test_str, s_len_2 - 1 ), 0 );
		BOOST_CHECK_EQUAL( s.size(), s_len_2 );
		BOOST_CHECK_GE( s.capacity(), s_len_2 );
		
		string_type s2( m_test_str, m_test_str_len ); /* 2 */
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
		
		s4.reserve( test_len_x2 );
		BOOST_CHECK_GE( s4.capacity(), test_len_x2 );
		/*
		Calling reserve() with a res_arg argument less than capacity() is in effect a non-binding shrink
		request. A call with res_arg <= size() is in effect a non-binding shrink-to-fit request.
		*/
		//Doesn't work for std::basic_string in VC9 (VS 2008) 
		//s4.reserve();
		//BOOST_CHECK_LT( s4.capacity(), test_len_x2 );
		//BOOST_CHECK_GE( s4.capacity(), s4.size() );

		//resize/reserve Throws: length_error if n > max_size().
		//218)reserve() uses Allocator::allocate() which may throw an appropriate exception.
		string_type st;
		BOOST_CHECK_THROW( st.resize( st.max_size() + 1 ), std::length_error );
		BOOST_CHECK_THROW( st.reserve( st.max_size() + 1 ), std::length_error );
		BOOST_CHECK_THROW( st.resize( st.max_size() + 1, ch ), std::length_error );	
	}

	
	//21.3.3 basic_string capacity
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_clear, string_type, t_list )
	{
		string_type s( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s.size(), m_test_str_len );
		BOOST_CHECK_GE( s.capacity(), m_test_str_len );

		s.clear();
		BOOST_CHECK_EQUAL( s.size(), sz_null );
		BOOST_CHECK_GE( s.capacity(), sz_null );

	}

	//21.3.3 basic_string capacity
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_empty, string_type, t_list )
	{
		string_type s;
		BOOST_CHECK_EQUAL( s.size(), sz_null );
		BOOST_CHECK( s.empty() );
	}

	//21.3.4 basic_string element access
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_access_ops, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch_null = char_type();
		size_t ch_id = m_test_str_len / 2;

		string_type s( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s.size(), m_test_str_len );
		BOOST_CHECK_GE( s.capacity(), m_test_str_len );

		BOOST_CHECK_EQUAL( m_test_str[ch_id],  s.c_str()[ch_id] );
		BOOST_CHECK_EQUAL( s[ch_id],  m_test_str[ch_id] );
		
		/*
		Requires: pos < size()
		Throws: out_of_range if pos >= size().
		Returns: operator[](pos).
		*/
		BOOST_CHECK_EQUAL( s.at( ch_id ),  s[ch_id] );
		BOOST_CHECK_THROW( s.at( s.size() ),  std::out_of_range );
		BOOST_CHECK_THROW( s.at( s.size() + 1 ),  std::out_of_range );

		string_type cs( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( cs.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( cs.size(), m_test_str_len );
		BOOST_CHECK_GE( cs.capacity(), m_test_str_len );

		BOOST_CHECK_EQUAL( m_test_str[ch_id],  cs.c_str()[ch_id] );
		BOOST_CHECK_EQUAL( cs[ch_id],  m_test_str[ch_id] );
		BOOST_CHECK_EQUAL( cs[cs.size()],  ch_null );
		BOOST_CHECK_EQUAL( cs.at( ch_id ),  cs[ch_id] );
		BOOST_CHECK_THROW( cs.at( s.size() ),  std::out_of_range );
		BOOST_CHECK_THROW( cs.at( s.size() + 1 ),  std::out_of_range );
	}

	//21.3.5.1 basic_string::operator+=
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_append_operator, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'c';

		size_t str_len_2 = m_test_str_len / 2;
		string_type s( m_test_str, 0, str_len_2 );
		string_type s2( m_test_str, str_len_2, string_type::npos );
		
		BOOST_CHECK_EQUAL( s.size(), str_len_2 );
		BOOST_CHECK_EQUAL( s2.size(), m_test_str_len - str_len_2 );
		
		string_type s3;
		//basic_string<charT,traits,Allocator>&
		//	operator+=(const basic_string<charT,traits,Allocator>& str);
		s3 += s;
		BOOST_CHECK_EQUAL( s3.size(), s.size() );
		BOOST_CHECK_EQUAL( s3.c_str(), s.c_str() );

		s3 += s2;
		BOOST_CHECK_EQUAL( s3.size(), m_test_str_len );
		BOOST_CHECK_EQUAL( s3.c_str(), m_test_str );

		string_type s4;
		//basic_string<charT,traits,Allocator>& operator+=(const charT* s);
		s4 += m_test_str; 
		BOOST_CHECK_EQUAL( s4.size(), m_test_str_len );
		BOOST_CHECK_EQUAL( s4.c_str(), m_test_str );

		string_type s5;
		//basic_string<charT,traits,Allocator>& operator+=(charT c);
		s5 += ch;
		BOOST_CHECK_EQUAL( s5.size(), sz_one );
		BOOST_CHECK_EQUAL( *s5.c_str(), ch );
	}

	//21.3.5.2 basic_string::append
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_append, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		size_t test_len = 10;

		string_type s;
		BOOST_CHECK( s.data() != 0 );
		BOOST_CHECK_EQUAL( s.size(), sz_null );

		string_type s2;
		s2.append( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s2.size(), m_test_str_len );
		BOOST_CHECK_GE( s2.capacity(), m_test_str_len );

		//Throws: out_of_range if pos > str.size().
		string_type se;
		BOOST_CHECK_THROW( se.append(s2, s2.size() + 1, string_type::npos ), std::out_of_range );
		/*
		Determines the effective length rlen of the string to append as the smaller of n and
		str.size() - pos. The function then throws length_error if size() >= npos -
		rlen.
		*/
		//TODO: implement this test case, don't know how to implement it
		//BOOST_CHECK_THROW( se.append(s2, string_type::npos, string_type::npos ), std::length_error );

		string_type s3;
		s3.append( m_test_str );
		BOOST_CHECK_EQUAL( s3.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s3.size(), m_test_str_len );
		BOOST_CHECK_GE( s3.capacity(), m_test_str_len );

		string_type s4;
		s4.append( s2 );
		BOOST_CHECK_EQUAL( s4.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s4.size(), s2.size() );
		BOOST_CHECK_GE( s4.capacity(), s2.size() );

		size_t s2_len_2 = s2.size() / 2;
		string_type s5;
		s5.append( s2, 0, s2_len_2 );
		BOOST_CHECK_EQUAL( char_traits::compare( s5.c_str(), s2.c_str(), s2_len_2 - 1 ), 0 );
		BOOST_CHECK_EQUAL( s5.size(), s2_len_2 );
		BOOST_CHECK_GE( s5.capacity(), s2_len_2 );

		string_type s6;
		s6.append( s2, s2_len_2, string_type::npos );
		BOOST_CHECK_EQUAL( s6.c_str(), s2.c_str() + s2_len_2 );
		BOOST_CHECK_EQUAL( s6.size(), s2_len_2 );
		BOOST_CHECK_GE( s6.capacity(), s2_len_2 );


		string_type s7;
		s7.append( test_len, ch );
		test_compare_n_chars( s7.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s7.size(), test_len );
		BOOST_CHECK_GE( s7.capacity(), test_len );

		string_type s8;
		s8.append( s2.begin(), s2.end() );
		BOOST_CHECK_EQUAL( s8.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s8.size(), s2.size() );
		BOOST_CHECK_GE( s8.capacity(), s2.size() );

		string_type s9;
		s9.append( (int)test_len, (int)ch );
		test_compare_n_chars( s9.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s9.size(), test_len );
		BOOST_CHECK_GE( s9.capacity(), test_len );
	}

	//21.3.5.3 basic_string::assign
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		char_type ch = 'x';
		size_t test_len = 10;

		string_type s;
		BOOST_CHECK( s.data() != 0 );
		BOOST_CHECK_EQUAL( s.size(), sz_null );

		string_type s2;
		s2.assign( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s2.size(), m_test_str_len );
		BOOST_CHECK_GE( s2.capacity(), m_test_str_len );

		//Throws: out_of_range if pos > str.size().
		string_type se;
		BOOST_CHECK_THROW( se.assign(s2, s2.size() + 1, string_type::npos ), std::out_of_range );
		
		string_type s3;
		s3.assign( m_test_str );
		BOOST_CHECK_EQUAL( s3.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s3.size(), m_test_str_len );
		BOOST_CHECK_GE( s3.capacity(), m_test_str_len );

		string_type s4;
		s4.assign( s2 );
		BOOST_CHECK_EQUAL( s4.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s4.size(), s2.size() );
		BOOST_CHECK_GE( s4.capacity(), s2.size() );

		size_t s2_len_2 = s2.size() / 2;
		string_type s5;
		s5.assign( s2, 0, s2_len_2 );
		BOOST_CHECK_EQUAL( char_traits::compare( s5.c_str(), s2.c_str(), s2_len_2 - 1 ), 0 );
		BOOST_CHECK_EQUAL( s5.size(), s2_len_2 );
		BOOST_CHECK_GE( s5.capacity(), s2_len_2 );

		string_type s6;
		s6.assign( s2, s2_len_2, string_type::npos );
		BOOST_CHECK_EQUAL( s6.c_str(), s2.c_str() + s2_len_2 );
		BOOST_CHECK_EQUAL( s6.size(), s2_len_2 );
		BOOST_CHECK_GE( s6.capacity(), s2_len_2 );


		string_type s7;
		s7.assign( test_len, ch );
		test_compare_n_chars( s7.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s7.size(), test_len );
		BOOST_CHECK_GE( s7.capacity(), test_len );

		string_type s8;
		s8.assign( s2.begin(), s2.end() );
		BOOST_CHECK_EQUAL( s8.c_str(), s2.c_str() );
		BOOST_CHECK_EQUAL( s8.size(), s2.size() );
		BOOST_CHECK_GE( s8.capacity(), s2.size() );

		string_type s9;
		s9.assign( (int)test_len, (int)ch );
		test_compare_n_chars( s9.c_str(), ch, test_len );
		BOOST_CHECK_EQUAL( s9.size(), test_len );
		BOOST_CHECK_GE( s9.capacity(), test_len );
	}

	//21.3.5.4 basic_string::insert
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_insert, string_type, t_list )
	{
		/*
		basic_string<charT,traits,Allocator>&
			insert(size_type pos1,
				const basic_string<charT,traits,Allocator>& str);
		Returns: insert(pos1,str,0,npos).
		*/
		string_type strorg = "This is test string for string calls";
		string_type str;
		/*
		* In case of reallocation there is no auto reference problem
		* so we reserve a big enough string to be sure to test this
		* particular point.
		*/
		str.reserve(100);
		str = strorg;

		//test self insertion:
		str.insert(10, str.c_str() + 5, 15);
		BOOST_CHECK_EQUAL( str, "This is teis test string st string for string calls" );

		str = strorg;
		str.insert(15, str.c_str() + 5, 25);
		BOOST_CHECK_EQUAL( str, "This is test stis test string for stringring for string calls" );

		str = strorg;
		str.insert(0, str.c_str() + str.size() - 4, 4);
		BOOST_CHECK_EQUAL( str, "allsThis is test string for string calls" );

		str = strorg;
		str.insert(0, str.c_str() + str.size() / 2 - 1, str.size() / 2 + 1);
		BOOST_CHECK_EQUAL( str, "ng for string callsThis is test string for string calls" );

		str = strorg;
		string_type::iterator b = str.begin();
		string_type::const_iterator s = str.begin() + str.size() / 2 - 1;
		string_type::const_iterator e = str.end();
		str.insert( b, s, e );
		BOOST_CHECK_EQUAL( str, "ng for string callsThis is test string for string calls" );

		str = strorg;
		str.insert(str.begin(), str.begin() + str.size() / 2 - 1, str.end());
		BOOST_CHECK_EQUAL( str, "ng for string callsThis is test string for string calls" );


		std::vector<int> int_vect;
		//Just a compile time test:
		str.insert(str.end(), int_vect.begin(), int_vect.end());


		string_type str0;
		str0.insert(str0.begin(), 5, '0');
		BOOST_CHECK_EQUAL( str0, "00000" );

		string_type str1;
		{
			string_type::size_type pos = 0, nb = 2;
			str1.insert(pos, nb, '1');
		}
		BOOST_CHECK_EQUAL( str1, "11" );

		str0.insert(0, str1);
		BOOST_CHECK_EQUAL( str0, "1100000" );

		/*
		basic_string<charT,traits,Allocator>& insert(size_type pos1,
			const basic_string<charT,traits,Allocator>& str, size_type pos2, size_type n);
		2 Requires pos1 <= size() and pos2 <= str.size()
		3 Throws: out_of_range if pos1 > size() or pos2 > str.size().
		*/
		string_type str2("2345");
		str0.insert( str0.size(), str2, 1, 2 );
		BOOST_CHECK_EQUAL( str0, "110000034" );
		BOOST_CHECK_THROW( str0.insert( str0.size() + 1, str2, 1, 2 );, std::out_of_range );
		BOOST_CHECK_THROW( str0.insert( str0.size(), str2, str2.size() + 1, 2 );, std::out_of_range );

		str1.insert( str1.begin() + 1, 2, '2' );
		BOOST_CHECK_EQUAL( str1, "1221" );

		str1.insert( 2, "333333", 3 );
		BOOST_CHECK_EQUAL( str1, "1233321" );

		str1.insert( 4, "4444" );
		BOOST_CHECK_EQUAL( str1, "12334444321" );

		str1.insert( str1.begin() + 6, '5' );
		BOOST_CHECK_EQUAL( str1, "123344544321" );
	}

	//21.3.5.5 basic_string::erase
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_erase, string_type, t_list )
	{
		string_type s( m_test_str );
		
		s.erase(s.begin() + 1, s.end() - 1); // Erase all but first and last.

		BOOST_CHECK_EQUAL( s.size(), sz_two );
		BOOST_CHECK_EQUAL( *s.c_str(), 'H' );
		BOOST_CHECK_EQUAL( s[s.size() - 1], '!' );
		
		s.insert( 1, m_test_str );
		s.erase( s.begin()); // Erase first element.
		s.erase( s.end() - 1); // Erase last element.
		BOOST_CHECK_EQUAL( s.c_str(), m_test_str );
		s.clear(); // Erase all.
		BOOST_CHECK( s.empty() );

		s = m_test_str;
		s.erase( 1, s.size() - 2 ); // Erase all but first and last.
		BOOST_CHECK_EQUAL( s.size(), sz_two );
		BOOST_CHECK_EQUAL( *s.c_str(), 'H' );
		BOOST_CHECK_EQUAL( s[s.size() - 1], '!' );

		s.erase(1);
		BOOST_CHECK_EQUAL( s, "H" );

		//basic_string<charT,traits,Allocator>& erase(size_type pos = 0, size_type n = npos);
		//Throws: out_of_range if pos > size().
		BOOST_CHECK_THROW( s.erase( s.size() + 1 ), std::out_of_range );
	}

	//21.3.5.6 basic_string::replace
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_replace, string_type, t_list )
	{

		/*
		basic_string<charT,traits,Allocator>&
			replace(size_type pos1, size_type n1,
				const basic_string<charT,traits,Allocator>& str);
		Returns: replace(pos1, n1, str, 0, npos).
		*/

		/*
		basic_string<charT,traits,Allocator>&
			replace(size_type pos1, size_type n1,
				const basic_string<charT,traits,Allocator>& str,
					size_type pos2, size_type n2);
		2 Requires: pos1 <= size() && pos2 <= str.size().
		3 Throws: out_of_range if pos1 > size() or pos2 > str.size().
		5 Throws: length_error if size() - xlen >= npos - rlen.
		*/
	}

	//21.3.5.7 basic_string::copy
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_copy, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;
		
		size_t test_len = m_test_str_len / 2;
		char_type str[m_test_str_len];// = new char_type[m_test_str_len];

		string_type s( m_test_str, m_test_str_len );
		string_type s2( m_test_str, test_len ); //== str
#ifdef _MSC_VER
#pragma warning(push)
#endif
//MSVC 9.0 C4996 Checked iterators
#pragma warning (disable: 4996)

		//Throws: out_of_range if pos > size().
		//TODO: uncomment. MSVC std::basic_string doesn't throw this exception
		//BOOST_CHECK_THROW( s.copy( str, test_len + 1, 0 ), std::out_of_range );

		s.copy( str, test_len, 0 );

#ifdef _MSC_VER
#pragma warning(pop)
#endif

		str[test_len] = 0;
		BOOST_CHECK_EQUAL( char_traits::length( str ), test_len );
		BOOST_CHECK_EQUAL( str, s2.c_str() );

	}
	
	//21.3.5.8 basic_string::swap
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_swap, string_type, t_list )
	{		
		string_type s( m_test_str, m_test_str_len );

		string_type s2( m_test_str2, m_test_str_len2 );

		s.swap( s2 );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );
		BOOST_CHECK_EQUAL( s2.size(), m_test_str_len );
		BOOST_CHECK_GE( s2.capacity(), m_test_str_len );

		BOOST_CHECK_EQUAL( s.c_str(), m_test_str2 );
		BOOST_CHECK_EQUAL( s.size(), m_test_str_len2 );
		BOOST_CHECK_GE( s.capacity(), m_test_str_len2 );
	}

	//21.3.6.1 basic_string::find
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_find, string_type, t_list )
	{
		string_type s("one two three one two three");
		BOOST_CHECK_EQUAL( s.find("one"), sz_null );
		BOOST_CHECK_EQUAL( s.find('t'), sz_four );
		BOOST_CHECK_EQUAL( s.find('t', 5), sz_eight );
		
		BOOST_CHECK_EQUAL( s.find("four"), string_type::npos );
		BOOST_CHECK_EQUAL( s.find("one", string_type::npos), string_type::npos );

	}

	//21.3.6.2 basic_string::rfind
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_rfind, string_type, t_list )
	{
		string_type s("one two three one two three");

		BOOST_CHECK_EQUAL( s.rfind("two"), size_t(18) );
		BOOST_CHECK_EQUAL( s.rfind("two", 0), string_type::npos );
		BOOST_CHECK_EQUAL( s.rfind("two", 11), sz_four );
		BOOST_CHECK_EQUAL( s.rfind('w'), size_t(19) );

		string_type test( "aba" );

		BOOST_CHECK_EQUAL( test.rfind( "a", 2, 1 ), sz_two );
		BOOST_CHECK_EQUAL( test.rfind( "a", 1, 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.rfind( "a", 0, 1 ), sz_null );

		BOOST_CHECK_EQUAL( test.rfind( 'a', 2 ), sz_two );
		BOOST_CHECK_EQUAL( test.rfind( 'a', 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.rfind( 'a', 0 ), sz_null );
	}

	//21.3.6.3 basic_string::find_first_of
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_find_first_of, string_type, t_list )
	{
		string_type s("one two three one two three");
		BOOST_CHECK_EQUAL( s.find_first_of("abcde"), sz_two );
	}

	//21.3.6.4 basic_string::find_last_of
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_find_last_of, string_type, t_list )
	{
		string_type s( "one two three one two three" );

		BOOST_CHECK_EQUAL( s.find_last_of( "abcde" ), size_t(26) );

		string_type test( "aba" );

		BOOST_CHECK_EQUAL( test.find_last_of( "a", 2, 1 ), sz_two );
		BOOST_CHECK_EQUAL( test.find_last_of( "a", 1, 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.find_last_of( "a", 0, 1 ), sz_null );

		BOOST_CHECK_EQUAL( test.find_last_of( 'a', 2 ), sz_two );
		BOOST_CHECK_EQUAL( test.find_last_of( 'a', 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.find_last_of( 'a', 0 ), sz_null );
	}

	//21.3.6.5 basic_string::find_first_not_of
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_find_first_not_of, string_type, t_list )
	{
		string_type s("one two three one two three");

		BOOST_CHECK_EQUAL( s.find_first_not_of("enotw "), sz_nine );
	}

	//21.3.6.6 basic_string::find_last_not_of
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_find_last_not_of, string_type, t_list )
	{
		string_type s( "one two three one two three" );

		BOOST_CHECK_EQUAL( s.find_last_not_of( "ehortw " ), size_t(15) );

		string_type test( "aba" );

		BOOST_CHECK_EQUAL( test.find_last_not_of( "a", 2, 1 ), sz_one );
		BOOST_CHECK_EQUAL( test.find_last_not_of( "b", 2, 1 ), sz_two );
		BOOST_CHECK_EQUAL( test.find_last_not_of( "a", 1, 1 ), sz_one );
		BOOST_CHECK_EQUAL( test.find_last_not_of( "b", 1, 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.find_last_not_of( "a", 0, 1 ), string_type::npos );
		BOOST_CHECK_EQUAL( test.find_last_not_of( "b", 0, 1 ), sz_null );

		BOOST_CHECK_EQUAL( test.find_last_not_of( 'a', 2 ), sz_one );
		BOOST_CHECK_EQUAL( test.find_last_not_of( 'b', 2 ), sz_two );
		BOOST_CHECK_EQUAL( test.find_last_not_of( 'a', 1 ), sz_one );
		BOOST_CHECK_EQUAL( test.find_last_not_of( 'b', 1 ), sz_null );
		BOOST_CHECK_EQUAL( test.find_last_not_of( 'a', 0 ), string_type::npos );
		BOOST_CHECK_EQUAL( test.find_last_not_of( 'b', 0 ), sz_null );
	}

	//21.3.6.7 basic_string::substr
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_substr, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;

		size_t test_len = m_test_str_len / 2;

		string_type s( m_test_str, m_test_str_len );

		string_type s2 = s.substr( 0, test_len );
		string_type s3( m_test_str, test_len );
		BOOST_CHECK_EQUAL( s2.c_str(), s3.c_str() );
		BOOST_CHECK_EQUAL( s2.size(), test_len );
		BOOST_CHECK_GE( s2.capacity(), test_len );

		//Throws: out_of_range if pos > size()
		BOOST_CHECK_THROW( s.substr( s.size() + 1 ), std::out_of_range );
	}

	//21.3.6.8 basic_string::compare
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_compare, string_type, t_list )
	{
		typedef typename string_type::value_type char_type;
		typedef gstl::char_traits< char_type > char_traits;

		size_t test_len = m_test_str_len / 2;

		string_type s( m_test_str, m_test_str_len );
		string_type s2( m_test_str, test_len );
		string_type s3( m_test_str2, m_test_str_len2 );

		/* int compare(const basic_string& str) const */
		BOOST_CHECK_LT( s2.compare( s ), 0 );
		BOOST_CHECK_EQUAL( s.compare( s ), 0 );
		BOOST_CHECK_EQUAL( s2.compare( s2 ), 0 );
		BOOST_CHECK_GT( s.compare( s2 ), 0 );

		/* int compare(const charT *s) const */
		BOOST_CHECK_LT( s2.compare( m_test_str ), 0 );
		BOOST_CHECK_EQUAL( s.compare( m_test_str ), 0 );
		BOOST_CHECK_EQUAL( s3.compare( m_test_str2 ), 0 );
		BOOST_CHECK_GT( s.compare( m_test_str2 ), 0 );

		/* int compare(size_type pos1, size_type n1, const basic_string<charT,traits,Allocator>& str) const; */
		const char_type* test_str = m_test_str + 2;
		string_type s4( test_str, 3 );
		BOOST_CHECK_EQUAL( s.compare(2, 3, s4), 0 );
		s4.assign( test_str, 2 );
		BOOST_CHECK_GT( s.compare(2, 3, s4), 0 );
		s4.assign( test_str, 4 );
		BOOST_CHECK_LT( s.compare(2, 3, s4), 0 );

		/* int compare(size_type pos1, size_type n1,
						const basic_string<charT,traits,Allocator>& str,
							size_type pos2, size_type n2) const;
		*/
		BOOST_CHECK_LT( s.compare(2, 3, s2, 2, 4), 0 );
		BOOST_CHECK_EQUAL( s.compare(2, 3, s2, 2, 3), 0 );
		BOOST_CHECK_GT( s.compare(2, 3, s2, 2, 2), 0 );

		/* int compare(size_type pos, size_type n1, const charT *s, size_type n2) const; */
		BOOST_CHECK_EQUAL( s.compare(2, 3, test_str, 3), 0 );
		BOOST_CHECK_LT( s.compare(2, 3, test_str, 4), 0 );
		BOOST_CHECK_GT( s.compare(2, 3, test_str, 2), 0 );

		/*
		int compare(const basic_string<charT,traits,Allocator>& str) const
		1 Effects: Determines the effective length rlen of the strings to compare as the smallest of size() and
		str.size(). The function then compares the two strings by calling
		traits::compare(data(), str.data(), rlen).
		2 Returns: the nonzero result if the result of the comparison is nonzero. Otherwise, returns a value as indicated
		in Table 44:
		size() < str.size() < 0
		size() == str.size() 0
		size() > str.size() > 0  
		
		//////////////////////////////////////////////////////////////////////////
		int compare(size_type pos1, size_type n1,
		const basic_string<charT,traits,Allocator>& str) const;
		3 Returns:
		basic_string<charT,traits,Allocator>(*this,pos1,n1).compare(
		str) .

		//////////////////////////////////////////////////////////////////////////
		int compare(size_type pos1, size_type n1,
		const basic_string<charT,traits,Allocator>& str,
		size_type pos2, size_type n2) const;
		4 Returns:
		basic_string<charT,traits,Allocator>(*this,pos1,n1).compare(
		basic_string<charT,traits,Allocator>(str,pos2,n2)) .

		//////////////////////////////////////////////////////////////////////////
		int compare(const charT *s) const;
		5 Returns: this->compare(basic_string<charT,traits,Allocator>(s)).
		int compare(size_type pos, size_type n1,
		const charT *s) const;
		6 Returns:
		basic_string<charT,traits,Allocator>(*this,pos,n1).compare(
		basic_string<charT,traits,Allocator>(s))

		//////////////////////////////////////////////////////////////////////////
		int compare(size_type pos, size_type n1,
		const charT *s, size_type n2) const;
		7 Returns:
		basic_string<charT,traits,Allocator>(*this,pos,n1).compare(
		basic_string<charT,traits,Allocator>(s,n2))

		//////////////////////////////////////////////////////////////////////////
		*/
	}

	//21.3.7 basic_string non-member functions
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_non_member_ops, string_type, t_list )
	{
		string_type s( m_test_str );
		string_type s2( m_test_str2 );
		string_type s3( m_test_str );
		s3 += s2;

		//21.3.7.1 operator+
		string_type s4 = s + s2;
		BOOST_CHECK_EQUAL( s3.c_str(), s4.c_str() );
		BOOST_CHECK_EQUAL( s3.c_str(), (m_test_str + s2).c_str() );
		BOOST_CHECK_EQUAL( s3.c_str(), (s + m_test_str2).c_str() );

		//21.3.7.2 operator==
		BOOST_CHECK_EQUAL( s3, s4 );
		BOOST_CHECK_EQUAL( s, m_test_str );
		BOOST_CHECK_EQUAL( m_test_str2, s2 );

		//21.3.7.3 operator!=
		BOOST_CHECK_NE( s3, s2 );
		BOOST_CHECK_NE( m_test_str, s2 );
		BOOST_CHECK_NE( s4, m_test_str2 );

		//21.3.7.4 operator<
		BOOST_CHECK_LT( s, s4 );
		BOOST_CHECK_LT( m_test_str, s4 );
		BOOST_CHECK_LT( s, s4.c_str() );

		//21.3.7.5 operator>
		BOOST_CHECK_GT( s4, s );
		BOOST_CHECK_GT( s4.c_str(), s );
		BOOST_CHECK_GT( s4, s.c_str() );

		//21.3.7.6 operator<=
		BOOST_CHECK_LE( s, s4 );
		BOOST_CHECK_LE( s.c_str(), s4 );
		BOOST_CHECK_LE( s, s4.c_str() );
		BOOST_CHECK_LE( s3, s4 );
		BOOST_CHECK_LE( s3.c_str(), s4 );
		BOOST_CHECK_LE( s3, s4.c_str() );

		//21.3.7.7 operator>=
		BOOST_CHECK_GE( s4, s );
		BOOST_CHECK_GE( s4.c_str(), s );
		BOOST_CHECK_GE( s4, s.c_str() );
		BOOST_CHECK_GE( s3, s4 );
		BOOST_CHECK_GE( s3.c_str(), s4 );
		BOOST_CHECK_GE( s3, s4.c_str() );

		//21.3.7.8 swap
		swap( s, s2 );
		BOOST_CHECK_EQUAL( s.c_str(), m_test_str2 );
		BOOST_CHECK_EQUAL( s2.c_str(), m_test_str );


	}

	//
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_, string_type, t_list )
	{

	}
BOOST_AUTO_TEST_SUITE_END()


