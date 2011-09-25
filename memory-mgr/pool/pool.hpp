// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.memory-mgr.org/LICENSE_1_0.txt)
//
// See http://www.memory-mgr.org for updates, documentation, and revision history.

#ifndef MEMORY_MGR_POOL_HPP
#define MEMORY_MGR_POOL_HPP

#include <boost/config.hpp>  // for workarounds

// std::less, std::less_equal, std::greater
#include <functional>
// new[], delete[], std::nothrow
#include <new>
// std::size_t, std::ptrdiff_t
#include <cstddef>
// std::allocate, std::free
#include <cstdlib>
// std::invalid_argument
#include <exception>
// std::max
#include <algorithm>

#include <memory-mgr/pool/poolfwd.hpp>

// memory_mgr::details::pool::ct_lcm
#include <memory-mgr/pool/detail/ct_gcd_lcm.hpp>
// memory_mgr::details::pool::lcm
#include <memory-mgr/pool/detail/gcd_lcm.hpp>
// memory_mgr::simple_segregated_storage
#include <memory-mgr/pool/simple_segregated_storage.hpp>
#include <memory-mgr/detail/ptr_casts.h>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::allocate; using ::free; }
#endif

// There are a few places in this file where the expression "this->m" is used.
// This expression is used to force instantiation-time name lookup, which I am
//   informed is required for strict Standard compliance.  It's only necessary
//   if "m" is a member of a base class that is dependent on a template
//   parameter.
// Thanks to Jens Maurer for pointing this out!

namespace memory_mgr {

	struct default_user_allocator_new_delete
	{
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		inline char* allocate(const size_type bytes)
		{
			return new (std::nothrow) char[bytes];
		}

		inline void deallocate(char* const block, size_type /*size*/)
		{
			delete [] block;
		}
	};

	struct default_user_allocator_malloc_free
	{
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		inline char* allocate(const size_type bytes)
		{
			return detail::char_cast(std::malloc(bytes));
		}
		
		inline void deallocate(char * const block, size_type /*size*/)
		{
			std::free(block);
		}
	};

	template<class MemMgr>
	struct mgr_pool_allocator
	{
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		mgr_pool_allocator( MemMgr& mgr )
			:m_mgr( &mgr )
		{

		}

		inline char* allocate(const size_type bytes)
		{
			return detail::char_cast( m_mgr->allocate( bytes ) );
		}

		inline void deallocate(char * const block, size_type size)
		{
			m_mgr->deallocate( block, size );
		}

	private:
		MemMgr* m_mgr;
	};

	template<class SingletonMemMgr>
	struct memory_mgr_pool_allocator
	{
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		char* allocate(const size_type bytes)
		{
			return detail::char_cast( SingletonMemMgr::instance().allocate( bytes ) );
		}

		void deallocate(char * const block, size_type size)
		{
			SingletonMemMgr::instance().deallocate( block, size );
		}
	};

	namespace details {

		// PODptr is a class that pretends to be a "pointer" to different class types
		//  that don't really exist.  It provides member functions to access the "data"
		//  of the "object" it points to.  Since these "class" types are of variable
		//  size, and contains some information at the *end* of its memory (for
		//  alignment reasons), PODptr must contain the size of this "class" as well as
		//  the pointer to this "object".
		template <typename SizeType, class Pointer = offset_ptr<char> >
		class PODptr
		{
		public:
			typedef SizeType size_type;

		private:
			typedef typename boost::pointer_to_other<Pointer, char>::type char_ptr;
			char_ptr ptr;
			size_type sz;

			struct POD_ptr_chunk
			{
				char_ptr next_;
			};

			char * ptr_next_size() const
			{
				return get_pointer(ptr + sz - sizeof(size_type));
			}

			char * ptr_next_ptr() const
			{
				return (ptr_next_size() -
					pool::ct_lcm<sizeof(size_type), sizeof(void *)>::value);
			}

		public:
			PODptr(char_ptr const nptr, const size_type nsize)
				:ptr(nptr), sz(nsize)
			{
			}

			PODptr()
				:ptr(0), sz(0)
			{
			}

			bool valid() const
			{
				return (ptr);
			}

			void invalidate()
			{
				ptr = 0;
			}

			char * begin() const
			{
				return get_pointer( ptr );
			}

			char * end() const
			{
				return ptr_next_ptr();
			}

			size_type total_size() const
			{
				return sz;
			}

			size_type element_size() const
			{
				return (sz - sizeof(size_type) -
					pool::ct_lcm<sizeof(size_type), sizeof(void *)>::value);
			}

			size_type & next_size() const
			{
				return *detail::size_cast( ptr_next_size() );
			}

			char_ptr& next_POD_ptr() const
			{ 
				return reinterpret_cast<POD_ptr_chunk*>( ptr_next_ptr() )->next_;
			}

			char* next_ptr() const
			{ 
				return &*next_POD_ptr();
			}

			PODptr next() const
			{
				return PODptr( next_ptr(), next_size() );
			}

			void next(const PODptr & arg) const
			{
				next_POD_ptr() = arg.begin();
				next_size() = arg.total_size();
			}
		};

	} // namespace details

	template <typename UserAllocator>
	class pool
		: protected simple_segregated_storage<
		typename UserAllocator::size_type>
	{
	public:
		typedef UserAllocator user_allocator;
		typedef typename UserAllocator::size_type size_type;
		typedef typename UserAllocator::difference_type difference_type;

	private:
		BOOST_STATIC_CONSTANT(unsigned, min_alloc_size =
			(::memory_mgr::details::pool::ct_lcm<sizeof(void *), sizeof(size_type)>::value) );

		// Returns 0 if out-of-memory
		// Called if allocate/ordered_allocate needs to resize the free list
		void * malloc_need_resize();
		void * ordered_allocate_need_resize();

	protected:
		details::PODptr<size_type> list;

		const size_type requested_size;
		size_type m_next_size;
		size_type start_size;
		size_type max_size;
		user_allocator m_alloc;

		simple_segregated_storage<size_type> & store()
		{
			return *this;
		}

		const simple_segregated_storage<size_type> & store() const
		{
			return *this;
		}

		// finds which POD in the list 'chunk' was allocated from
		details::PODptr<size_type> find_POD(void * const chunk) const;

		// is_from() tests a chunk to determine if it belongs in a block
		static bool is_from(void_ptr const chunk, char * const i,
			const size_type sizeof_i)
		{
			// We use std::less_equal and std::less to test 'chunk'
			//  against the array bounds because standard operators
			//  may return unspecified results.
			// This is to ensure portability.  The operators < <= > >= are only
			//  defined for pointers to objects that are 1) in the same array, or
			//  2) subobjects of the same object [5.9/2].
			// The functor objects guarantee a total order for any pointer [20.3.3/8]
			//WAS:
			//      return (std::less_equal<void *>()(static_cast<void *>(i), chunk)
			//          && std::less<void *>()(chunk,
			//              static_cast<void *>(i + sizeof_i)));
			std::less_equal<void *> lt_eq;
			std::less<void *> lt;
			return (lt_eq(i, get_pointer(chunk) ) && lt(get_pointer(chunk), i + sizeof_i));
		}

		size_type alloc_size() const
		{
			const unsigned min_size = min_alloc_size;
			return details::pool::lcm<size_type>(requested_size, min_size);
		}

		size_type calc_POD_size( const size_type partition_size ) 
		{
			return m_next_size * partition_size +
				details::pool::ct_lcm<sizeof(size_type), sizeof(void *)>::value + sizeof(size_type);
		}

	public:
		// The second parameter here is an extension!
		// pre: npartition_size != 0 && nnext_size != 0
		explicit pool(const size_type nrequested_size,
			const size_type nnext_size = 32,
			const size_type nmax_size = 0,
			const user_allocator& alloc = user_allocator() )
			:list(0, 0),
			requested_size(nrequested_size),
			m_next_size(nnext_size),
			start_size(nnext_size),
			max_size(nmax_size),
			m_alloc( alloc )
		{
		}

		~pool()
		{
			purge_memory();
		}

		// Releases memory blocks that don't have chunks allocated
		// pre: lists are ordered
		//  Returns true if memory was actually deallocated
		bool release_memory();

		// Releases *all* memory blocks, even if chunks are still allocated
		//  Returns true if memory was actually deallocated
		bool purge_memory();

		// These functions are extensions!
		size_type get_next_size() const { return m_next_size; }
		void set_next_size(const size_type nnext_size) { m_next_size = start_size = nnext_size; }
		size_type get_max_size() const { return max_size; }
		void set_max_size(const size_type nmax_size) { max_size = nmax_size; }
		size_type get_requested_size() const { return requested_size; }

		// Both allocate and ordered_allocate do a quick inlined check first for any
		//  free chunks.  Only if we need to get another memory block do we call
		//  the non-inlined *_need_resize() functions.
		// Returns 0 if out-of-memory
		void * allocate()
		{
			// Look for a non-empty storage
			if (!store().empty())
			{
				return store().allocate();
			}
			return malloc_need_resize();
		}

		void * ordered_allocate()
		{
			// Look for a non-empty storage
			if (!store().empty())
			{
				return store().allocate();
			}
			return ordered_allocate_need_resize();
		}

		// Returns 0 if out-of-memory
		// Allocate a contiguous section of n chunks
		void * ordered_allocate(size_type n);

		// pre: 'chunk' must have been previously
		//        returned by *this.allocate().
		void deallocate(void * const chunk)
		{
			MGR_POOL_ASSERT( is_from( chunk ), "This chunk was not allocated by this pool" );
			store().deallocate(chunk);
		}

		// pre: 'chunk' must have been previously
		//        returned by *this.allocate().
		void ordered_deallocate(void * const chunk)
		{
			MGR_POOL_ASSERT( is_from( chunk ), "This chunk was not allocated by this pool" );
			store().ordered_deallocate(chunk);
		}

		// pre: 'chunk' must have been previously
		//        returned by *this.allocate(n).
		void deallocate(void * const chunks, const size_type n)
		{
			const size_type partition_size = alloc_size();
			const size_type total_req_size = n * requested_size;
			const size_type num_chunks = total_req_size / partition_size +
				((total_req_size % partition_size) ? true : false);

			store().deallocate_n(chunks, num_chunks, partition_size);
		}

		// pre: 'chunk' must have been previously
		//        returned by *this.allocate(n).
		void ordered_deallocate(void * const chunks, const size_type n)
		{
			const size_type partition_size = alloc_size();
			const size_type total_req_size = n * requested_size;
			const size_type num_chunks = total_req_size / partition_size +
				((total_req_size % partition_size) ? true : false);

			store().ordered_deallocate_n(chunks, num_chunks, partition_size);
		}

		// is_from() tests a chunk to determine if it was allocated from *this
		bool is_from(void * const chunk) const
		{
			return (find_POD(chunk).valid());
		}
	};

	template <typename UserAllocator>
	bool pool<UserAllocator>::release_memory()
	{
		// This is the return value: it will be set to true when we actually call
		//  UserAllocator::free(..)
		bool ret = false;

		// This is a current & previous iterator pair over the memory block list
		details::PODptr<size_type> ptr = list;
		details::PODptr<size_type> prev;

		// This is a current & previous iterator pair over the free memory chunk list
		//  Note that "prev_free" in this case does NOT point to the previous memory
		//  chunk in the free list, but rather the last free memory chunk before the
		//  current block.
		void_ptr free_p = this->first_;
		void_ptr prev_free_p = 0;

		const size_type partition_size = alloc_size();

		// Search through all the all the allocated memory blocks
		while (ptr.valid())
		{
			// At this point:
			//  ptr points to a valid memory block
			//  free_p points to either:
			//    0 if there are no more free chunks
			//    the first free chunk in this or some next memory block
			//  prev_free_p points to either:
			//    the last free chunk in some previous memory block
			//    0 if there is no such free chunk
			//  prev is either:
			//    the PODptr whose next() is ptr
			//    !valid() if there is no such PODptr

			// If there are no more free memory chunks, then every remaining
			//  block is allocated out to its fullest capacity, and we can't
			//  release any more memory
			if ( ! free_p )
			{
				break;
			}

			// We have to check all the chunks.  If they are *all* free (i.e., present
			//  in the free list), then we can free the block.
			bool all_chunks_free = true;

			// Iterate 'i' through all chunks in the memory block
			// if free starts in the memory block, be careful to keep it there
			void_ptr saved_free = free_p;
			for (char * i = ptr.begin(); i != ptr.end(); i += partition_size)
			{
				// If this chunk is not free
				if (void_ptr(i) != free_p)
				{
					// We won't be able to free this block
					all_chunks_free = false;

					// free_p might have travelled outside ptr
					free_p = saved_free;
					// Abort searching the chunks; we won't be able to free this
					//  block because a chunk is not free.
					break;
				}

				// We do not increment prev_free_p because we are in the same block
				free_p = nextof(free_p);
			}

			// post: if the memory block has any chunks, free_p points to one of them
			// otherwise, our assertions above are still valid

			const details::PODptr<size_type> next = ptr.next();

			if (!all_chunks_free)
			{
				if (is_from(free_p, ptr.begin(), ptr.element_size()))
				{
					std::less<void_ptr> lt;
					void_ptr const end = ptr.end();
					do
					{
						prev_free_p = free_p;
						free_p = nextof(free_p);
					} while (free_p && lt(free_p, end));
				}
				// This invariant is now restored:
				//     free_p points to the first free chunk in some next memory block, or
				//       0 if there is no such chunk.
				//     prev_free_p points to the last free chunk in this memory block.

				// We are just about to advance ptr.  Maintain the invariant:
				// prev is the PODptr whose next() is ptr, or !valid()
				// if there is no such PODptr
				prev = ptr;
			}
			else
			{
				// All chunks from this block are free

				// Remove block from list
				if (prev.valid())
				{
					prev.next(next);
				}
				else
				{
					list = next;
				}

				// Remove all entries in the free list from this block
				if (!! prev_free_p )
				{
					nextof(prev_free_p) = free_p;
				}
				else
				{
					this->first_ = free_p;
				}

				// And release memory
				m_alloc.deallocate(ptr.begin(), ptr.total_size());
				ret = true;
			}

			// Increment ptr
			ptr = next;
		}

		m_next_size = start_size;
		return ret;
	}

	template <typename UserAllocator>
	bool pool<UserAllocator>::purge_memory()
	{
		details::PODptr<size_type> iter = list;

		if (!iter.valid())
		{
			return false;
		}

		do
		{
			// hold "next" pointer
			const details::PODptr<size_type> next = iter.next();

			// delete the storage
			m_alloc.deallocate(iter.begin(), iter.total_size());

			// increment iter
			iter = next;
		} while (iter.valid());

		list.invalidate();
		this->first_ = 0;
		m_next_size = start_size;

		return true;
	}

	template <typename UserAllocator>
	void * pool<UserAllocator>::malloc_need_resize()
	{
		// No memory in any of our storages; make a new storage,
		const size_type partition_size = alloc_size();
		const size_type POD_size = calc_POD_size(partition_size);
		char * const ptr = m_alloc.allocate(POD_size);
		if (ptr == 0)
		{
			return 0;
		}
		const details::PODptr<size_type> node(ptr, POD_size);

		BOOST_USING_STD_MIN();
		if(!max_size)
		{
			m_next_size <<= 1;
		}
		else if( m_next_size*partition_size/requested_size < max_size)
		{
			m_next_size = min (m_next_size << 1, max_size*requested_size/ partition_size);
		}

		//  initialize it,
		store().add_block(node.begin(), node.element_size(), partition_size);

		//  insert it into the list,
		node.next(list);
		list = node;

		//  and return a chunk from it.
		return store().allocate();
	}

	template <typename UserAllocator>
	void * pool<UserAllocator>::ordered_allocate_need_resize()
	{
		// No memory in any of our storages; make a new storage,
		const size_type partition_size = alloc_size();
		const size_type POD_size = calc_POD_size(partition_size);
		char * const ptr = m_alloc.allocate(POD_size);
		if (ptr == 0)
		{
			return 0;
		}
		const details::PODptr<size_type> node(ptr, POD_size);

		BOOST_USING_STD_MIN();
		if(!max_size)
		{
			m_next_size <<= 1;
		}
		else if( m_next_size*partition_size/requested_size < max_size)
		{
			m_next_size = min (m_next_size << 1, max_size*requested_size/ partition_size);
		}

		//  initialize it,
		//  (we can use "add_block" here because we know that
		//  the free list is empty, so we don't have to use
		//  the slower ordered version)
		store().add_ordered_block(node.begin(), node.element_size(), partition_size);

		//  insert it into the list,
		//   handle border case
		if (!list.valid() || std::greater<void *>()(list.begin(), node.begin()))
		{
			node.next(list);
			list = node;
		}
		else
		{
			details::PODptr<size_type> prev = list;

			while (true)
			{
				// if we're about to hit the end or
				//  if we've found where "node" goes
				if (prev.next_ptr() == 0
					|| std::greater<void *>()(prev.next_ptr(), node.begin()))
				{
					break;
				}

				prev = prev.next();
			}

			node.next(prev.next());
			prev.next(node);
		}

		//  and return a chunk from it.
		return store().allocate();
	}

	template <typename UserAllocator>
	void * pool<UserAllocator>::ordered_allocate(const size_type n)
	{
		const size_type partition_size = alloc_size();
		const size_type total_req_size = n * requested_size;
		const size_type num_chunks = total_req_size / partition_size +
			((total_req_size % partition_size) ? true : false);

		void * ret = store().allocate_n(num_chunks, partition_size);

		if (ret != 0)
		{
			return ret;
		}

		// Not enougn memory in our storages; make a new storage,
		BOOST_USING_STD_MAX();
		m_next_size = max (m_next_size, num_chunks);
		const size_type POD_size = m_next_size * partition_size +
			details::pool::ct_lcm<sizeof(size_type), sizeof(void *)>::value + sizeof(size_type);
		char * const ptr = m_alloc.allocate(POD_size);
		if (ptr == 0)
		{
			return 0;
		}
		const details::PODptr<size_type> node(ptr, POD_size);

		// Split up block so we can use what wasn't requested
		//  (we can use "add_block" here because we know that
		//  the free list is empty, so we don't have to use
		//  the slower ordered version)
		if (m_next_size > num_chunks)
		{
			store().add_ordered_block(node.begin() + num_chunks * partition_size,
				node.element_size() - num_chunks * partition_size, partition_size);
		}

		BOOST_USING_STD_MIN();
		if(!max_size)
		{
			m_next_size <<= 1;
		}
		else if( m_next_size*partition_size/requested_size < max_size)
		{
			m_next_size = min (m_next_size << 1, max_size*requested_size/ partition_size);
		}

		//  insert it into the list,
		//   handle border case
		if (!list.valid() || std::greater<void *>()(list.begin(), node.begin()))
		{
			node.next(list);
			list = node;
		}
		else
		{
			details::PODptr<size_type> prev = list;

			while (true)
			{
				// if we're about to hit the end or
				//  if we've found where "node" goes
				if (prev.next_ptr() == 0
					|| std::greater<void *>()(prev.next_ptr(), node.begin()))
				{
					break;
				}

				prev = prev.next();
			}

			node.next(prev.next());
			prev.next(node);
		}

		//  and return it.
		return node.begin();
	}

	template <typename UserAllocator>
	details::PODptr<typename pool<UserAllocator>::size_type>
		pool<UserAllocator>::find_POD(void * const chunk) const
	{
		// We have to find which storage this chunk is from.
		details::PODptr<size_type> iter = list;
		while (iter.valid())
		{
			if (is_from(chunk, iter.begin(), iter.element_size()))
			{
				return iter;
			}
			iter = iter.next();
		}

		return iter;
	}

} // namespace memory_mgr

#endif
