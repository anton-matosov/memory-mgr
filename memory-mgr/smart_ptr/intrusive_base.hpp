#ifndef MEMORY_MGR_INTRUSIVE_BASE_HPP_INCLUDED
#define MEMORY_MGR_INTRUSIVE_BASE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  intrusive_base.hpp
//
//  Copyright (c) 2011 Anton Matosov
//  The initial idea belongs to Aleksandr Gutenev
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

#include <memory-mgr/smart_ptr/mgr_deleter.hpp>
#include <memory-mgr/smart_ptr/smart_ptr/detail/atomic_count.hpp>
#include <memory-mgr/smart_ptr/smart_ptr/intrusive_ptr.hpp>

namespace memory_mgr
{
	template
	<
		class Derived,
		class DeleteStrategy = ::memory_mgr::std_deleter,
		class CounterType = ::memory_mgr::detail::atomic_count,
		template <class> class IntrusivePtr = memory_mgr::intrusive_ptr
	>
	class intrusive_base
	{
	private:
		typedef intrusive_base self;

	public:
		IntrusivePtr<Derived> shared_from_this() const
		{
			Derived const* derived = static_cast<Derived const*>(this);
			return IntrusivePtr<Derived>( const_cast<Derived*>( derived ) );
		}

	protected:
		intrusive_base()
			: reference_counter_(0)
		{
		}

		intrusive_base(self const&)
			: reference_counter_(0)
		{
		}

		intrusive_base& operator=(self const&)
		{
			return *this;
		}

		~intrusive_base()
		{

		}

	private:
		friend void intrusive_ptr_add_ref(const Derived* p) 
		{
			++(static_cast<self const*>(p)->reference_counter_);
		}

		friend void intrusive_ptr_release(const Derived* p) 
		{
			if ( ! --(static_cast<self const*>(p)->reference_counter_) )
			{
				DeleteStrategy deleter;
				deleter( const_cast<Derived*>(p) );
			}
		}

		mutable CounterType reference_counter_;
	};
}

#endif  // #ifndef MEMORY_MGR_INTRUSIVE_BASE_HPP_INCLUDED
