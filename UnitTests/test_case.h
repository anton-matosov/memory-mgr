#pragma once

class test_case
{
	void print_line();

	std::wstring m_name;
public:
	test_case( const std::wstring& name );

	void failed( const std::wstring& description );

	~test_case();
};

#define TEST_START( name ) test_case test( (name) );

#define TEST_CHECH( expr, msg ) if (!(expr)){test.failed(msg); return false;} else {std::wcout << L"Succeeded\n";}
#define TEST_PRINT( msg ) std::wcout << (msg) << L"\n";
#define TEST_METHOD_PRINT( method_name ) std::wcout << L"Testing " << (method_name) << L" method...\n";