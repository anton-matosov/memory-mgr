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

#ifndef MGR_MEMORY_DEBUG_HEADER
#define MGR_MEMORY_DEBUG_HEADER

#include <memory-mgr/detail/decorator_base.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	template< class MemMgr >
	class memory_debug
		:public detail::decorator_base<MemMgr>
	{
	public:
		typedef detail::decorator_base<MemMgr> base_type;

		inline memory_debug()
		{}


		inline explicit memory_debug( void* segment_base )
			:base_type( segment_base )
		{}

		static const size_type m_debug_marks_size = 8;
		static const size_type m_debug_begin_mark = 0xBEF0BEF0;
		static const size_type m_debug_end_mark = 0xAF7EAF7E;
		static const unsigned char m_debug_allocated_memory_fill = 0xCC;
		static const unsigned char m_debug_deallocated_memory_fill = 0xBA;


		inline void* allocate( size_type size )
		{
			pre_allocate( size );
			void* allocated_memory = decorated_mgr::allocate( size );
			return post_allocate( allocated_memory, size );
		}

		inline void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
		{			
			pre_allocate( size );
			void* allocated_memory = decorated_mgr::allocate( size, nothrow );
			return post_allocate( allocated_memory, size );
		}

		void pre_allocate( size_type& size )
		{
			size += m_debug_marks_size;
		}

		inline void* post_allocate( void* allocated_memory, size_type size )
		{
			if( ! allocated_memory )
			{
				return NULL;
			}
			fill_allocated( allocated_memory, size );

			size_type* block_start_ptr = detail::size_cast( allocated_memory );
			*block_start_ptr = m_debug_begin_mark;

			size_type* block_end_ptr = detail::size_cast( detail::shift( allocated_memory, size ) );
			--block_end_ptr;
			*block_end_ptr = m_debug_end_mark;

			allocated_memory = block_start_ptr + 1;

			return allocated_memory;
		}

		void fill_allocated( void* allocated_memory, size_type size )
		{
			memset( allocated_memory, m_debug_allocated_memory_fill, size );
		}

		inline void deallocate( const void* ptr, size_type size )
		{
			ptr = pre_deallocate( ptr, size );
			decorated_mgr::deallocate( ptr, size );
			fill_deallocated( ptr, size );
		}

		const void* pre_deallocate( const void* allocated_memory, size_type& size )
		{
			if( ! allocated_memory )
			{
				return NULL;
			}
			size_t original_size = size;
			size += m_debug_marks_size;

			const size_type* start_ptr = detail::size_cast( allocated_memory ) - 1;
			MGR_ASSERT( m_debug_begin_mark == *start_ptr, "memory block start was overwritten" );

			const size_type* block_end_ptr = detail::size_cast( detail::shift( allocated_memory, original_size ) );
			MGR_ASSERT( m_debug_end_mark == *block_end_ptr, "memory block end was overwritten" );
			(void)block_end_ptr;

			return start_ptr;
		}

		void fill_deallocated( const void* allocated_memory, size_type size )
		{
			if( ! allocated_memory )
			{
				return;
			}
			memset( const_cast<void*>( allocated_memory ), m_debug_deallocated_memory_fill, size );
		}
	};

	template<class MemMgr>
	struct manager_traits< memory_debug< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual memory_debugging_tag
		{};
	};
}




#endif //MGR_MEMORY_DEBUG_HEADER
