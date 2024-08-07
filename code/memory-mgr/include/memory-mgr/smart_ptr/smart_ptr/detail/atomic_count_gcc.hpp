#pragma once
//
//  boost/detail/atomic_count_gcc.hpp
//
//  atomic_count for GNU libstdc++ v3
//
//  http://gcc.gnu.org/onlinedocs/porting/Thread-safety.html
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2002 Lars Gullik Bj�nnes <larsbj@lyx.org>
//  Copyright 2003-2005 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#if __GNUC__ * 100 + __GNUC_MINOR__ >= 402
# include <ext/atomicity.h> 
#else 
# include <bits/atomicity.h>
#endif

namespace memory_mgr
{

namespace detail
{

#if defined(__GLIBCXX__) // g++ 3.4+

using __gnu_cxx::__atomic_add;
using __gnu_cxx::__exchange_and_add;

#endif

class atomic_count
{
public:

    explicit atomic_count( long v ) : value_( v ) {}

    long operator++()
    {
        return __exchange_and_add( &value_, +1 ) + 1;
    }

    long operator--()
    {
        return __exchange_and_add( &value_, -1 ) - 1;
    }

	long operator+=(long add)
	{
		return __exchange_and_add( &value_, add ) + add;
	}

	long operator-=(long substract)
	{
		return __exchange_and_add( &value_, -substract ) - substract;
	}

    operator long() const
    {
        return __exchange_and_add( &value_, 0 );
    }

private:

    atomic_count(atomic_count const &);
    atomic_count & operator=(atomic_count const &);

    mutable _Atomic_word value_;
};

} // namespace detail

} // namespace memory_mgr

