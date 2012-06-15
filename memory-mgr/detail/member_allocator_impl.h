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

#ifndef MGR_MEMBER_ALLOCATOR_IMPL_HEADER
#define MGR_MEMBER_ALLOCATOR_IMPL_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/manager_traits.h>
#include <memory-mgr/detail/types.h>

namespace memory_mgr
{
	namespace detail
	{
		template<class MemMgr, class BaseType = detail::empty_type>
		class member_allocator_impl
			:public BaseType
		{
		public:
			typedef MemMgr												mgr_type;
			typedef manager_traits<mgr_type>							traits_type;
			typedef typename traits_type::size_type						mgr_size_type;

			typedef ::memory_mgr::detail::portable_size_t size_type;

			inline member_allocator_impl( mgr_type* mgr )
				:m_mgr( mgr )
			{
 				STATIC_ASSERT( (is_category_supported< mgr_type, memory_manager_tag >::value)
					, invalid_memory_manager_class );
			}

			// allocate array of count elements
			inline void* allocate(size_type size)
			{
				MGR_ASSERT( m_mgr, "manager pointer should not be NULL" );
				return m_mgr->allocate( static_cast<mgr_size_type>(size) );
			}

			inline void deallocate( void* ptr, size_type size )
			{
				MGR_ASSERT( m_mgr, "manager pointer should not be NULL" );
				m_mgr->deallocate( ptr, static_cast<mgr_size_type>(size) );
			}

			bool equal( const BaseType& rhs ) const /*throw()*/
			{
				return m_mgr == static_cast<const member_allocator_impl&>(rhs).m_mgr;
			}
		private:
			mgr_type* m_mgr;
		};

	}
}

#endif //MGR_MEMBER_ALLOCATOR_IMPL_HEADER

