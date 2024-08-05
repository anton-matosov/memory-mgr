/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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

#pragma once

#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW


#include <gstl/detail/boost_warnings_off.hpp>
//#define BOOST_TEST_NO_LIB
#include <boost/test/test_tools.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/mpl/list.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/foreach.hpp>
#include <gstl/detail/boost_warnings_on.hpp>

namespace ut = boost::unit_test;

#define GSTL_ARRAY_LEN( arr ) ( sizeof( arr ) / sizeof( *arr ) )
#define GSTL_ARRAY_END( arr ) ( arr + GSTL_ARRAY_LEN( arr ) )
#define GSTL_STR_LEN( s ) ( GSTL_ARRAY_LEN( s ) - 1 )


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
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};												\
	BOOST_AUTO_TU_REGISTRAR( test_name )(																\
	BOOST_PARAM_TEST_CASE( function, begin, end ),											\
	boost::unit_test::decorator::collector_t::instance() );

//#define GSTL_STATIC_ARR_END( arr )\
//	template<class T, T[]

template<class ValueT, class IterT>
void test_compare_n_values( IterT first, ValueT val, size_t len )
{
	while( len-- )
	{
		BOOST_CHECK_EQUAL( *first++, val );
		if( *first != val ) 
		{
			break;
		}
	}
}


namespace gstl
{
	template <class T, class Alloc, class PtrTraits, class StreamT,
		template <class,class,class> class Container>
	StreamT& operator<<( StreamT& stream, const Container<T, Alloc, PtrTraits>& vec )
	{
		BOOST_FOREACH( const T& val, vec )
		{
			stream << val << ' ';
		}
		return stream;
	}
}

namespace std
{
	template <class T, class Alloc, class StreamT, template <class,class> class Container>
	StreamT& operator<<( StreamT& stream, const Container<T, Alloc>& vec )
	{
		BOOST_FOREACH( const T& val, vec )
		{
			stream << val << ' ';
		}
		return stream;
	}
}

namespace boost
{
	//Required by boost::unit_test to print test log
	template <class T, class StreamT>
	StreamT& operator<<( StreamT& stream, const reverse_iterator<T>& iter )
	{
		stream << *iter;
		return stream;
	}
}

