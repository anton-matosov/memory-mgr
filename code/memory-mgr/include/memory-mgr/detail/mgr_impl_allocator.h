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

#include "memory-mgr/manager_traits.h"
#include "memory-mgr/detail/allocator_base.h"

namespace memory_mgr
{
	namespace detail
	{

		template< class T,	class MemMgr, class AllocImpl, class RebindPointersFrom = detail::offset_pointers<T> >
		class mgr_impl_allocator
			:public allocator_base<T, RebindPointersFrom>
		{
			typedef AllocImpl							impl_type;
			
		public:
			impl_type	m_impl;
	
			typedef allocator_base<T, RebindPointersFrom> base_type;
			typedef MemMgr mgr_type;
			typedef mgr_impl_allocator self_type;

			using typename base_type::value_type;
			using typename base_type::raw_pointer;
			using typename base_type::const_raw_pointer;
			using typename base_type::pointer;
			using typename base_type::const_pointer;
			using typename base_type::reference;
			using typename base_type::const_reference;
			using typename base_type::size_type;

			// construct default allocator (do nothing)
			inline mgr_impl_allocator()
			{
			}

			// construct allocator from pointer to manager
			inline mgr_impl_allocator( mgr_type* mgr )
				:m_impl( mgr )
			{
			}

			template<class other>
			inline mgr_impl_allocator( const mgr_impl_allocator<other, mgr_type, impl_type, RebindPointersFrom>& rhs ) /*throw()*/
				:base_type(rhs),
				m_impl( rhs.m_impl )
			{	// construct from a related allocator
			}

			template<class other>
			inline self_type& operator=( const mgr_impl_allocator<other, mgr_type, impl_type, RebindPointersFrom>& rhs )
			{	// assign from a related allocator
				base_type::operator =( rhs );
				m_impl = rhs.m_impl;
				return (*this);
			}

			// allocate array of count elements
			inline raw_pointer allocate(size_type count)
			{	
				return static_cast<raw_pointer>( m_impl.allocate( count * sizeof(value_type) ) );
			}

			// allocate array of count elements, ignore hint
			inline raw_pointer allocate(size_type count, const void *)
			{	
				return (allocate(count));
			}

			// deallocate object at ptr, ignore size
			inline void deallocate( pointer ptr, size_type count )
			{
				m_impl.deallocate( &*ptr, count * sizeof(value_type) );
			}

			template<class other>
			bool equal( const mgr_impl_allocator<other, mgr_type, impl_type, RebindPointersFrom>& rhs ) const /*throw()*/
			{
				return m_impl.equal( rhs.m_impl );
			}
		};

	}

}
