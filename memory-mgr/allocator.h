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
#include <memory-mgr/detail/allocator_base.h>
#include <memory-mgr/detail/member_allocator_impl.h>

#include <memory-mgr/detail/singleton_allocator_impl.h>

namespace memory_mgr
{

	template< class T,	class MemMgr >
	class allocator
		:public detail::allocator_base<T, MemMgr, detail::singleton_allocator_impl< MemMgr > >
	{
	public:
		typedef detail::allocator_base<T, MemMgr, detail::singleton_allocator_impl< MemMgr > > base_type;

		typedef typename base_type::mgr_type		mgr_type;
		typedef typename base_type::value_type		value_type;
		typedef allocator< value_type, mgr_type >	self_type;

		typedef typename base_type::pointer				pointer;
		typedef typename base_type::const_pointer		const_pointer;
		typedef typename base_type::reference			reference;
		typedef typename base_type::const_reference		const_reference;
		typedef typename base_type::size_type			size_type;
		typedef typename base_type::difference_type		difference_type;

		// construct default allocator (do nothing)
		inline allocator()
		{
		}

		template<class Other>
		struct rebind
		{	// convert an allocator<T> to an allocator <Other>
			typedef allocator< Other, mgr_type > other;
		};

		template<class other>
		inline allocator( const allocator<other, mgr_type>& rhs ) /*throw()*/
			:base_type( rhs )
		{	// construct from a related allocator
		}

		template<class other>
		inline self_type& operator=( const allocator<other, mgr_type>& rhs )
		{	// assign from a related allocator
			base_type::operator =( rhs );
			return (*this);
		}
	};

	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( allocator );

	template< class T,	class MemMgr >
	class member_allocator
		:public detail::allocator_base<T, MemMgr, detail::member_allocator_impl< MemMgr > >
	{
	public:
		typedef detail::allocator_base<T, MemMgr, detail::member_allocator_impl< MemMgr > > base_type;

		typedef typename base_type::mgr_type		mgr_type;
		typedef typename base_type::value_type		value_type;
		typedef member_allocator< value_type, mgr_type >	self_type;

		typedef typename base_type::pointer				pointer;
		typedef typename base_type::const_pointer		const_pointer;
		typedef typename base_type::reference			reference;
		typedef typename base_type::const_reference		const_reference;
		typedef typename base_type::size_type			size_type;
		typedef typename base_type::difference_type		difference_type;

		// construct allocator from pointer to manager
		inline member_allocator( mgr_type* mgr = 0 )
			:base_type( mgr )
		{
		}

		template<class Other>
		struct rebind
		{	// convert an allocator<T> to an allocator <Other>
			typedef member_allocator< Other, mgr_type > other;
		};

		template<class other>
		inline member_allocator( const member_allocator<other, mgr_type>& rhs ) /*throw()*/
			:base_type( rhs )
		{	// construct from a related allocator
		}

		template<class other>
		inline self_type& operator=( const member_allocator<other, mgr_type>& rhs )
		{	// assign from a related allocator
			base_type::operator =( rhs );
			return (*this);
		}
	};

	MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( member_allocator );
}

#endif //MGR_ALLOCATOR_HEADER
