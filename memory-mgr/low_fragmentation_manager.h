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

#ifndef MGR_LOW_FRAGMENTATION_MANAGER_HEADER
#define MGR_LOW_FRAGMENTATION_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/detail/lfm_pool_id_helpers.h>
#include <memory-mgr/pool/pool.hpp>


namespace memory_mgr
{
	template<class MemMgr>
	class low_fragmentation_manager
		:public detail::decorator_base<MemMgr>
	{
	public:
		typedef detail::decorator_base<MemMgr> base_type;
		typedef memory_mgr::pool<memory_mgr::default_user_allocator_new_delete> pool_type;
		typedef memory_mgr::offset_ptr<pool_type> pool_ptr;

		low_fragmentation_manager()
		{
			m_pools = memory_mgr::new_<pool_type>( get_decorated_mgr(), 
				"low_fragmentation_manager_pools" )()[detail::num_pools_required];
		}

		///One more constructor that delegates parameters to the base class
		inline explicit low_fragmentation_manager( void* segment_base )
			:base_type( segment_base )
		{}

		~low_fragmentation_manager()
		{
			 memory_mgr::delete_array( m_pools, get_decorated_mgr(), 
				"low_fragmentation_manager_pools" );
		}

		inline void* allocate( size_type size )
		{
			if( can_allocate_in_pool(size) )
			{
				return allocate_in_pool(size);
			}
			else
			{
				return decorated_mgr::allocate( size );
			}
		}

		inline void* allocate( size_type size, const std::nothrow_t& nothrow )
		{
			if( can_allocate_in_pool(size) )
			{
				return allocate_in_pool(size);
			}
			else
			{
				return decorated_mgr::allocate( size, nothrow );
			}
		}

		inline void deallocate( const void* ptr, size_type size )
		{
			if( can_allocate_in_pool(size) )
			{
				deallocate_in_pool( ptr, size );
			}
			else
			{
				decorated_mgr::deallocate( ptr, size );
			}
		}

	private:
		bool can_allocate_in_pool( size_type size )
		{
			return size < detail::max_lfm_object_size;
		}

		void* allocate_in_pool( size_type size )
		{
			return get_pool(size).allocate();
		}

		void deallocate_in_pool( void* ptr, size_type size )
		{
			get_pool(size).deallocate( ptr );
		}


		pool_type& get_pool( size_type size )
		{
			size_t pool_id = detail::get_pool_id[size];
			pool_ptr& pool = (*m_pools)[pool_id];
			if( ! pool )
			{
				pool = memory_mgr::new_<pool_type>( get_decorated_mgr() )( 
					detail::get_allocation_size(size) );
			}
			return *pool;
		}

		pool_ptr* m_pools;
	};

	template< class MemMgr >
	struct manager_traits< low_fragmentation_manager< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		typedef MemMgr base_manager_type;
	};
}

#endif //MGR_LOW_FRAGMENTATION_MANAGER_HEADER
