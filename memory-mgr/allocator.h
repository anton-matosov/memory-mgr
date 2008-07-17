/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3, 29 June 2007 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA <http://fsf.org/>


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#ifndef MGR_ALLOCATOR_HEADER
#define MGR_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/manager_traits.h>
#include <memory-mgr/memory_manager.h>

namespace memory_mgr
{

	template< class T,	class MemMgr >
	class allocator
	{
	public:
		typedef MemMgr								mgr_type;
		typedef T									value_type;
		typedef allocator< value_type, mgr_type >	self_type;

		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		
		typedef typename manager_traits<mgr_type>::size_type	size_type;
		typedef ptrdiff_t										difference_type;

		template<class Other>
		struct rebind
		{	// convert an allocator<T> to an allocator <Other>
			typedef typename memory_mgr::allocator< Other, mgr_type > other;
		};

		// return address of mutable val
		inline pointer address( reference val ) const
		{	
			return pointer(&val);
		}

		// return address of nonmutable val
		inline const_pointer address( const_reference val ) const
		{	
			return const_pointer(&val);
		}

		// construct default allocator (do nothing)
		inline allocator()
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, memory_manager_tag >::value) &&
				(is_category_supported< mgr_type, size_tracking_tag >::value) &&
				(is_category_supported< mgr_type, pointer_conversion_tag >::value) &&
				(is_category_supported< mgr_type, singleton_manager_tag >::value), Invalid_memory_manager_class );

		}

		template<class other>
		inline allocator( const allocator<other, mgr_type>& ) /*throw()*/
		{	// construct from a related allocator (do nothing)
		}

		template<class other>
		inline self_type& operator=( const allocator<other, mgr_type>& )
		{	// assign from a related allocator (do nothing)
			return (*this);
		}

		// deallocate object at ptr, ignore size
		inline void deallocate( pointer ptr, size_type size )
		{	
			mgr_type::instance().deallocate( &*ptr, size );
		}

		// allocate array of count elements
		inline pointer allocate(size_type count)
		{	
			return static_cast<pointer>( mgr_type::instance().allocate( count * sizeof(value_type) ) );
		}

		// allocate array of count elements, ignore hint
		inline pointer allocate(size_type count, const void *)
		{	
			return (allocate(count));
		}

		// construct object at ptr with value val
		inline void construct(pointer ptr, const_reference val)
		{	
			::new (&*ptr) value_type(val);
		}

		// destroy object at ptr
		inline void destroy(pointer ptr)
		{	
			ptr;//VS 2008 warning
			(&*ptr)->~value_type();
		}

		// estimate maximum array size
		inline size_type max_size() const 
		{	
			size_type count = manager_traits<mgr_type>::memory_size / sizeof( value_type );
			return (0 < count ? count : 1);
		}

	};

	// allocator TEMPLATE OPERATORS
	template<class T,
	class U, class mem_mgr >
	inline bool operator==(const allocator<T, mem_mgr>&, const allocator<U, mem_mgr>&) /*throw()*/
	{	// test for allocator equality (always true)
		return true;
	}

	template<class T,
	class U, class mem_mgr >
	inline bool operator!=(const allocator<T, mem_mgr>&, const allocator<U, mem_mgr>&) /*throw()*/
	{	// test for allocator inequality (always false)
		return false;
	}

}

#endif //MGR_ALLOCATOR_HEADER
