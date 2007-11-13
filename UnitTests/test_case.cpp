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

#include "StdAfx.h"
#include "test_case.h"

static const size_t fill_count = 50;
static const wchar_t endl = L'\n';

test_case::test_case( const std::wstring& name, wchar_t fill_char/*= L'='*/ )
:m_name( name ),
m_fill_char( fill_char )
{
	print_line();
	std::wcout << L"Testing '" << m_name << L"'..." << endl;
}

test_case::~test_case()
{
	std::wcout << L"Test succeeded." << endl;
	print_line();
}

void test_case::print_line()
{
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), fill_count, m_fill_char );
	std::wcout << endl;
}

void test_case::failed( const std::wstring& description )
{
	std::wcout << L">>>" << m_name << L" test failed!!!" << endl;
	if( !description.empty() )
	{
		std::wcout << description << endl;
	}
}