#pragma once

//  detail/sp_convertible.hpp
//
//  Copyright 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>

#if !defined( BOOST_SP_NO_SP_CONVERTIBLE ) && defined( BOOST_NO_SFINAE )
# define BOOST_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( BOOST_SP_NO_SP_CONVERTIBLE ) && defined( __GNUC__ ) && ( __GNUC__ * 100 + __GNUC_MINOR__ < 303 )
# define BOOST_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( BOOST_SP_NO_SP_CONVERTIBLE ) && defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x630 )
# define BOOST_SP_NO_SP_CONVERTIBLE
#endif

#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

namespace memory_mgr
{

namespace detail
{

template< class Y, class T > struct sp_convertible
{
    typedef char (&yes) [1];
    typedef char (&no)  [2];

    static yes f( T* );
    static no  f( ... );

    enum _vt { value = sizeof( (f)( static_cast<Y*>(0) ) ) == sizeof(yes) };
};

struct sp_empty
{
};

template< bool > struct sp_enable_if_convertible_impl;

template<> struct sp_enable_if_convertible_impl<true>
{
    typedef sp_empty type;
};

template<> struct sp_enable_if_convertible_impl<false>
{
};

template< class Y, class T >
struct sp_enable_if_convertible
	: public sp_enable_if_convertible_impl< sp_convertible< Y, T >::value >
{
};

} // namespace detail

} // namespace memory_mgr

#endif // !defined( BOOST_SP_NO_SP_CONVERTIBLE )

