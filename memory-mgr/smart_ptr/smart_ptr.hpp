//
//  This file is the adaptation for Generic Memory Manager library
//  smart_ptr.hpp
//
//  For convenience, this header includes the rest of the smart
//  pointer library headers.
//
//  Copyright (c) 2003 Peter Dimov  Distributed under the Boost
//  (C) Copyright Anton Matosov 2011.
//
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/smart_ptr.htm
//

#include <boost/config.hpp>

#include <memory-mgr/smart_ptr/scoped_ptr.hpp>
#include <memory-mgr/smart_ptr/scoped_array.hpp>
#include <memory-mgr/smart_ptr/shared_ptr.hpp>
#include <memory-mgr/smart_ptr/shared_array.hpp>

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
# include <memory-mgr/smart_ptr/weak_ptr.hpp>
# include <memory-mgr/smart_ptr/intrusive_ptr.hpp>
# include <memory-mgr/smart_ptr/enable_shared_from_this.hpp>
#endif
