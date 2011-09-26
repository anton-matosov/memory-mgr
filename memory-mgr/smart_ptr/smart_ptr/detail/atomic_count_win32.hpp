#ifndef MEMORY_MGR_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
#define MEMORY_MGR_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/atomic_count_win32.hpp
//
//  Copyright (c) 2001-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/detail/interlocked.hpp>

namespace memory_mgr
{

namespace detail
{

class atomic_count
{
public:

    explicit atomic_count( long v ): value_( v )
    {
    }

    long operator++()
    {
        return BOOST_INTERLOCKED_INCREMENT( &value_ );
    }

    long operator--()
    {
        return BOOST_INTERLOCKED_DECREMENT( &value_ );
    }

	long operator+=(long add)
	{
		return BOOST_INTERLOCKED_EXCHANGE_ADD( &value_, add );
	}

	long operator-=(long substract)
	{
		return BOOST_INTERLOCKED_EXCHANGE_ADD( &value_, -substract );
	}

    operator long() const
    {
        return static_cast<long const volatile &>( value_ );
    }

private:

    atomic_count( atomic_count const & );
    atomic_count & operator=( atomic_count const & );

    long value_;
};

} // namespace detail

} // namespace memory_mgr

#endif // #ifndef MEMORY_MGR_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
