
//  This file is the adaptation for Generic Memory Manager library
//  
// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

#ifndef MEMORY_MGR_SIMPLE_SEGREGATED_STORAGE_HPP
#define MEMORY_MGR_SIMPLE_SEGREGATED_STORAGE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// std::greater
#include <functional>

#include <memory-mgr/get_pointer.h>
#include <memory-mgr/pool/poolfwd.hpp>
#include <boost/pointer_to_other.hpp>

namespace memory_mgr {

	template <typename SizeType, class Pointer>
	class simple_segregated_storage
	{
	public:
		typedef SizeType size_type;
		typedef typename boost::pointer_to_other<Pointer, void>::type void_ptr;

	private:
		simple_segregated_storage(const simple_segregated_storage &);
		void operator=(const simple_segregated_storage &);

		// A plain wrapper on the offset_ptr to make pointer to pointer
		// definition possible
		struct pool_chunk
		{
			void_ptr next_;
		};

		// pre: (n > 0), (start != 0), (nextof(start) != 0)
		// post: (start != 0)

		// The following function attempts to find n contiguous chunks
		//  of size partition_size in the free list, starting at start.
		// If it succeeds, it returns the last chunk in that contiguous
		//  sequence, so that the sequence is known by [start, {retval}]
		// If it fails, it does do either because it's at the end of the
		//  free list or hits a non-contiguous chunk.  In either case,
		//  it will return 0, and set start to the last considered
		//  chunk.  You are at the end of the free list if
		//  nextof(start) == 0.  Otherwise, start points to the last
		//  chunk in the contiguous sequence, and nextof(start) points
		//  to the first chunk in the next contiguous sequence (assuming
		//  an ordered free list)
		void_ptr try_alloc_n(
			void_ptr & start, size_type n, const size_type partition_size)
		{
			void_ptr iter = nextof(start);
			while (--n != 0)
			{
				void_ptr next = nextof(iter);
				if (next != static_pointer_cast<char>(iter) + partition_size )
				{
					// next == 0 (end-of-list) or non-contiguous chunk found
					start = iter;
					return 0;
				}
				iter = next;
			}
			return iter;
		}

	protected:
		void_ptr first_;

		// Traverses the free list referred to by "first",
		//  and returns the iterator previous to where
		//  "ptr" would go if it was in the free list.
		// Returns 0 if "ptr" would go at the beginning
		//  of the free list (i.e., before "first")
		void_ptr find_prev(void_ptr const ptr)
		{
			// Handle border case
			if ( ! first_ || (first_ > ptr))
			{
				return 0;
			}

			void_ptr iter = first_;
			while (true)
			{
				void_ptr& next = nextof(iter);
				// if we're about to hit the end or
				//  if we've found where "ptr" goes
				if( ! next || (next > ptr))
				{
					return iter;
				}

				iter = next;
			}
		};

		// for the sake of code readability :)
		static void_ptr& nextof(void_ptr const ptr)
		{
			return memory_mgr::static_pointer_cast<pool_chunk>(ptr)->next_; 
		}

	public:
		// Post: empty()
		simple_segregated_storage()
			:first_(0)
		{
			STATIC_ASSERT( sizeof(first_) == sizeof(void*), PointerTypeIsBiggerThanVoidPtr )
		}

		// pre: npartition_sz >= sizeof(void *)
		//      npartition_sz = sizeof(void *) * i, for some integer i
		//      nsz >= npartition_sz
		//      block is properly aligned for an array of object of
		//        size npartition_sz and array of void *
		// The requirements above guarantee that any pointer to a chunk
		//  (which is a pointer to an element in an array of npartition_sz)
		//  may be cast to void **.
		static void_ptr segregate(
			const void * const block,
			const size_type nsz,
			const size_type npartition_sz,
			void_ptr const end = 0)
		{
			// Get pointer to last valid chunk, preventing overflow on size calculations
			//  The division followed by the multiplication just makes sure that
			//    old == block + partition_sz * i, for some integer i, even if the
			//    block size (sz) is not a multiple of the partition size.
			const char * old = static_cast<const char *>(block)
				+ ((nsz - npartition_sz) / npartition_sz) * npartition_sz;

			// Set it to point to the end
			nextof(old) = end;

			// Handle border case where sz == partition_sz (i.e., we're handling an array
			//  of 1 element)
			if (old == block)
			{
				return block;
			}

			// Iterate backwards, building a singly-linked list of pointers
			for (const char * iter = old - npartition_sz; iter != block;
				old = iter, iter -= npartition_sz)
			{
				nextof(iter) = old;
			}

			// Point the first pointer, too
			nextof(block) = old;

			return block;
		}

		// Same preconditions as 'segregate'
		// Post: !empty()
		void add_block(const void * const block,
			const size_type nsz, const size_type npartition_sz)
		{
			// Segregate this block and merge its free list into the
			//  free list referred to by "first"
			first_ = segregate(block, nsz, npartition_sz, first_);
		}

		// Same preconditions as 'segregate'
		// Post: !empty()
		void add_ordered_block(const void * const block,
			const size_type nsz, const size_type npartition_sz)
		{
			// This (slower) version of add_block segregates the
			//  block and merges its free list into our free list
			//  in the proper order

			// Find where "block" would go in the free list
			void_ptr const loc = find_prev(block);

			// Place either at beginning or in middle/end
			if ( ! loc )
			{
				add_block(block, nsz, npartition_sz);
			}
			else
			{
				nextof(loc) = segregate(block, nsz, npartition_sz, nextof(loc));
			}
		}

		// default destructor

		bool empty() const { return ( ! first_ ); }

		// pre: !empty()
		void * allocate()
		{
			void_ptr ret = first_;

			// Increment the "first" pointer to point to the next chunk
			first_ = nextof(first_);
			return get_pointer( ret );
		}

		// pre: chunk was previously returned from a allocate() referring to the
		//  same free list
		// post: !empty()
		void deallocate(void * const chunk)
		{
			nextof(chunk) = first_;
			first_ = chunk;
		}

		// pre: chunk was previously returned from a allocate() referring to the
		//  same free list
		// post: !empty()
		void ordered_deallocate(void * const chunk)
		{
			// This (slower) implementation of 'free' places the memory
			//  back in the list in its proper order.

			// Find where "chunk" goes in the free list
			void_ptr const loc = find_prev(chunk);

			// Place either at beginning or in middle/end
			if( ! loc )
			{
				deallocate(chunk);
			}
			else
			{
				nextof(chunk) = nextof(loc);
				nextof(loc) = chunk;
			}
		}

		// Note: if you're allocating/deallocating n a lot, you should
		//  be using an ordered pool.
		void * allocate_n(const size_type n,
			const size_type partition_size)
		{
			if(n == 0)
			{
				return 0;
			}
			void_ptr start = &first_;
			void_ptr iter;
			do
			{
				if( ! nextof(start) )
				{
					return 0;
				}
				iter = try_alloc_n(start, n, partition_size);
			} while ( ! iter );

			void_ptr const ret = nextof(start);
			nextof(start) = nextof(iter);
			return get_pointer( ret );
		}

		// pre: chunks was previously allocated from *this with the same
		//   values for n and partition_size
		// post: !empty()
		// Note: if you're allocating/deallocating n a lot, you should
		//  be using an ordered pool.
		void deallocate_n(void * const chunks, const size_type n,
			const size_type partition_size)
		{
			if(n != 0)
			{
				add_block(chunks, n * partition_size, partition_size);
			}
		}

		// pre: chunks was previously allocated from *this with the same
		//   values for n and partition_size
		// post: !empty()
		void ordered_deallocate_n(void * const chunks, const size_type n,
			const size_type partition_size)
		{
			if(n != 0)
			{
				add_ordered_block(chunks, n * partition_size, partition_size);
			}
		}
	};

} // namespace memory_mgr

#endif
