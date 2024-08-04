#pragma once

//
//  boost/detail/spinlock.hpp
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  struct spinlock
//  {
//      void lock();
//      bool try_lock();
//      void unlock();
//
//      class scoped_lock;
//  };
//
//  #define MGR_DETAIL_SPINLOCK_INIT <unspecified>
//

#include <boost/config.hpp>
#include "memory-mgr/smart_ptr/smart_ptr/detail/sp_has_sync.hpp"

#if defined(__GNUC__) && defined( __arm__ ) && !defined( __thumb__ )
#  include "memory-mgr/smart_ptr/smart_ptr/detail/spinlock_gcc_arm.hpp"

#elif defined( BOOST_SP_HAS_SYNC )
#  include "memory-mgr/smart_ptr/smart_ptr/detail/spinlock_sync.hpp"

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/spinlock_w32.hpp"

#elif defined(BOOST_HAS_PTHREADS)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/spinlock_pt.hpp"

#elif !defined(BOOST_HAS_THREADS)
#  include "memory-mgr/smart_ptr/smart_ptr/detail/spinlock_nt.hpp"

#else
#  error Unrecognized threading platform
#endif

