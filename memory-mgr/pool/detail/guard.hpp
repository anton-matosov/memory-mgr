// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

#ifndef MEMORY_MGR_POOL_GUARD_HPP
#define MEMORY_MGR_POOL_GUARD_HPP

// Extremely Light-Weight guard glass

namespace memory_mgr
{
	namespace details
	{
		namespace pool
		{

			template <typename Mutex>
			class guard
			{
			private:
				Mutex & mtx;

				guard(const guard &);
				void operator=(const guard &);

			public:
				explicit guard(Mutex & nmtx)
					:mtx(nmtx)
				{
					mtx.lock();
				}

				~guard()
				{
					mtx.unlock();
				}
			};

		} // namespace pool
	} // namespace details

} // namespace memory_mgr

#endif
