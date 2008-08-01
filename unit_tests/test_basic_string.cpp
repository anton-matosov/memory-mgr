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

#include "test_common.hpp"

#include <gstl/detail/string.hpp>
#include <memory-mgr/allocator.h>
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/offset_allocator.h>

#include <boost/mpl/list.hpp>

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

	typedef boost::mpl::list< gstl_string, memory_mgr_string, memory_mgr_off_string/**/> t_list;
		
 	BOOST_AUTO_TEST_CASE_TEMPLATE( test_construction, string_type, t_list )
 	{
 		string_type s;
 		BOOST_CHECK( s.data() != 0 );
 		BOOST_CHECK_EQUAL( s.size(), sz_null );	
 	}

	
	BOOST_AUTO_TEST_CASE_TEMPLATE( test_assign, string_type, t_list )
	{
		string_type s;
		BOOST_CHECK( s.data() != 0 );
		BOOST_CHECK_EQUAL( s.size(), sz_null );
		

		s.assign( m_test_str, m_test_str + m_test_str_len );
		BOOST_CHECK_EQUAL( s.begin(), m_test_str );

		string_type s2;
		s2.assign( m_test_str, m_test_str_len );
		BOOST_CHECK_EQUAL( s2.begin(), m_test_str );
	}
	

BOOST_AUTO_TEST_SUITE_END()

