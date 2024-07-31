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

#include "memory-mgr/detail/types.h"
#include "memory-mgr/detail/ptr_helpers.h"
#include "memory-mgr/memory_segment.h"
#include "memory-mgr/manager_category.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/segment_traits.h"
#include "memory-mgr/detail/malloc_allocator.h"

namespace memory_mgr
{	
	/**
	   @brief Heap segment for memory managers
	   @tparam MemMgr - must support MemoryManagerConcept
	*/
	template< class MemMgr >	
	class heap_segment 
		: public memory_segment< detail::malloc_allocator, MemMgr >
	{
		typedef memory_segment< detail::malloc_allocator, MemMgr > base_type;
	public:
		heap_segment( void* segment_base )
			:base_type( segment_base )
		{}

		heap_segment( const size_t id = 0 )
			:base_type( id )
		{}
	};

	/**
	   @brief memory_manager + heap_segment traits
	*/
	template< class MemMgr >
	struct manager_traits< heap_segment< MemMgr > > 
		: public manager_traits< memory_segment< detail::malloc_allocator, MemMgr > >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};
}
