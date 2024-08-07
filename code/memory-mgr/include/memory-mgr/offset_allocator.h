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

#include "memory-mgr/allocator.h"
#include "memory-mgr/offset_pointer.h"

namespace memory_mgr
{
	namespace detail
	{
		template<class T, class MemMgr>
		class old_offset_pointers
		{
		public:
			typedef offset_pointer<T, MemMgr> pointer;
			typedef offset_pointer<const T, MemMgr> const_pointer;
			typedef typename type_manip::add_reference<T>::type reference;
			typedef typename type_manip::add_reference<const T>::type const_reference;

			template<class Other>
			struct rebind
			{
				typedef old_offset_pointers<Other, MemMgr> other;
			};
		};
	}
	
	template< class T, class MemMgr >
	class offset_allocator
		:public allocator<T, MemMgr, detail::old_offset_pointers<T, MemMgr> >
	{
	public:
		typedef allocator<T, MemMgr, detail::old_offset_pointers<T, MemMgr> > base_type;

		typedef offset_allocator	self_type;
		using typename base_type::mgr_type;

		template<class Other>
		struct rebind
		{	// convert an offset_allocator<T> to an offset_allocator <Other>
			typedef typename memory_mgr::offset_allocator< Other, mgr_type > other;
		};
		// construct default offset_allocator (do nothing)
		inline offset_allocator()
		{
			static_assert( (is_category_supported< mgr_type, memory_manager_tag >::value) &&
				//(is_category_supported< mgr_type, size_tracking_tag >::value) &&
				//(is_category_supported< mgr_type, pointer_conversion_tag >::value) &&
				(is_category_supported< mgr_type, singleton_manager_tag >::value), Invalid_memory_manager_class );

		}

		template<class other>
		inline offset_allocator( const offset_allocator<other, mgr_type>& ) /*throw()*/
		{	// construct from a related offset_allocator (do nothing)
		}

		template<class other>
		inline self_type& operator=( const offset_allocator<other, mgr_type>& )
		{	// assign from a related offset_allocator (do nothing)
			return (*this);
		}
	};

	// offset_allocator TEMPLATE OPERATORS
	template<class T,
	class U, class mem_mgr >
	inline bool operator==(const offset_allocator<T, mem_mgr>&, const offset_allocator<U, mem_mgr>&) /*throw()*/
	{	// test for offset_allocator equality (always true)
		return true;
	}

	template<class T,
	class U, class mem_mgr >
	inline bool operator!=(const offset_allocator<T, mem_mgr>&, const offset_allocator<U, mem_mgr>&) /*throw()*/
	{	// test for offset_allocator inequality (always false)
		return false;
	}

}
