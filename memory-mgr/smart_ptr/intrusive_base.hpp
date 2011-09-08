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

#include <memory-mgr/smart_ptr/smart_ptr/detail/atomic_count.hpp>

namespace memory_mgr
{
	template<class Derived, class DeleteStrategy, class CounterType = ::memory_mgr::detail::atomic_count>
	class intrusive_base
	{
	private:
		typedef intrusive_base self;

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
		void friend intrusive_ptr_add_ref(const Derived* p) 
		{
			++(static_cast<self const*>(p)->reference_counter_);
		}

		void friend intrusive_ptr_release(const Derived* p) 
		{
			if ( ! --(static_cast<self const*>(p)->reference_counter_) )
			{
				DeleteStrategy deleter;
				deleter( const_cast<Derived*>(p) );
			}
		}

	public:
		memory_mgr::intrusive_ptr<Derived> shared_from_this()
		{
			return boost::intrusive_ptr<Derived>( static_cast<Derived*>(this) );
		}

		memory_mgr::intrusive_ptr<Derived const> shared_from_this() const
		{
			return boost::intrusive_ptr<Derived>(this);
		}

		mutable CounterType reference_counter_;
	};
}

#endif  // #ifndef MEMORY_MGR_INTRUSIVE_BASE_HPP_INCLUDED
