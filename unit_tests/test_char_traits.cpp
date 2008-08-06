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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#include "stdafx.h"
#include <gstl/detail/char_traits.hpp>

typedef gstl::char_traits<char> traits_type;


namespace
{
	const char ch = 'c';

	const char s1[] = "Hello";
	const size_t s1_len = STR_LEN( s1 );
	const char c1_exist = 'l';
	const char c1_unexist = 'd';

	const char s2[] = "HelLo"; //Lesser then s1
	const size_t s2_len = STR_LEN( s2 );

	const char s3[] = "World";
	const size_t s3_len = STR_LEN( s3 );

	const char s4[] = "Goodb";
	const size_t s4_len = STR_LEN( s4 );

	const size_t max_len = 256;
}

class char_traits_test_fixture
{
public:
	char* m_s1;
	char* m_s2;
	char* m_s3;
	char* m_s4;
	char m_c1;

	char_traits_test_fixture()
	{
		m_c1 = 'a';
		m_s1 = new char[ max_len ];
		m_s2 = new char[ max_len ];
		m_s3 = new char[ max_len ];
		m_s4 = new char[ max_len ];
	}

	~char_traits_test_fixture()
	{
		delete[] m_s1;
		delete[] m_s2;
		delete[] m_s3;
		delete[] m_s4;
	}
};


BOOST_FIXTURE_TEST_SUITE( char_traits_test, char_traits_test_fixture )

	BOOST_AUTO_TEST_CASE(test_assign)
	{
		traits_type::assign( m_c1, ch );
		BOOST_CHECK_EQUAL( m_c1, ch );
	}

	BOOST_AUTO_TEST_CASE(test_eq)
	{
		m_c1 = ch;
		BOOST_CHECK( m_c1 == ch );
		BOOST_CHECK( traits_type::eq( m_c1, ch ) );
	}

	BOOST_AUTO_TEST_CASE(test_lt)
	{
		m_c1 = ch - 1;

		BOOST_CHECK( m_c1 < ch );
		BOOST_CHECK( traits_type::lt( m_c1, ch ) );
	}
	typedef boost::tuple<const char*, size_t, const char*, size_t> test_compare_param_type;
	test_compare_param_type params[] = {
		boost::make_tuple( s1, s1_len, s2, s2_len ),
		boost::make_tuple( s1, s1_len, s3, s3_len ),
		boost::make_tuple( s1, s1_len, s4, s4_len ),
		boost::make_tuple( s2, s2_len, s3, s3_len ),
		boost::make_tuple( s2, s2_len, s4, s4_len ),
		boost::make_tuple( s3, s3_len, s4, s4_len ),
	};
	void test_compare_impl( test_compare_param_type params )
	{
		const char *str1 = 0, *str2 = 0;
		size_t size1 = 0, size2 = 0;
		boost::tie( str1, size1, str2, size2 ) = params;

		BOOST_CHECK( size1 == size2 );
		
		BOOST_CHECK_EQUAL( traits_type::compare( str1, str1, size1 ), memcmp( str1, str1, size1 ) );
		BOOST_CHECK_EQUAL( traits_type::compare( str1, str2, size1 ), memcmp( str1, str2, size1 ) );
		BOOST_CHECK_EQUAL( traits_type::compare( str2, str1, size1 ), memcmp( str2, str1, size1 ) );
	}

	GSTL_AUTO_PARAMS_TEST_CASE( test_compare, test_compare_impl,
		params, params + sizeof(params)/sizeof(test_compare_param_type) );


	BOOST_AUTO_TEST_CASE(test_length)
	{
		BOOST_CHECK( traits_type::length( s1 ) == strlen( s1 ) );
		BOOST_CHECK( traits_type::length( s2 ) == strlen( s2 ) );
		BOOST_CHECK( traits_type::length( s3 ) == strlen( s3 ) );
	}
	BOOST_AUTO_TEST_CASE(test_find)
	{
		const traits_type::char_type* fres = traits_type::find( s1, s1_len, c1_exist );
		BOOST_CHECK( fres != 0 );
		BOOST_CHECK( *fres == c1_exist );
		BOOST_CHECK( fres == memchr( s1, c1_exist, s1_len ) );

		const traits_type::char_type* fres2 = traits_type::find( s1, 0, c1_exist );
		BOOST_CHECK( fres2 == 0 );

		const traits_type::char_type* fres3 = traits_type::find( s1, s1_len, c1_unexist );
		BOOST_CHECK( fres3 == 0 );
	}

	BOOST_AUTO_TEST_CASE(test_move)
	{
		traits_type::move( m_s1, s1, s1_len + 1 );
		traits_type::move( m_s2, s2, s2_len + 1  );
		traits_type::move( m_s3, s3, s3_len + 1  );

		BOOST_CHECK_EQUAL( m_s1, s1 );
		BOOST_CHECK_EQUAL( m_s2, s2 );
		BOOST_CHECK_EQUAL( m_s3, s3 );
	}

	BOOST_AUTO_TEST_CASE(test_copy)
	{
		traits_type::copy( m_s1, s1, s1_len + 1 );
		traits_type::copy( m_s2, s2, s2_len + 1 );
		traits_type::copy( m_s3, s3, s3_len + 1 );

		BOOST_CHECK_EQUAL( m_s1, s1 );
		BOOST_CHECK_EQUAL( m_s2, s2 );
		BOOST_CHECK_EQUAL( m_s3, s3 );
	}

	BOOST_AUTO_TEST_CASE(test_assign_str)
	{
		traits_type::assign( m_s1, s1_len, m_c1 );
 
		test_compare_n_chars( m_s1, m_c1, s1_len );
	}

	BOOST_AUTO_TEST_CASE(test_eq_int_type)
	{
		m_c1 = ch;
		BOOST_CHECK( m_c1 == ch );
		BOOST_CHECK( traits_type::eq( m_c1, ch ) );

		BOOST_CHECK( traits_type::eq_int_type( traits_type::to_int_type(m_c1),
			traits_type::to_int_type(ch) ) );
	}

	BOOST_AUTO_TEST_CASE(test_not_eof)
	{
		traits_type::int_type eof = traits_type::eof();
		traits_type::int_type not_eof = traits_type::to_int_type( 'A' );
		BOOST_CHECK( traits_type::not_eof( eof ) != eof );
		BOOST_CHECK( traits_type::not_eof( not_eof ) == not_eof );
	}


BOOST_AUTO_TEST_SUITE_END()


