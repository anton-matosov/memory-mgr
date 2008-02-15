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

#ifndef MGR_SHARED_SEGMENT_HEADER
#define MGR_SHARED_SEGMENT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/config.h"
#include "memory_segment.h"
#include "manager_traits.h"
#include "segment_traits.h"

namespace memory_mgr
{	
	//Adapter for std::vector to SegmentAllocatorConcept 
	struct shared_allocator
	{
		//Default constructor, allocates mem_size bytes
		shared_allocator( const size_t mem_size )
			//:std::vector<ubyte>( mem_size )
		{}

		//Returns addres of allocated segment
		void* segment_base()
		{ return 0; }

		typedef shared_memory_tag	memory_type;
	};

	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr >	
	class shared_segment 
		: public memory_segment< shared_allocator, MemMgr >
	{};

	template< class MemMgr >
	struct manager_traits< shared_segment< MemMgr > > 
		: public manager_traits< MemMgr >
	{
	};
}

#endif// MGR_SHARED_SEGMENT_HEADER
