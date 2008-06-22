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

#ifndef GSTL_UNIT_TESTS_MANAGER_HEADER
#define GSTL_UNIT_TESTS_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <deque>
#include <string>

typedef std::wstring string_type;

class unit_tests_manager
{
	typedef std::pair< bool, string_type > test_entry_type;
	typedef std::deque< test_entry_type > test_results_type;

	test_results_type m_test_results;
public:
	void add_result( bool result, const string_type& test_name );

	void print_results();

	~unit_tests_manager();
};

#endif //GSTL_UNIT_TESTS_MANAGER_HEADER

