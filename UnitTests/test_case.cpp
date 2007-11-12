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