/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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

#ifndef MGR_NEW_HEADER
#define MGR_NEW_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{
		template
		<
			class MgrType,
			class PointerConvSupp,
			class SizeTrackingSupp
		>
		struct new_helpers
		{
			static inline void* new_impl( size_t size, MgrType& mgr )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}
		};

		template< class MemMgr >
		struct new_helpers<MemMgr, typename is_category_supported<MemMgr, size_tracking_tag>::yes_type,
			typename is_category_supported<MemMgr, pointer_convertion_tag>::yes_type>
		{
			typedef MemMgr mgr_type;
			static inline void* new_impl( size_t size, mgr_type& mgr )
			{
				return mgr.allocate( size );
			}

			static inline void delete_impl( void* p, mgr_type& mgr )
			{
				return mgr.deallocate( p );
			}
		};
	

		template<class MemMgr> 
		class mem_mgr_helper
		{			
			mem_mgr_helper& operator=( const mem_mgr_helper& );
		public:
			typedef MemMgr mgr_type;

			mutable mgr_type& m_mgr;
			mem_mgr_helper( mgr_type& mgr )
				:m_mgr( mgr )
			{}
		};

		
	}

	template<class MemMgr>
	detail::mem_mgr_helper<MemMgr> mem_mgr( MemMgr& mgr )
	{
		return detail::mem_mgr_helper<MemMgr>( mgr );
	};
}


template<class MemMgr>
void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	return memory_mgr::detail::new_helpers<mgr_type, memory_mgr::is_category_supported<mgr_type, memory_mgr::size_tracking_tag>::result,
		memory_mgr::is_category_supported<mgr_type, memory_mgr::pointer_convertion_tag>::result>::new_impl( size, mgr.m_mgr );
}

#endif //MGR_NEW_HEADER
