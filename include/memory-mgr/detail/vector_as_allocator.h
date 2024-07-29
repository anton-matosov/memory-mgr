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

#ifndef MGR_VECTOR_AS_ALLOCATOR_HEADER
#define MGR_VECTOR_AS_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <vector>

namespace memory_mgr
{	
	namespace detail
	{
		/**
		   @brief Adapter for std::vector to SegmentAllocatorConcept	                                                            
		*/
		struct vector_as_allocator
		{
			
			/**
			   @brief Default constructor, allocates memory
			   @param mem_size  memory in bytes            
			*/
			vector_as_allocator( const size_t mem_size, const size_t /*id*/ = 0 )
				:m_memory( mem_size )
			{}

			/**
			   @brief Call this method to get  address of allocated segment
			   @return  address of allocated segment
			                                                               
			*/
			void* segment_base()
			{ return &m_memory.front(); }

			/**
			   @brief stores memory
			*/
			std::vector<ubyte> m_memory;
			
			/**
			   @brief Memory type tag
			*/
			typedef heap_memory_tag	memory_type;
		};
	}
}

#endif //MGR_VECTOR_AS_ALLOCATOR_HEADER


