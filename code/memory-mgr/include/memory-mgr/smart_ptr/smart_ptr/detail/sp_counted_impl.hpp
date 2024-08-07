#pragma once

//  This file is the adaptation for Generic Memory Manager library
//
//  detail/sp_counted_impl.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//  Copyright (c) 2011 Anton (shikin) Matosov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)
# error BOOST_SP_USE_QUICK_ALLOCATOR is not supported by the current addoptation.
#endif

#include "memory-mgr/offset_ptr.h"
#include <boost/checked_delete.hpp>
#include "memory-mgr/smart_ptr/smart_ptr/detail/sp_counted_base.hpp"


#include <memory>           // std::allocator
#include <cstddef>          // std::size_t

namespace memory_mgr
{

#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)

void sp_scalar_constructor_hook( void * px, std::size_t size, void * pn );
void sp_scalar_destructor_hook( void * px, std::size_t size, void * pn );

#endif

namespace detail
{

template<class X>
class sp_counted_impl_p
	: public sp_counted_base
{
private:

	typedef offset_ptr<X> pointer;
    pointer px_;

    sp_counted_impl_p( sp_counted_impl_p const & );
    sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

    typedef sp_counted_impl_p<X> this_type;

public:

    explicit sp_counted_impl_p( pointer px )
		: px_( px )
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        memory_mgr::sp_scalar_constructor_hook( &*px, sizeof(X), this );
#endif
    }

    virtual void dispose() // nothrow
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        memory_mgr::sp_scalar_destructor_hook( &*px_, sizeof(X), this );
#endif
        boost::checked_delete( &*px_ );
    }

	virtual void * get_deleter( memory_mgr::detail::sp_typeinfo const & )
    {
        return 0;
    }
};

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D>
class sp_counted_impl_pd
	: public sp_counted_base
{
private:

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    sp_counted_impl_pd( sp_counted_impl_pd const & );
    sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

    typedef sp_counted_impl_pd<P, D> this_type;

public:

    // pre: d(p) must not throw

    sp_counted_impl_pd( P p, D d ): ptr(p), del(d)
    {
    }

    virtual void dispose() // nothrow
    {
        del( &*ptr );
    }

    virtual void * get_deleter( memory_mgr::detail::sp_typeinfo const & ti )
    {
        return ti == BOOST_SP_TYPEID(D)? &reinterpret_cast<char&>( del ): 0;
    }
};

template<class P, class D, class A>
class sp_counted_impl_pda
	: public sp_counted_base
{
private:

    P p_; // copy constructor must not throw
    D d_; // copy constructor must not throw
    A a_; // copy constructor must not throw

    sp_counted_impl_pda( sp_counted_impl_pda const & );
    sp_counted_impl_pda & operator= ( sp_counted_impl_pda const & );

    typedef sp_counted_impl_pda<P, D, A> this_type;

public:

    // pre: d( p ) must not throw

    sp_counted_impl_pda( P p, D d, A a ): p_( p ), d_( d ), a_( a )
    {
    }

    virtual void dispose() // nothrow
    {
        d_( &*p_ );
    }

    virtual void destroy() // nothrow
    {
        typedef typename A::template rebind< this_type >::other A2;

        A2 a2( a_ );

        this->~this_type();
        a2.deallocate( this, 1 );
    }

    virtual void * get_deleter( memory_mgr::detail::sp_typeinfo const & ti )
    {
        return ti == BOOST_SP_TYPEID( D )? &reinterpret_cast<char&>( d_ ): 0;
    }
};

#ifdef __CODEGUARD__
# pragma option pop
#endif

} // namespace detail

} // namespace memory_mgr

