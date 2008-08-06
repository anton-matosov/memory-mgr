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

#ifndef GSTL_TEST_COMMON_HEADER
#define GSTL_TEST_COMMON_HEADER

#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW


//#define BOOST_TEST_NO_LIB
#include <boost/test/test_tools.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/mpl/list.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>


namespace ut = boost::unit_test;

#define STR_LEN( s ) ( ( sizeof( s ) / sizeof( char ) ) - 1 )


#define GSTL_TEST_CASE_TEMPLATE_TWO_LISTS( name, type_name1, type_name2, type_list_1, type_list_2 )	\
template<class type_name1>																			\
class auto_templ_test_impl##name																	\
{																									\
public:																								\
	template<class type_name2>																		\
	void operator()( const type_name2& /*v2*/ );													\
};																									\
																									\
BOOST_AUTO_TEST_CASE_TEMPLATE( name, type_name1, type_list_1 )										\
{																									\
	boost::mpl::for_each<type_list_2>( auto_templ_test_impl##name<type_name1>() );					\
}																									\
																									\
template<class type_name1>																			\
	template<class type_name2>																		\
	void auto_templ_test_impl##name<type_name1>::operator()( const type_name2& /*v2*/ )


#define GSTL_AUTO_PARAMS_TEST_CASE( test_name, function, begin, end )	\
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};											\
	BOOST_AUTO_TU_REGISTRAR( test_name )(												\
	BOOST_PARAM_TEST_CASE( function, begin, end ) );	

//#define GSTL_STATIC_ARR_END( arr )\
//	template<class T, T[]

template<class CharT>
void test_compare_n_chars( const CharT* str, CharT ch, size_t len )
{
	while( len-- && (*str == ch) )
	{
		BOOST_CHECK_EQUAL( *str++, ch );
	}
}

#endif //GSTL_TEST_COMMON_HEADER