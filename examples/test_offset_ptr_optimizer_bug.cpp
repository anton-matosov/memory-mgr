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

#include <iostream>
#include <algorithm>
#include <string>
#include <conio.h>
#include "memory-mgr/offset_ptr.h"
#include "memory-mgr/allocator.h"
#include "../unit_tests/managers.h"

static int variable = 123;
static int* variablePtr = NULL;

typedef memory_mgr::offset_ptr<int> pointer;
typedef memory_mgr::offset_ptr<int*> ppointer;

//__declspec(noinline)
static inline pointer get_var()
{
	return &variable;
}

//__declspec(noinline)
static inline ppointer get_pvar()
{
	return &variablePtr;
}

static inline int* get_var2()
{
	return &variable;
}

__declspec(noinline)
void change_variable()
{
	*get_var() = 345;

}

__declspec(noinline) 
void testAssign()
{
	//std::cout << *get_var2() << std::endl;
	*get_var() = 345;
	*get_var() = 456;
	//change_variable();
	//std::cout << *get_var2() << std::endl;
}

__declspec(noinline) 
void testPtrAssign()
{
	*get_pvar() = &variable;
	*variablePtr = 567;
}

void test_offset_ptr_optimizer_bug_1()
{
	std::cout << "Start" << std::endl;
	testAssign();
	std::cout << *get_var2() << std::endl;
	testPtrAssign();
	std::cout << *get_var2() << std::endl;
}

void test_offset_ptr_optimizer_bug_2()
{
	typedef memory_mgr::allocator<char, sing_sz_lfm_heap_sz_mgr> Allocator;

	typedef gstl::basic_string<char, gstl::char_traits<char>, Allocator> MyString;

	//for(int i = 0; i < 10000; ++i) 
	{
		const MyString orig(boost::lexical_cast<std::string>(rand()).c_str());
		MyString str(orig.begin(), orig.end());

		if (str != str.c_str()) 
		{
			//std::cout << "i = " << i << std::endl;
			std::cout << "str = '" << str << "'" << std::endl;
			std::cout << "str.c_str() = '" << str.c_str() << "'" << std::endl;
			return;
		}
	}
	std::cout << "No problems!" << std::endl;
}

void test_offset_ptr_optimizer_bug()
{
	test_offset_ptr_optimizer_bug_1();
	test_offset_ptr_optimizer_bug_2();
}

