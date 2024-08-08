#pragma once

//  This file is the adaptation for Generic Memory Manager library
//
//  make_shared.hpp
//
//  Copyright (c) 2007, 2008 Peter Dimov
//  Copyright (c) 2011 Anton (shikin) Matosov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/smart_ptr/make_shared.html
//  for documentation.

#include <boost/config.hpp>
#include "memory-mgr/offset_ptr.h"
#include "memory-mgr/allocator.h"
#include "memory-mgr/smart_ptr/smart_ptr/shared_ptr.hpp"
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <cstddef>

namespace memory_mgr
{

namespace detail
{

template< std::size_t N, std::size_t A > struct sp_aligned_storage
{
    union type
    {
        char data_[ N ];
        typename boost::type_with_alignment< A >::type align_;
    };
};

template< class T > class sp_ms_deleter
{
private:

    typedef typename sp_aligned_storage< sizeof( T ), ::boost::alignment_of< T >::value >::type storage_type;

    bool initialized_;
    storage_type storage_;

private:

    void destroy()
    {
        if( initialized_ )
        {
            reinterpret_cast< T* >( storage_.data_ )->~T();
            initialized_ = false;
        }
    }

public:

    sp_ms_deleter(): initialized_( false )
    {
    }

    // optimization: do not copy storage_
    sp_ms_deleter( sp_ms_deleter const & ): initialized_( false )
    {
    }

    ~sp_ms_deleter()
    {
        destroy();
    }

    void operator()( T * )
    {
        destroy();
    }

    void * address()
    {
        return storage_.data_;
    }

    void set_initialized()
    {
        initialized_ = true;
    }
};

#if defined( BOOST_HAS_RVALUE_REFS )

template< class T > T&& sp_forward( T & t )
{
    return static_cast< T&& >( t );
}

#endif

} // namespace detail


#if defined( BOOST_HAS_VARIADIC_TMPL ) && defined( BOOST_HAS_RVALUE_REFS )

// Variadic templates, rvalue reference

template< class T, class Alloc, class... Args > memory_mgr::shared_ptr< T > allocate_shared( Alloc const & a, Args && ... args )
{
    memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), a );

    memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    ::new( pv ) T( memory_mgr::detail::sp_forward<Args>( args )... );
    pd->set_initialized();

    T * pt2 = static_cast< T* >( pv );

    memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
    return memory_mgr::shared_ptr< T >( pt, pt2 );
}

template< class T, class MemMgr , class... Args>
shared_ptr< T > make_shared(MemMgr& mgr , Args && ... args)
{
	memory_mgr::member_allocator<T, MemMgr> alloc( &mgr );
	return memory_mgr::allocate_shared<T>( alloc , memory_mgr::detail::sp_forward<Args>( args )... );
}

template< class T, class MemMgr , class... Args>
shared_ptr< T > make_shared( Args && ... args)
{
    memory_mgr::member_allocator<T, MemMgr> alloc;
	return memory_mgr::allocate_shared<T>( alloc , memory_mgr::detail::sp_forward<Args>( args )... );
}

#else

// C++03 version
#include "memory-mgr/smart_ptr/smart_ptr/detail/make_shared_pp_pregenerated.hpp"

#endif

} // namespace memory_mgr

