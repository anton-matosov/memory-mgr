#pragma once
//  This file is the adaptation for Generic Memory Manager library
//
//  Copyright (c) 2008 Peter Dimov
//  Copyright (c) 2011 Anton (shikin) Matosov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "memory-mgr/smart_ptr/smart_ptr/detail/yield_k.hpp"

namespace memory_mgr
{

namespace detail
{

class spinlock
{
public:

    int v_;

public:

    bool try_lock()
    {
        int r;

        __asm__ __volatile__(
            "swp %0, %1, [%2]":
            "=&r"( r ): // outputs
            "r"( 1 ), "r"( &v_ ): // inputs
            "memory", "cc" );

        return r == 0;
    }

    void lock()
    {
        for( unsigned k = 0; !try_lock(); ++k )
        {
            memory_mgr::detail::yield( k );
        }
    }

    void unlock()
    {
        __asm__ __volatile__( "" ::: "memory" );
        *const_cast< int volatile* >( &v_ ) = 0;
    }

public:

    class scoped_lock
    {
    private:

        spinlock & sp_;

        scoped_lock( scoped_lock const & );
        scoped_lock & operator=( scoped_lock const & );

    public:

        explicit scoped_lock( spinlock & sp ): sp_( sp )
        {
            sp.lock();
        }

        ~scoped_lock()
        {
            sp_.unlock();
        }
    };
};

} // namespace detail
} // namespace memory_mgr

#define MGR_DETAIL_SPINLOCK_INIT {0}

