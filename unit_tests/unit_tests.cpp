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

#include <iostream>

#include "unit_tests_manager.hpp"
#include <gstl/config/config.hpp>


bool test_allocator();




int main(int /*argc*/, char* /*argv*/[])
{
	unit_tests_manager TestMgr;
		
	std::wcout << L"Unit tests for Generic STL library\n";
	std::wcout << L"Exe path: " << gstl::osapi::get_executable_path().c_str() << L"\n";
	std::wcout << L"Exe dir: " << gstl::osapi::get_exe_dir().c_str() << L"\n";
	TestMgr.add_result( test_allocator(),		L"test_allocator" );

	TestMgr.print_results();
	return std::cin.get();
}
