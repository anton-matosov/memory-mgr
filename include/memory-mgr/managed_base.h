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

#include "memory-mgr/manager_category.h"
#include "memory-mgr/new.h"
#include "memory-mgr/detail/static_assert.h"
#include "memory-mgr/detail/ptr_helpers.h"

namespace memory_mgr
{
	namespace detail
	{
		/**
		   @brief Call this method to get full size of memory block 
					that was used to store array
		  
		   @param	p	pointer to memory block 
					that was used to store array
		   @param	size	size passed as parameter to delete[] function
		   @exception newer throws
		  
		   @return size of memory block 
		*/
		static inline size_t calc_full_size( void* p, size_t size )
		{
#ifdef _MSC_VER
			const size_t size_of_items_count = sizeof( size_t );
			size_t size_of_single_item = size;
			size_t items_count = *size_cast(p);
			size_t size_of_all_array_items = items_count * size_of_single_item;

			return size_of_all_array_items + size_of_items_count;
#else
			return size;
#endif
		}
	}

	

	/**
	   @brief 
	   @details 
	   @tparam MemMgr Memory manager class, must support SingletonManagerConcept
	*/
	template< class MemMgr >
	class managed_base
	{
	protected:
		~managed_base()
		{
			STATIC_ASSERT( (is_category_supported< mem_mgr, singleton_manager_tag>::value),
				Memory_manager_does_not_implement_required_concepts );
		}

		typedef MemMgr mem_mgr;
	public:
		
		static inline void* operator new( size_t size )/*throw( std::bad_alloc )*/
		{
			return mem_mgr::instance().allocate( size );			
		}

		static inline void* operator new( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		/**
		   @brief Overloaded placement operator new
		*/
		static inline void* operator new( size_t, void* p ) /*throw()*/
		{
			return p;
		}
		
		static void operator delete(void*, size_t, void*)
		{

		}

		static inline void operator delete( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, size );
		}


		static inline void* operator new[]( size_t size )/*throw( std::bad_alloc )*/
		{			
			return mem_mgr::instance().allocate( size );			
		}

		static inline void* operator new[]( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		/**
		   @brief Overloaded placement operator new[]
		*/
		static inline void* operator new[](  size_t, void* p ) /*throw()*/
		{
			return p;
		}

		static inline void operator delete[]( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, detail::calc_full_size( p, size) );
		}

	};
	
	
}
