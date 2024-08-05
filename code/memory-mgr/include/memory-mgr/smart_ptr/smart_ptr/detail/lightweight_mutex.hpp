#pragma once

//  This file is the adaptation for Generic Memory Manager library
//
//  boost/detail/lightweight_mutex.hpp - lightweight mutex
//
//  Copyright (c) 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2011 Anton (shikin) Matosov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  typedef <unspecified> memory_mgr::detail::lightweight_mutex;
//
//  memory_mgr::detail::lightweight_mutex is a header-only implementation of
//  a subset of the Mutex concept requirements:
//
//  http://www.boost.org/doc/html/threads/concepts.html#threads.concepts.Mutex
//
//  It maps to a CRITICAL_SECTION on Windows or a pthread_mutex on POSIX.
//

#include <boost/config.hpp>

#if !defined(BOOST_HAS_THREADS)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/lwm_nop.hpp"
#elif defined(BOOST_HAS_PTHREADS)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/lwm_pthreads.hpp"
#elif defined(BOOST_HAS_WINTHREADS) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/lwm_win32_cs.hpp"
#else
// Use #define BOOST_DISABLE_THREADS to avoid the error
#  error Unrecognized threading platform
#endif

