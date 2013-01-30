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

#ifndef MGR_MALLOC_ALLOCATOR_HEADER
#define MGR_MALLOC_ALLOCATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <windows.h>

namespace memory_mgr
{	
	namespace detail
	{
		/**
		   @brief Adapter for VirtualAlloc/VirtualFree to SegmentAllocatorConcept	                                                            
		*/
		struct virtualmem_allocator
		{
			
			/**
			   @brief Default constructor, allocates memory
			   @param mem_size  memory in bytes            
			*/
			virtualmem_allocator( const size_t mem_size, const size_t /*id*/ = 0 )
				:m_memory( NULL ),
				m_size( mem_size )
			{
			}

			/**
			   @brief Destructor, deallocates memory
			*/
			~virtualmem_allocator()
			{
				VirtualFree( m_memory, 0, MEM_RELEASE );
			}

			/**
			   @brief Call this method to get  address of allocated segment
			   @return  address of allocated segment
			                                                               
			*/
			void* segment_base()
			{ 
				if( ! m_memory && m_size )
				{
					m_memory = VirtualAlloc(0, m_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
					*detail::size_cast(m_memory) = 0;
				}
				return m_memory;
			}

			/**
			   @brief stores memory
			*/
			void* m_memory;
			size_t m_size;
			
			/**
			   @brief Memory type tag
			*/
			typedef heap_memory_tag	memory_type;
		};
	}
}

#endif //MGR_MALLOC_ALLOCATOR_HEADER
