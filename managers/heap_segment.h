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

#ifndef MGR_HEAP_SEGMENT_HEADER
#define MGR_HEAP_SEGMENT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <vector>
#include "detail/types.h"
#include "memory_segment.h"
#include "manager_category.h"
#include "manager_traits.h"
#include "segment_traits.h"

namespace memory_mgr
{	
	//Adapter for std::vector to SegmentAllocatorConcept 
	struct vector_as_allocator
	{
		//Default constructor, allocates mem_size bytes
		vector_as_allocator( const size_t mem_size )
			:m_memory( mem_size )
		{}

		//Returns addres of allocated segment
		void* segment_base()
		{ return &m_memory.front(); }

		std::vector<ubyte> m_memory;
		typedef heap_memory_tag	memory_type;
	};

	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr >	
	class heap_segment 
		: public memory_segment< vector_as_allocator, MemMgr >
	{};

	template< class MemMgr >
	struct manager_traits< heap_segment< MemMgr > > 
		: public manager_traits< MemMgr >
	{
	};
}

#endif// MGR_HEAP_SEGMENT_HEADER
