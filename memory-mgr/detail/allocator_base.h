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

#ifndef MGR_ALLOCATOR_BASE_HEADER
#define MGR_ALLOCATOR_BASE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/manager_traits.h>
#include <memory-mgr/memory_manager.h>

namespace memory_mgr
{
	namespace detail
	{

		template< class T,	class MemMgr, class AllocImpl >
		class allocator_base
		{
			typedef AllocImpl							impl_type;
		public:
			impl_type	m_impl;

			typedef MemMgr								mgr_type;
			typedef T									value_type;
			typedef allocator_base< value_type, mgr_type, impl_type >	self_type;

			typedef value_type*								pointer;
			typedef const value_type*						const_pointer;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;

			typedef typename manager_traits<mgr_type>::size_type	size_type;
			typedef ptrdiff_t										difference_type;


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
			inline pointer address( pointer val ) const
			{	
				return val;
			}

			// return address of nonmutable val
			inline const_pointer address( const_pointer val ) const
			{	
				return val;
			}

			// construct default allocator (do nothing)
			inline allocator_base()
			{
			}

			// construct allocator from pointer to manager
			inline allocator_base( mgr_type* mgr )
				:m_impl( mgr )
			{
			}

			template<class other>
			inline allocator_base( const allocator_base<other, mgr_type, impl_type>& rhs ) /*throw()*/
				:m_impl( rhs.m_impl )
			{	// construct from a related allocator
			}

			template<class other>
			inline self_type& operator=( const allocator_base<other, mgr_type, impl_type>& rhs )
			{	// assign from a related allocator
				m_impl = rhs.m_impl;
				return (*this);
			}

			// deallocate object at ptr, ignore size
			inline void deallocate( pointer ptr, size_type count )
			{
				m_impl.deallocate( &*ptr, count * sizeof(value_type) );
			}

			// allocate array of count elements
			inline pointer allocate(size_type count)
			{	
				return static_cast<pointer>( m_impl.allocate( count * sizeof(value_type) ) );
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
				size_type count = manager_traits<mgr_type>::allocable_memory / sizeof( value_type );
				return (0 < count ? count : 1);
			}

			template<class other>
			bool equal( const allocator_base<other, mgr_type, impl_type>& rhs ) const /*throw()*/
			{
				return m_impl.equal( rhs.m_impl );
			}
		};

	}

}
						
#define MGR_DECLARE_ALLOCATOR_CMP_OPERATORS( allocator_type )			\
		/*allocator TEMPLATE OPERATORS*/								\
		template<class T, class U, class mem_mgr >						\
		inline bool operator==(const allocator_type<T, mem_mgr>& lhs,	\
					const allocator_type<U, mem_mgr>& rhs) /*throw()*/	\
		{	/*test for allocator equality (always true)*/				\
			return lhs.equal( rhs );									\
		}																\
																		\
		template<class T, class U, class mem_mgr >						\
		inline bool operator!=(const allocator_type<T, mem_mgr>& lhs,	\
			const allocator_type<U, mem_mgr>& rhs) /*throw()*/			\
		{	/*test for allocator inequality (always false)*/			\
			return std::rel_ops::operator !=( lhs, rhs );				\
		}

#endif //MGR_ALLOCATOR_BASE_HEADER
