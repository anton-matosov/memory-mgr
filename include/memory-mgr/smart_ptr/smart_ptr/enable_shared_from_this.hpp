#ifndef MEMORY_MGR_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define MEMORY_MGR_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//  This file is the adaptation for Generic Memory Manager library
//
//  enable_shared_from_this.hpp
//
//  Copyright 2002, 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/smart_ptr/enable_shared_from_this.html
//

#include <memory-mgr/smart_ptr/smart_ptr/weak_ptr.hpp>
#include <memory-mgr/smart_ptr/smart_ptr/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace memory_mgr
{

template<class T> class enable_shared_from_this
{
protected:

    enable_shared_from_this()
    {
    }

    enable_shared_from_this(enable_shared_from_this const &)
    {
    }

    enable_shared_from_this & operator=(enable_shared_from_this const &)
    {
        return *this;
    }

    ~enable_shared_from_this()
    {
    }

public:

    shared_ptr<T> shared_from_this()
    {
        shared_ptr<T> p( weak_this_ );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

    shared_ptr<T const> shared_from_this() const
    {
        shared_ptr<T const> p( weak_this_ );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

public: // actually private, but avoids compiler template friendship issues

    // Note: invoked automatically by shared_ptr; do not call
    template<class X, class Y> void _internal_accept_owner( shared_ptr<X> const * ppx, Y * py ) const
    {
        if( weak_this_.expired() )
        {
            weak_this_ = shared_ptr<T>( *ppx, py );
        }
    }

private:

    mutable weak_ptr<T> weak_this_;
};

} // namespace memory_mgr

#endif  // #ifndef MEMORY_MGR_SMART_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
