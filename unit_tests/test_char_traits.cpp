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
#include <gstl/detail/char_traits.hpp>

typedef gstl::char_traits<char> traits_type;

#define STR_LEN( s ) ( ( sizeof( s ) / sizeof( char ) ) - 1 )

namespace
{
	const char ch = 'c';

	const char* s1 = "Hello";
	const size_t s1_len = STR_LEN( s1 );
	const char c1_exist = 'l';
	const char c1_unexist = 'd';

	const char* s2 = "HelLo"; //Lesser then s1
	const size_t s2_len = STR_LEN( s2 );

	const char* s3 = "World";
	const size_t s3_len = STR_LEN( s3 );

	const size_t max_len = 256;
}

template<class T>
const T lesser( const T v1, const T v2 )
{
	return v1 < v2 ? v1 : v2;
}

class char_traits_test
	:public CppUnit::TestFixture
{
	char* m_s1;
	char* m_s2;
	char* m_s3;
	char* m_s4;
	char m_c1;
public:
	void setUp()
	{
		m_c1 = 'a';
		m_s1 = new char[ max_len ];
		m_s2 = new char[ max_len ];
		m_s3 = new char[ max_len ];
		m_s4 = new char[ max_len ];

	}

	void tearDown()
	{
		delete[] m_s1;
		delete[] m_s2;
		delete[] m_s3;
		delete[] m_s4;
	}

	void test_assign()
	{
		traits_type::assign( m_c1, ch );
		CPPUNIT_ASSERT( m_c1 == ch );
	}

	void test_eq()
	{
		m_c1 = ch;
		CPPUNIT_ASSERT( m_c1 == ch );
		CPPUNIT_ASSERT( traits_type::eq( m_c1, ch ) );
	}

	void test_lt()
	{
		m_c1 = ch - 1;

		CPPUNIT_ASSERT( m_c1 < ch );
		CPPUNIT_ASSERT( traits_type::lt( m_c1, ch ) );
	}

	void test_compare()
	{
		CPPUNIT_ASSERT( s1_len == s2_len );
		CPPUNIT_ASSERT( traits_type::compare( s1, s1, s1_len ) == 0 );
		CPPUNIT_ASSERT( traits_type::compare( s1, s2, s1_len ) > 0 );
		CPPUNIT_ASSERT( traits_type::compare( s2, s1, s1_len ) < 0 );
	}

	void test_length()
	{
		CPPUNIT_ASSERT( traits_type::length( s1 ) == strlen( s1 ) );
		CPPUNIT_ASSERT( traits_type::length( s2 ) == strlen( s2 ) );
		CPPUNIT_ASSERT( traits_type::length( s3 ) == strlen( s3 ) );
	}

	void test_find()
	{
		const traits_type::char_type* fres = traits_type::find( s1, s1_len, c1_exist );
		CPPUNIT_ASSERT( fres != 0 );
		CPPUNIT_ASSERT( *fres == c1_exist );
		CPPUNIT_ASSERT( fres == memchr( s1, c1_exist, s1_len ) );

		const traits_type::char_type* fres2 = traits_type::find( s1, 0, c1_exist );
		CPPUNIT_ASSERT( fres2 == 0 );

		const traits_type::char_type* fres3 = traits_type::find( s1, s1_len, c1_unexist );
		CPPUNIT_ASSERT( fres3 == 0 );
	}

	void test_move()
	{
		traits_type::move( m_s1, s1, s1_len );
		traits_type::move( m_s2, s2, s2_len );
		traits_type::move( m_s3, s3, s3_len );

		CPPUNIT_ASSERT( traits_type::compare( m_s1, s1, s1_len ) == 0 );
		CPPUNIT_ASSERT( traits_type::compare( m_s2, s2, s2_len ) == 0 );
		CPPUNIT_ASSERT( traits_type::compare( m_s3, s3, s3_len ) == 0 );
	}

	void test_copy()
	{
		traits_type::copy( m_s1, s1, s1_len );
		traits_type::copy( m_s2, s2, s2_len );
		traits_type::copy( m_s3, s3, s3_len );

		CPPUNIT_ASSERT( traits_type::compare( m_s1, s1, s1_len ) == 0 );
		CPPUNIT_ASSERT( traits_type::compare( m_s2, s2, s2_len ) == 0 );
		CPPUNIT_ASSERT( traits_type::compare( m_s3, s3, s3_len ) == 0 );
	}

	void test_assign_str()
	{
		traits_type::assign( m_s1, m_c1, s1_len );

		size_t i = s1_len;
		const traits_type::char_type* s = m_s1; 
		while( i-- )
		{
			CPPUNIT_ASSERT( traits_type::eq( *s++, m_c1 ) == 0 );
		}
	}

	void test_eq_int_type()
	{
		m_c1 = ch;
		CPPUNIT_ASSERT( m_c1 == ch );
		CPPUNIT_ASSERT( traits_type::eq( m_c1, ch ) );

		CPPUNIT_ASSERT( traits_type::eq_int_type( traits_type::to_int_type(m_c1),
			traits_type::to_int_type(ch) ) );
	}

	void test_not_eof()
	{
		traits_type::int_type eof = traits_type::eof();
		traits_type::int_type not_eof = traits_type::to_int_type( 'A' );
		CPPUNIT_ASSERT( traits_type::not_eof( eof ) != eof );
		CPPUNIT_ASSERT( traits_type::not_eof( not_eof ) == not_eof );
	}

	//////////////////////////////////////////////////////////////////////////
	CPPUNIT_TEST_SUITE( char_traits_test );
	CPPUNIT_TEST( test_assign );
	CPPUNIT_TEST( test_eq );
	CPPUNIT_TEST( test_lt );
	CPPUNIT_TEST( test_length );
	CPPUNIT_TEST( test_compare );
	CPPUNIT_TEST( test_find );
	CPPUNIT_TEST( test_move );
	CPPUNIT_TEST( test_assign_str );
	CPPUNIT_TEST( test_eq_int_type );
	CPPUNIT_TEST( test_not_eof );
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( char_traits_test );