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


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#include "test_common.hpp"

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

typedef gstl::basic_string<char, gstl::char_traits<char>,
	memory_mgr::allocator<char, ptr_alloc_mgr>, gstl::pointer_traits<char> > memory_mgr_string;

typedef gstl::basic_string<char, gstl::char_traits<char>,
	memory_mgr::offset_allocator<char, off_alloc_mgr> > memory_mgr_off_string;

template<class StringType = gstl::string >
class basic_string_test
	:public CppUnit::TestFixture
{
	typedef StringType string_type;
public:

	void test_construction()
	{
		string_type s;
		CPPUNIT_ASSERT( s.data() != 0 );
	}

	//////////////////////////////////////////////////////////////////////////
	CPPUNIT_TEST_SUITE( basic_string_test<string_type> );
 	CPPUNIT_TEST( test_construction );
// 	CPPUNIT_TEST( test_eq );
// 	CPPUNIT_TEST( test_lt );
// 	CPPUNIT_TEST( test_length );
// 	CPPUNIT_TEST( test_compare );
// 	CPPUNIT_TEST( test_find );
// 	CPPUNIT_TEST( test_move );
// 	CPPUNIT_TEST( test_assign_str );
// 	CPPUNIT_TEST( test_eq_int_type );
// 	CPPUNIT_TEST( test_not_eof );
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( basic_string_test<> );
CPPUNIT_TEST_SUITE_REGISTRATION( basic_string_test<memory_mgr_string> );
CPPUNIT_TEST_SUITE_REGISTRATION( basic_string_test<memory_mgr_off_string> );