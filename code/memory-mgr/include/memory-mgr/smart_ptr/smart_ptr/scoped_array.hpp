#pragma once

//  This file is the adaptation for Generic Memory Manager library
// 
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//  Copyright (c) 2011 Anton (shikin) Matosov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/scoped_array.htm
//

#include "memory-mgr/offset_ptr.h"
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/config.hpp>   // in case ptrdiff_t not in std

#include <boost/detail/workaround.hpp>

#include <cstddef>            // for std::ptrdiff_t

namespace memory_mgr
{

// Debug hooks

#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)

void sp_array_constructor_hook(void * p);
void sp_array_destructor_hook(void * p);

#endif

//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
//  is guaranteed, either on destruction of the scoped_array or via an explicit
//  reset(). Use shared_array or std::vector if your needs are more complex.

template<class T> class scoped_array // noncopyable
{
private:

	typedef offset_ptr<T> pointer;
	pointer px;

    scoped_array(scoped_array const &);
    scoped_array & operator=(scoped_array const &);

    typedef scoped_array<T> this_type;

    void operator==( scoped_array const& ) const;
    void operator!=( scoped_array const& ) const;

public:

    typedef T element_type;

    explicit scoped_array( T * p = 0 ) // never throws
		: px( p )
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        memory_mgr::sp_array_constructor_hook( &*px );
#endif
    }

    ~scoped_array() // never throws
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
		memory_mgr::sp_array_destructor_hook( &*px );
#endif
		boost::checked_array_delete( &*px );
    }

    void reset(T * p = 0) // never throws
    {
        BOOST_ASSERT( p == 0 || p != get() ); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator[](std::ptrdiff_t i) const // never throws
    {
        BOOST_ASSERT(  !! px  );
        BOOST_ASSERT( i >= 0 );
        return px[i];
    }

    T * get() const // never throws
    {
        return &*px;
    }

// implicit conversion to "bool"
#include "memory-mgr/smart_ptr/smart_ptr/detail/operator_bool.hpp"

    void swap(scoped_array & b) // never throws
    {
        pointer tmp = b.px;
        b.px = px;
        px = tmp;
    }
};

template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace memory_mgr

