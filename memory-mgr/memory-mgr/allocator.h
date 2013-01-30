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
#include <memory-mgr/allocator_decorator.h>
#include <memory-mgr/detail/mgr_impl_allocator.h>
#include <memory-mgr/detail/member_allocator_impl.h>
#include <memory-mgr/detail/polymorphic_allocator.h>

#include <memory-mgr/detail/singleton_allocator_impl.h>
#include <memory-mgr/smart_ptr/make_shared.hpp>

namespace memory_mgr
{
	
#define MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( allocator_type )				\
	template<class Other>																\
	struct rebind																		\
	{	/*convert an allocator<T> to an allocator <Other>*/								\
		typedef allocator_type< Other, mgr_type, pointers_types > other;				\
	};																					\
																						\
	template<class other>																\
	inline allocator_type( const allocator_type<other, mgr_type,						\
										pointers_types>& rhs )							\
		:base_type( rhs )																\
	{	/*construct from a related allocator*/											\
	}																					\
																						\
	template<class other>																\
	inline allocator_type& operator=( const allocator_type<other, mgr_type,				\
										pointers_types>& rhs )							\
	{	/*assign from a related allocator*/												\
		base_type::operator =( rhs );													\
		return (*this);																	\
	}
						
#define MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( allocator_type )					\
		/*allocator TEMPLATE OPERATORS*/										\
		template<class T, class U, class mem_mgr, class Ptrs >					\
		inline bool operator==(const allocator_type<T, mem_mgr, Ptrs>& lhs,		\
					const allocator_type<U, mem_mgr, Ptrs>& rhs) /*throw()*/	\
		{	/*test for allocator equality (always true)*/						\
			return lhs.equal( rhs );											\
		}																		\
																				\
		template<class T, class U, class mem_mgr, class Ptrs >					\
		inline bool operator!=(const allocator_type<T, mem_mgr, Ptrs>& lhs,		\
			const allocator_type<U, mem_mgr, Ptrs>& rhs) /*throw()*/			\
		{	/*test for allocator inequality (always false)*/					\
			return std::rel_ops::operator !=( lhs, rhs );						\
		}


	template< class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class allocator
		:public detail::mgr_impl_allocator<T, MemMgr, detail::singleton_allocator_impl< MemMgr >, RebindPointersFrom >
	{
	public:
		typedef detail::mgr_impl_allocator<T, MemMgr, detail::singleton_allocator_impl< MemMgr >, RebindPointersFrom > base_type;
		
		typedef allocator	self_type;

		allocator()
		{

		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( allocator );

	template<class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class polymorphic_allocator
		:public allocator_decorator<T, RebindPointersFrom>
	{
	public:
		typedef typename MemMgr		mgr_type;
		typedef allocator_decorator<T, RebindPointersFrom> base_type;
		typedef detail::singleton_allocator_impl<MemMgr, detail::polymorphic_allocator > impl_type;

		typedef polymorphic_allocator	self_type;

		// construct default allocator (do nothing)
		inline polymorphic_allocator()
			: base_type( make_shared<impl_type, mgr_type>() )
		{
		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( polymorphic_allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( polymorphic_allocator );

	template< class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class member_allocator
		:public detail::mgr_impl_allocator<T, MemMgr, detail::member_allocator_impl< MemMgr >, RebindPointersFrom >
	{
	public:
		typedef detail::mgr_impl_allocator<T, MemMgr, detail::member_allocator_impl< MemMgr >, RebindPointersFrom > base_type;

		typedef typename base_type::mgr_type		mgr_type;
		// construct allocator from pointer to manager
		inline member_allocator( mgr_type* mgr )
			:base_type( mgr )
		{
		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( member_allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( member_allocator );

	template< class T, class MemMgr, class RebindPointersFrom = detail::offset_pointers<T> >
	class polymorphic_member_allocator
		:public allocator_decorator<T, RebindPointersFrom>
	{
	public:
		typedef MemMgr mgr_type;
		typedef allocator_decorator<T, RebindPointersFrom> base_type;
		typedef detail::member_allocator_impl< MemMgr, detail::polymorphic_allocator > impl_type;

		// construct allocator from pointer to manager
		inline polymorphic_member_allocator( mgr_type* mgr )
			:base_type( mgr ? make_shared<impl_type>( *mgr, mgr ) : base_type::pimpl_type() )
		{
		}

		MGR_DECLARE_ALLOCATOR_REBIND_AND_ASSIGN_OPERATORS( polymorphic_member_allocator )
	};
	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( polymorphic_member_allocator );
}

#endif //MGR_ALLOCATOR_HEADER
