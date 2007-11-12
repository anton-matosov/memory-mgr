#pragma once

class test_case
{
	void print_line();

	std::wstring m_name;
	wchar_t m_fill_char;
public:
	test_case( const std::wstring& name, wchar_t fill_char = L'=' );

	void failed( const std::wstring& description = L"" );

	~test_case();
};

#define TEST_START( name ) test_case test( (name) );
#define SUBTEST_START( name ) test_case test( (name), L'-' );

#define TEST_END( expr ) if (!(expr)){test.failed(); return false;} else {return true;}
#define SUBTEST_END( expr ) TEST_END( expr )

#define TEST_SUCCEDED {return true;}
#define SUBTEST_SUCCEDED TEST_SUCCEDED

#define TEST_FAILED {test.failed(); return false;}
#define SUBTEST_FAILED TEST_FAILED


#define TEST_CHECH( expr ) if (!(expr)){test.failed(); return false;} else {std::wcout << L"Succeeded\n";}
#define TEST_CHECH_MSG( expr, msg ) if (!(expr)){test.failed(msg); return false;} else {std::wcout << L"Succeeded\n";}
#define TEST_PRINT( msg ) std::wcout << (msg) << L"\n";
#define TEST_METHOD_PRINT( method_name ) std::wcout << L"Testing " << (method_name) << L" method...\n";