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

#include <memory-mgr/smart_ptr/smart_ptr/detail/atomic_count.hpp>

namespace memory_mgr
{
	template< class MemMgr, class CounterType = detail::atomic_count >
	class memory_usage_tracker
		:public detail::decorator_base<MemMgr>
	{
	public:
		typedef CounterType counter_type;

		static inline size_type chunked_size( size_type size )
		{
			return chunks_required( size ) * chunk_size;
		}

		inline memory_usage_tracker()
		{
		}

		inline explicit memory_usage_tracker( void* segment_base )
			:decorated_mgr( segment_base )
		{
		}

		inline void* allocate( size_type size )
		{
			on_allocate(size);
			return decorated_mgr::allocate( size );
		}

		inline void* allocate( size_type size, const char* name )
		{
			on_allocate(size);
			return decorated_mgr::allocate( size, name );
		}

		inline void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
		{
			on_allocate(size);
			return decorated_mgr::allocate( size, nothrow );
		}

		inline void deallocate( const void* ptr, size_type size )
		{
			on_deallocate(size);
			decorated_mgr::deallocate( ptr, size );
		}

		inline void deallocate( const void* ptr, size_type size, const char* name )
		{
			on_deallocate(size);
			decorated_mgr::deallocate( ptr, size, name );
		}

		inline void deallocate_named( const void* ptr, size_type size )
		{
			on_deallocate(size);
			decorated_mgr::deallocate_named( ptr, size );
		}

		inline void deallocate_named( const void* ptr, size_type size, const char* name )
		{
			on_deallocate(size);
			decorated_mgr::deallocate_named( ptr, size, name );
		}
	private:
		inline void on_allocate( size_type size )
		{
			m_memory_used_ += chunked_size(size);
		}

		inline void on_deallocate( const void* ptr, size_type size )
		{
			if( ptr )
			{
				m_memory_used_ -= chunked_size(size);
			}
		}

		counter_type m_memory_used_;
	};

	template<class MemMgr>
	struct manager_traits< memory_usage_tracker< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		typedef MemMgr base_manager_type;

		typedef manager_traits<base_manager_type> base_traits;
	};
}
