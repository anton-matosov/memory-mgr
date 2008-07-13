/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
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

#ifndef GSTL_ALLOCATOR_HEADER
#define GSTL_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/pointer_traits.hpp>

namespace gstl
{

	template< class T >
	class allocator
	{
	public:
		typedef T									value_type;
		typedef pointer_traits<T>					traits_type;
		typedef allocator< value_type >				self_type;

		typedef typename traits_type::pointer				pointer;
		typedef typename traits_type::const_pointer			const_pointer;
		typedef typename traits_type::reference				reference;
		typedef typename traits_type::const_reference		const_reference;
		
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;

		template<class Other>
		struct rebind
		{	// convert an allocator<T> to an allocator <Other>
			typedef typename allocator< Other > other;
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
		}

		template<class other>
		inline allocator( const allocator<other>& ) /*throw()*/
		{	// construct from a related allocator (do nothing)
		}

		template<class other>
		inline self_type& operator=( const allocator<other>& )
		{	// assign from a related allocator (do nothing)
			return (*this);
		}

		// deallocate object at ptr, ignore size
		inline void deallocate( pointer ptr, size_type /*size*/ )
		{	
			::operator delete( ptr );
		}

		// allocate array of count elements
		inline pointer allocate(size_type count)
		{	
			return static_cast<pointer>( ::operator new( count * sizeof(value_type) ) );
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
			size_type count = ~size_type(0) / sizeof value_type;
			return (0 < count ? count : 1);
		}

	};

	// allocator TEMPLATE OPERATORS
	template<class T,
	class U>
	inline bool operator==(const allocator<T>&, const allocator<U>&) /*throw()*/
	{	// test for allocator equality (always true)
		return true;
	}

	template<class T,
	class U>
	inline bool operator!=(const allocator<T>&, const allocator<U>&) /*throw()*/
	{	// test for allocator inequality (always false)
		return false;
	}
}

#endif GSTL_ALLOCATOR_HEADER
