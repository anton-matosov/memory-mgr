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
#include <memory-mgr/smart_ptr/shared_ptr.hpp>
#include <memory-mgr/smart_ptr/make_shared.hpp>
#include <memory-mgr/new.h>


namespace memory_mgr
{
	template<class MemMgr>
	class low_fragmentation_manager
		:public detail::decorator_base<MemMgr>
	{
	public:
		typedef detail::decorator_base<MemMgr> base_type;

		typedef memory_mgr::mgr_pool_allocator<base_type> pool_allocator_type;
		typedef memory_mgr::pool<pool_allocator_type> pool_type;
		typedef memory_mgr::shared_ptr<pool_type> pool_ptr;

		low_fragmentation_manager()
			: m_pools(NULL)
		{
			create_pools_array();
		}

		///One more constructor that delegates parameters to the base class
		inline explicit low_fragmentation_manager( void* segment_base )
			: base_type( segment_base ),
			m_pools(NULL)
		{
			create_pools_array();
		}

		void create_pools_array()
		{
			m_pools = memory_mgr::new_<pool_ptr>( get_decorated_mgr(), 
				"low_fragmentation_manager_pools" )[detail::num_pools_required]();
		}

		~low_fragmentation_manager()
		{
			::delete_array( get_decorated_mgr(), m_pools, 
				"low_fragmentation_manager_pools" );
		}

		using base_type::allocate;
		inline void* allocate( size_type size )
		{
			if( size && can_allocate_in_pool(size) )
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

		using base_type::deallocate;
		inline void deallocate( const void* ptr, size_type size )
		{
			if( can_allocate_in_pool(ptr, size) )
			{
				deallocate_in_pool( ptr, size );
			}
			else
			{
				decorated_mgr::deallocate( ptr, size );
			}
		}

	private:
		bool can_allocate_in_pool( const void* ptr, size_type size )
		{
			return ptr && can_allocate_in_pool(size);
		}

		bool can_allocate_in_pool( size_type size )
		{
			return size && size < detail::max_lfm_object_size;
		}

		void* allocate_in_pool( size_type size )
		{
			lock_type lock( this->get_lockable() );
			return get_pool(size).allocate();
		}

		void deallocate_in_pool( const void* ptr, size_type size )
		{
			lock_type lock( this->get_lockable() );
			get_pool(size).deallocate( detail::unconst_void( ptr ) );
		}


		pool_type& get_pool( size_type size )
		{
			size_t pool_id = detail::get_pool_id(size);
			pool_ptr& pool = m_pools[pool_id];
			if( ! pool )
			{
				size_type segment_allocation_size = 1;
				size_type pool_object_size = detail::get_allocation_size(size, segment_allocation_size);
				pool = memory_mgr::make_shared<pool_type>( get_decorated_mgr(), 
					pool_object_size,
					detail::get_pool_grow_size(segment_allocation_size),
					0,
					pool_allocator_type( get_decorated_mgr() )
					);
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
