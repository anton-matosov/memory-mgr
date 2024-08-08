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
#include "memory-mgr/allocator_decorator.h"
#include "memory-mgr/detail/member_allocator_impl.h"
#include "memory-mgr/detail/polymorphic_allocator_base.h"

#include "memory-mgr/detail/singleton_allocator_impl.h"

// There is a circular dependency between allocator.h and make_shared.hpp, so do not include it here
#include "memory-mgr/smart_ptr/make_shared.hpp"

namespace memory_mgr
{
	template<class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class polymorphic_allocator
		:public allocator_decorator<T, RebindPointersFrom>
	{
	public:
		typedef MemMgr		mgr_type;
		typedef allocator_decorator<T, RebindPointersFrom> base_type;
		typedef detail::singleton_allocator_impl<MemMgr, detail::polymorphic_allocator_base > impl_type;

		typedef polymorphic_allocator	self_type;
		using typename base_type::pointers_types;

		// construct default allocator (do nothing)
		inline polymorphic_allocator()
			: base_type( make_shared<impl_type, mgr_type>() )
		{
		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( polymorphic_allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( polymorphic_allocator );

	template< class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class polymorphic_member_allocator
		:public allocator_decorator<T, RebindPointersFrom>
	{
	public:
		typedef MemMgr mgr_type;
		typedef allocator_decorator<T, RebindPointersFrom> base_type;
		typedef detail::member_allocator_impl< MemMgr, detail::polymorphic_allocator_base > impl_type;
		using typename base_type::pointers_types;

		// construct allocator from pointer to manager
		inline polymorphic_member_allocator( mgr_type* mgr )
			:base_type( mgr ? make_shared<impl_type>( *mgr, mgr ) : base_type::pimpl_type() )
		{
		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( polymorphic_member_allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( polymorphic_member_allocator );
}
