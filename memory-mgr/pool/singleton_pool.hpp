// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

#ifndef MEMORY_MGR_SINGLETON_POOL_HPP
#define MEMORY_MGR_SINGLETON_POOL_HPP

#include <memory-mgr/pool/poolfwd.hpp>

// memory_mgr::pool
#include <memory-mgr/pool/pool.hpp>
// memory_mgr::details::pool::singleton_default
#include <memory-mgr/pool/detail/singleton.hpp>
// memory_mgr::details::pool::guard
#include <memory-mgr/pool/detail/guard.hpp>

namespace memory_mgr {

	//
	// The singleton_pool class allows other pool interfaces for types of the same
	//   size to share the same pool
	//
	template <typename Tag, unsigned RequestedSize,
		typename UserAllocator,
		typename Mutex,
		unsigned NextSize,
		unsigned MaxSize>
	struct singleton_pool
	{
	public:
		typedef Tag tag;
		typedef Mutex mutex;
		typedef UserAllocator user_allocator;
		typedef typename pool<UserAllocator>::size_type size_type;
		typedef typename pool<UserAllocator>::difference_type difference_type;

		BOOST_STATIC_CONSTANT(unsigned, requested_size = RequestedSize);
		BOOST_STATIC_CONSTANT(unsigned, next_size = NextSize);

	private:
		struct pool_type: Mutex
		{
			pool<UserAllocator> p;
			pool_type():p(RequestedSize, NextSize) { }
		};

		typedef details::pool::singleton_default<pool_type> singleton;

		singleton_pool();

	public:
		static void * malloc BOOST_PREVENT_MACRO_SUBSTITUTION()
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return (p.p.malloc)();
		}
		static void * ordered_malloc()
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return p.p.ordered_malloc();
		}
		static void * ordered_malloc(const size_type n)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return p.p.ordered_malloc(n);
		}
		static bool is_from(void * const ptr)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return p.p.is_from(ptr);
		}
		static void free BOOST_PREVENT_MACRO_SUBSTITUTION(void * const ptr)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			(p.p.free)(ptr);
		}
		static void ordered_free(void * const ptr)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			p.p.ordered_free(ptr);
		}
		static void free BOOST_PREVENT_MACRO_SUBSTITUTION(void * const ptr, const size_type n)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			(p.p.free)(ptr, n);
		}
		static void ordered_free(void * const ptr, const size_type n)
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			p.p.ordered_free(ptr, n);
		}
		static bool release_memory()
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return p.p.release_memory();
		}
		static bool purge_memory()
		{
			pool_type & p = singleton::instance();
			details::pool::guard<Mutex> g(p);
			return p.p.purge_memory();
		}
	};

} // namespace memory_mgr

#endif
