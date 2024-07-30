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

#include "memory-mgr/detail/polymorphic_allocator.h"
#include "memory-mgr/detail/allocator_base.h"
#include "memory-mgr/smart_ptr/shared_ptr.hpp"

namespace memory_mgr
{
	template<class T, class RebindPointersFrom =  detail::offset_pointers<T> >
	class allocator_decorator
		:public detail::allocator_base<T, RebindPointersFrom>
	{
	public:
		typedef allocator_decorator self_type;
		typedef detail::polymorphic_allocator impl_type;
		typedef shared_ptr<impl_type> pimpl_type;
		typedef allocator_base<T, RebindPointersFrom> base_type;

		template<class Other>
		struct rebind
		{	/*convert an allocator<T> to an allocator <Other>*/
			typedef allocator_decorator< Other, RebindPointersFrom > other;
		};

		// construct default allocator (do nothing)
		inline allocator_decorator()
		{
		}

		// construct allocator from pointer to manager
		inline allocator_decorator( pimpl_type pimpl )
			:m_pimpl( pimpl )
		{
		}

		template<class other, class RebindPointersFrom>
		inline allocator_decorator( const allocator_decorator<other, RebindPointersFrom>& rhs ) /*throw()*/
			:m_pimpl( rhs.get_pimpl() )
		{	// construct from a related allocator
		}

		template<class other, class RebindPointersFrom>
		inline self_type& operator=( const allocator_decorator<other, RebindPointersFrom>& rhs )
		{	// assign from a related allocator
			m_pimpl = rhs.m_impl;
			return (*this);
		}

		// deallocate object at ptr, ignore size
		inline void deallocate( pointer ptr, size_type count )
		{
			m_pimpl->deallocate( &*ptr, count * sizeof(value_type) );
		}

		// allocate array of count elements
		inline raw_pointer allocate(size_type count)
		{	
			return static_cast<raw_pointer>( m_pimpl->allocate( count * sizeof(value_type) ) );
		}

		// allocate array of count elements, ignore hint
		inline raw_pointer allocate(size_type count, const void *)
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

		template<class other, class RebindPointersFrom>
		bool equal( const allocator_decorator<other, RebindPointersFrom>& rhs ) const /*throw()*/
		{
			return m_pimpl->equal( *rhs.get_pimpl() );
		}
		
		pimpl_type get_pimpl() const
		{
			return m_pimpl;
		}
	private:
		pimpl_type m_pimpl;
		
	};

	template<class T, class U, class RebindPointersFrom>
	inline bool operator==(const allocator_decorator<T, RebindPointersFrom>& lhs,
		const allocator_decorator<U, RebindPointersFrom>& rhs) /*throw()*/
	{
		return lhs.equal( rhs );
	}

	template<class T, class U, class RebindPointersFrom>
	inline bool operator!=(const allocator_decorator<T, RebindPointersFrom>& lhs,
		const allocator_decorator<U, RebindPointersFrom>& rhs) /*throw()*/
	{
		return std::rel_ops::operator !=( lhs, rhs );
	}
}
