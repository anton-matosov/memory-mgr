// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

#ifndef MEMORY_MGR_POOLFWD_HPP
#define MEMORY_MGR_POOLFWD_HPP

//#include <boost/config.hpp> // for workarounds

// std::size_t
#include <cstddef>

// memory_mgr::details::pool::default_mutex
#include <memory-mgr/pool/detail/mutex.hpp>
#include <memory-mgr/offset_ptr.h>

namespace memory_mgr {

	//
	// Location: <memory-mgr/pool/simple_segregated_storage.hpp>
	//
	template <typename SizeType = std::size_t, class Pointer = offset_ptr<void> >
	class simple_segregated_storage;

	//
	// Location: <memory-mgr/pool/pool.hpp>
	//
	struct default_user_allocator_new_delete;
	struct default_user_allocator_malloc_free;

	template <typename UserAllocator = default_user_allocator_new_delete>
	class pool;

	//
	// Location: <memory-mgr/pool/object_pool.hpp>
	//
	template <typename T, typename UserAllocator = default_user_allocator_new_delete>
	class object_pool;

	//
	// Location: <memory-mgr/pool/singleton_pool.hpp>
	//
	template <typename Tag, unsigned RequestedSize,
		typename UserAllocator = default_user_allocator_new_delete,
		typename Mutex = details::pool::default_mutex,
		unsigned NextSize = 32,
		unsigned MaxSize = 0>
	struct singleton_pool;

	//
	// Location: <memory-mgr/pool/pool_alloc.hpp>
	//
	struct pool_allocator_tag;

	template <typename T,
		typename UserAllocator = default_user_allocator_new_delete,
		typename Mutex = details::pool::default_mutex,
		unsigned NextSize = 32,
		unsigned MaxSize = 0>
	class pool_allocator;

	struct fast_pool_allocator_tag;

	template <typename T,
		typename UserAllocator = default_user_allocator_new_delete,
		typename Mutex = details::pool::default_mutex,
		unsigned NextSize = 32,
		unsigned MaxSize = 0>
	class fast_pool_allocator;

} // namespace memory_mgr

#endif
