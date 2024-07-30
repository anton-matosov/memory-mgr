#ifndef MEMORY_MGR_SMART_PTR_DETAIL_SPINLOCK_SYNC_HPP_INCLUDED
#define MEMORY_MGR_SMART_PTR_DETAIL_SPINLOCK_SYNC_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//  This file is the adaptation for Generic Memory Manager library
//
//  Copyright (c) 2008 Peter Dimov
//  This file is the adaptation for Generic Memory Manager library
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "memory-mgr/smart_ptr/smart_ptr/detail/yield_k.hpp"

#if defined( __ia64__ ) && defined( __INTEL_COMPILER )
# include <ia64intrin.h>
#endif

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
        int r = __sync_lock_test_and_set( &v_, 1 );
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
        __sync_lock_release( &v_ );
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

#define BOOST_DETAIL_SPINLOCK_INIT {0}

#endif // #ifndef MEMORY_MGR_SMART_PTR_DETAIL_SPINLOCK_SYNC_HPP_INCLUDED
