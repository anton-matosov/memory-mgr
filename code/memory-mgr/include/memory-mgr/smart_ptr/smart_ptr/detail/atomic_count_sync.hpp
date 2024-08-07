//
//  boost/detail/atomic_count_sync.hpp
//
//  atomic_count for g++ 4.1+
//
//  http://gcc.gnu.org/onlinedocs/gcc-4.1.1/gcc/Atomic-Builtins.html
//
//  Copyright 2007 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#if defined( __ia64__ ) && defined( __INTEL_COMPILER )
# include <ia64intrin.h>
#endif

namespace memory_mgr
{

namespace detail
{

class atomic_count
{
public:

    explicit atomic_count( long v ) : value_( v ) {}

    long operator++()
    {
        return __sync_add_and_fetch( &value_, 1 );
    }

    long operator--()
    {
        return __sync_add_and_fetch( &value_, -1 );
    }

	long operator+=(long add)
	{
		return __sync_add_and_fetch( &value_, add );
	}

	long operator-=(long substract)
	{
		return __sync_add_and_fetch( &value_, -substract );
	}

    operator long() const
    {
        return __sync_fetch_and_add( &value_, 0 );
    }

private:

    atomic_count(atomic_count const &);
    atomic_count & operator=(atomic_count const &);

    mutable long value_;
};

} // namespace detail

} // namespace memory_mgr

