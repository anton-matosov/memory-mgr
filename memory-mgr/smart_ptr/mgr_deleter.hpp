#ifndef MEMORY_MGR_DELETER_HPP_INCLUDED
#define MEMORY_MGR_DELETER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

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
}

#endif //MEMORY_MGR_DELETER_HPP_INCLUDED