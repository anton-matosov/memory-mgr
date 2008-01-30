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

#ifndef MGR_MEMORY_SEGMENT_HEADER
#define MGR_MEMORY_SEGMENT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <vector>
#include "manager_traits.h"
#include "manager_category.h"

namespace memory_mgr
{	
	

	//Links memory_manager and memory segment
	//SegmentAllocator - must support SegmentAllocatorConcept
	//MemMgr - must support MemoryManagerConcept
	template< class SegmentAllocator, class MemMgr >	
	class memory_segment : private SegmentAllocator, public MemMgr
	{
		typedef MemMgr				memmgr_type;
		typedef	SegmentAllocator	memory;

	public:
		memory_segment()
			:memory( manager_traits<memmgr_type>::memory_size ),
			memmgr_type( memory::segment_base() )
		{}

	};

	template< class SegmentAllocator, class MemMgr >
	struct manager_traits< memory_segment< SegmentAllocator, MemMgr > > 
		: public manager_traits< typename manager_traits<MemMgr>::manager_type >
	{
	};

}
#endif// MGR_MEMORY_SEGMENT_HEADER
