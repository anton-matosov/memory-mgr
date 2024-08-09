
#pragma once

#include "memory-mgr/detail/dynamic_cast_ex.h"
#include <assert.h>
//
//  mgr_deleter.hpp
//
//  Copyright (c) 2011 Anton Matosov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

namespace memory_mgr
{
	struct std_deleter
	{
		template<class T>
		void operator()( T* ptr )
		{
			delete ptr;
		}
	};

	template<class Allocator>
	struct allocator_deleter
	{
		template<class T>
		void operator()( T* ptr )
		{
			typename Allocator::template rebind<T>::other alloc;
			alloc.destroy( ptr );
			alloc.deallocate( ptr, sizeof(T) );
		}
	};

	template<class MemMgr>
	struct mgr_deleter
	{
		template<class T>
		void operator()( T* ptr )
		{
			(*ptr).~T();
			MemMgr::instance().deallocate( ptr );
		}
	};


	template<class MemMgr>
	struct mgr_delete_deleter
	{
		template<class T>
		void operator()( T* ptr )
		{
			::delete_<MemMgr>( ptr );
		}
	};
}

