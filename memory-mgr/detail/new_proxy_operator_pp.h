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

#define MGR_NEW_PROXY_CONSTRUCT_TEMPLATE_PARAMS\
	BOOST_PP_IF( n, template<, BOOST_PP_EMPTY() ) BOOST_PP_ENUM_PARAMS(n, typename A) BOOST_PP_IF( n, >, BOOST_PP_EMPTY() )

#define MGR_NEW_PROXY_CONSTRUCT_PARM(J,I,D) BOOST_PP_CAT(A,I) BOOST_PP_CAT(a,I)

#define MGR_NEW_PROXY_CONSTRUCT_PARMS BOOST_PP_ENUM(n,MGR_NEW_PROXY_CONSTRUCT_PARM,BOOST_PP_EMPTY)

#define MGR_NEW_PROXY_CONSTRUCT_ARGS BOOST_PP_ENUM_PARAMS(n, a)
										
	MGR_NEW_PROXY_CONSTRUCT_TEMPLATE_PARAMS
	object_pointer_type operator()(MGR_NEW_PROXY_CONSTRUCT_PARMS)
	{
		allocate();
		object_pointer_type object = m_object;

		if( m_alloc->construction_needed() )
		{
			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type(MGR_NEW_PROXY_CONSTRUCT_ARGS);
			}
		}				

		return m_object;
	}

#undef MGR_NEW_PROXY_CONSTRUCT_TEMPLATE_PARMS
#undef MGR_NEW_PROXY_CONSTRUCT_PARM
#undef MGR_NEW_PROXY_CONSTRUCT_PARMS
#undef MGR_NEW_PROXY_CONSTRUCT_ARGS
#undef n

#else //BOOST_PP_IS_ITERATING

#ifndef MGR_NEW_PROXY_OPERATOR_PP_INCLUDED
#define MGR_NEW_PROXY_OPERATOR_PP_INCLUDED

	/* Code to be generated if n==2
	template<class A1, class A2>
	object_pointer_type operator()( const A1& a1, const A2& a2 )
	{
		allocate();
		object_pointer_type object = m_object;

		if( m_alloc->construction_needed() )
		{
			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type( a1, a2 );
			}
		}

		return m_object;
	}
	 **/

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#define MGR_MAX_NEW_PARAMETERS 10

#define BOOST_PP_ITERATION_LIMITS (0, MGR_MAX_NEW_PARAMETERS)
#define BOOST_PP_FILENAME_1       <memory-mgr/detail/new_proxy_operator_pp.h>
#include BOOST_PP_ITERATE()

#endif //MGR_NEW_PROXY_OPERATOR_PP_INCLUDED

#endif  //BOOST_PP_IS_ITERATING
