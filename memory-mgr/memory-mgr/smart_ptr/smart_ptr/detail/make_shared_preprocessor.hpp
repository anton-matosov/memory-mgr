//  This header intentionally has no include guards.
//  
//  This file is a preprocessor version of the make_shared.hpp
//  
//  make_shared_preprocessor.hpp
//
//  Copyright (c) 2011 Anton (shikin) Matosov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org/libs/smart_ptr/make_shared.html
//  for documentation.

#if BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

#define MAKE_SHARED_TEMPLATE_PARMS BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A)

#define MAKE_SHARED_PARM(J,I,D) const BOOST_PP_CAT(A,I) &BOOST_PP_CAT(a,I)

#define MAKE_SHARED_PARMS BOOST_PP_ENUM(n,MAKE_SHARED_PARM,BOOST_PP_EMPTY)

#define MAKE_SHARED_PARMS_TRAILING BOOST_PP_ENUM_TRAILING(n,MAKE_SHARED_PARM,BOOST_PP_EMPTY)

#define MAKE_SHARED_ARGS BOOST_PP_ENUM_PARAMS(n, a)

#define MAKE_SHARED_ARGS_TRAILING BOOST_PP_ENUM_TRAILING_PARAMS(n, a)

template< typename T, typename Alloc MAKE_SHARED_TEMPLATE_PARMS >
memory_mgr::shared_ptr< T > allocate_shared( const Alloc & alloc BOOST_PP_COMMA_IF(n) MAKE_SHARED_PARMS )
{
	memory_mgr::shared_ptr< T > pt( static_cast< T* >( 0 ), memory_mgr::detail::sp_ms_deleter< T >(), alloc );

	memory_mgr::detail::sp_ms_deleter< T > * pd = memory_mgr::get_deleter< memory_mgr::detail::sp_ms_deleter< T > >( pt );

	void * pv = pd->address();

	::new( pv ) T(MAKE_SHARED_ARGS);
	pd->set_initialized();

	T * pt2 = static_cast< T* >( pv );

	memory_mgr::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );
	return memory_mgr::shared_ptr< T >( pt, pt2 );
}

//Regular Memory Manager version
template< typename T, class MemMgr MAKE_SHARED_TEMPLATE_PARMS>
memory_mgr::shared_ptr< T > make_shared(MemMgr& mgr MAKE_SHARED_PARMS_TRAILING)
{							
	memory_mgr::member_allocator<T, MemMgr> alloc( &mgr );
	return memory_mgr::allocate_shared<T>( alloc MAKE_SHARED_ARGS_TRAILING );
}

//Singleton Memory Manager version
template< typename T, class MemMgr MAKE_SHARED_TEMPLATE_PARMS>
memory_mgr::shared_ptr< T > make_shared(MAKE_SHARED_PARMS)
{																	
	return memory_mgr::allocate_shared<T>( memory_mgr::allocator<T, MemMgr>() MAKE_SHARED_ARGS_TRAILING );
}

#undef MAKE_SHARED_TEMPLATE_PARMS
#undef MAKE_SHARED_TEMPLATE_ARGS
#undef MAKE_SHARED_PARM
#undef MAKE_SHARED_PARMS
#undef MAKE_SHARED_ARGS

#undef n


#else //BOOST_PP_IS_ITERATING


#ifndef MAKE_SHARED_PREPROCESSOR_HPP_INCLUDED
#define MAKE_SHARED_PREPROCESSOR_HPP_INCLUDED


#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define MAKE_SHARED_NUM_ARGS 10

#define BOOST_PP_ITERATION_LIMITS (0, MAKE_SHARED_NUM_ARGS - 1)
#define BOOST_PP_FILENAME_1       <memory-mgr/smart_ptr/smart_ptr/detail/make_shared_preprocessor.hpp>
#include BOOST_PP_ITERATE()

#endif //MAKE_SHARED_PREPROCESSOR_HPP_INCLUDED

#endif  //BOOST_PP_IS_ITERATING
