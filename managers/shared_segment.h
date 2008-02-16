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

#include "config/config.h"
#include "memory_segment.h"
#include "manager_traits.h"
#include "segment_traits.h"

namespace memory_mgr
{	
#ifdef MGR_WINDOWS_PLATFORM
	//Windows shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	class shared_allocator
	{
	public:
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )
			:m_mapping( osapi::create_file_mapping( SegNameOp::GetName(), 0,
			PAGE_READWRITE, mem_size ) ),
			m_base( osapi::map_view_of_file_ex( m_mapping, FILE_MAP_ALL_ACCESS,
			mem_size) ) 
		{
			if( !m_mapping || !m_base )
			{
				throw std::runtime_error( "shared segmet creation failed" );
			}
		}
		
		~shared_allocator()
		{
			if( m_base )
			{
				osapi::unmap_view_of_file(m_base);
			}

			if( m_mapping )
			{
				osapi::close_handle(m_mapping);
			}
		}

		//Returns addres of allocated segment
		void* segment_base()
		{ return m_base; }

		typedef shared_memory_tag	memory_type;

	private:
		HANDLE m_mapping;
		void*  m_base;
	};

#elif MGR_LINUX_PLATFORM
	//Posix shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	struct shared_allocator
	{
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )			
		{}

		//Returns addres of allocated segment
		void* segment_base()
		{ return 0; }

		typedef shared_memory_tag	memory_type;
	};
#endif

	struct WinNameReturner
	{
		static inline const wchar_t* GetName( const size_t/* id */= 0)
		{ return L"seg_name"; }
	};
	
	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr, class SegNameOp = WinNameReturner >	
	class shared_segment 
		: public memory_segment< shared_allocator<SegNameOp>, MemMgr >
	{
	};

	template< class MemMgr, class SegNameOp >
	struct manager_traits< shared_segment< MemMgr, SegNameOp > > 
		: public manager_traits< MemMgr >
	{
	};
}

#endif// MGR_SHARED_SEGMENT_HEADER
