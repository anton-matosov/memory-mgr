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

#pragma once

#include <memory-mgr/detail/types.h>
#include <memory-mgr/detail/compatibility_types.h>
#include <memory-mgr/offset_ptr.h>

namespace memory_mgr
{
	namespace detail
	{
		template<class T>
		class offset_pointers
		{
		public:
			typedef typename offset_ptr<T> pointer;
			typedef typename offset_ptr<const T> const_pointer;

			typedef typename type_manip::add_reference<T>::type reference;
			typedef typename type_manip::add_reference<const T>::type const_reference;

			template<class Other>
			struct rebind
			{
				typedef offset_pointers<Other> other;
			};
		};

		template<class T, class RebindPointersFrom = detail::offset_pointers<T> >
		class allocator_base
		{
		public:
			typedef T								value_type;
			typedef allocator_base< value_type >	self_type;

			typedef value_type* raw_pointer;
			typedef const value_type* const_raw_pointer;

			typedef RebindPointersFrom pointers_types;
			typedef typename pointers_types::template rebind<value_type>::other ptr_types;

			typedef typename ptr_types::pointer pointer;
			typedef typename ptr_types::const_pointer const_pointer;
			typedef typename ptr_types::reference reference;
			typedef typename ptr_types::const_reference const_reference;

			typedef ::memory_mgr::detail::portable_size_t size_type;
			typedef ::memory_mgr::detail::portable_difference_type difference_type;

			// construct default allocator (do nothing)
			inline allocator_base()
			{
			}

			template<class other>
			inline allocator_base( const allocator_base<other, RebindPointersFrom>& /*rhs*/ ) /*throw()*/
			{	// construct from a related allocator
			}

			template<class other>
			inline self_type& operator=( const allocator_base<other, RebindPointersFrom>& /*rhs*/ )
			{	// assign from a related allocator
				return (*this);
			}

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

			// return address of mutable val
			inline raw_pointer address( pointer val ) const
			{	
				return &*val;
			}

			// return address of nonmutable val
			inline const_raw_pointer address( const_pointer val ) const
			{	
				return &*val;
			}

			// construct object at ptr with value val
			inline void construct( pointer ptr, const_reference val )
			{	
				::new (&*ptr) value_type(val);
			}
// 
// 			// construct object at ptr with value val
// 			template<class T1>
// 			inline void construct(pointer ptr, const T1& a1)
// 			{	
// 				::new (&*ptr) value_type(a1);
// 			}
// 
// 			// construct object at ptr with value val
// 			template<class T1, class T2>
// 			inline void construct(pointer ptr, const T1& a1, const T2& a2)
// 			{	
// 				::new (&*ptr) value_type(a1, a2);
// 			}

			// destroy object at ptr
			inline void destroy(pointer ptr)
			{	
				ptr;//VS 2008 warning
				(*ptr).~value_type();
			}

			// estimate maximum array size
			inline size_type max_size() const 
			{	
				size_type count = size_type(-1) / sizeof (value_type);
				return 0 < count ? count : 1;
			}
		};

	}

}
