#include "StdAfx.h"
#include "test_case.h"

static const size_t fill_count = 50;
static const wchar_t fill_char = L'-';
static const wchar_t endl = L'\n';

test_case::test_case( const std::wstring& name )
:m_name( name )
{
	print_line();
	std::wcout << L"Testing '" << m_name << L"'..." << endl;
}

test_case::~test_case()
{
	std::wcout << L"Testing succeeded." << endl;
	print_line();
}

void test_case::print_line()
{
	std::fill_n( std::ostream_iterator<wchar_t, wchar_t>( std::wcout ), fill_count, fill_char );
	std::wcout << endl;
}

void test_case::failed( const std::wstring& description )
{
	std::wcout << L">>>Testing failed!!!" << endl;
	std::wcout << description << endl;
}