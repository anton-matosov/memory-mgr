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

#ifndef MGR_TEST_CASE_HEADER
#define MGR_TEST_CASE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

class test_case
{
	void print_line();

	std::wstring	m_name;
	wchar_t			m_fill_char;
	bool			m_failed;
public:
	test_case( const std::wstring& name, wchar_t fill_char = L'=' );

	void failed( const std::wstring& description = L"" );

	~test_case();
};

#define TEST_START( name ) test_case __Test__Case__Entry__( (name) );
#define SUBTEST_START( name ) test_case __Test__Case__Entry__( (name), L'-' );

#define TEST_END( expr ) if (!(expr)){ TEST_FAILED } else {return true;}
#define SUBTEST_END( expr ) TEST_END( expr )

#define TEST_SUCCEDED {return true;}
#define SUBTEST_SUCCEDED TEST_SUCCEDED

#define TEST_FAILED_MSG( msg ) {__Test__Case__Entry__.failed( msg ); std::wcout << L"Line: " << __LINE__ << L"\n"; return false;}
#define TEST_FAILED TEST_FAILED_MSG( L"" )
#define SUBTEST_FAILED TEST_FAILED


#define TEST_CHECK( expr ) if (!(expr)){TEST_FAILED} else {std::wcout << L"Succeeded\n";}
#define TEST_CHECK_MSG( expr, msg ) if (!(expr)){ TEST_FAILED_MSG( msg ) } else {std::wcout << L"Succeeded\n";}
#define TEST_PRINT( msg ) std::wcout << (msg) << L"\n";
#define TEST_METHOD_PRINT( method_name ) std::wcout << L"Testing " << (method_name) << L" method...\n";
#define TEST_OPERATOR_PRINT( operator_name ) std::wcout << L"Testing operator" << (operator_name) << L"\n";

#endif// MGR_TEST_CASE_HEADER



