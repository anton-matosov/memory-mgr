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

#include <stdexcept>
#include "config/config.h"
#include "detail/helpers.h"
#include "memory_segment.h"
#include "manager_traits.h"
#include "segment_traits.h"

namespace memory_mgr
{	
	namespace detail
	{
		class shared_allocator_base
		{
		public:
			shared_allocator_base()
				:m_mapping(0),
				m_base(0)
			{

			}
			//Returns address of allocated segment
			void* segment_base()
			{ return m_base; }

			typedef shared_memory_tag	memory_type;

		protected:
			osapi::mapping_handle_t m_mapping;
			void*  m_base;
		};
	}

	

#ifdef MGR_WINDOWS_PLATFORM
	//Windows shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	class shared_allocator: public detail::shared_allocator_base
	{
	public:
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )
		{
			std::string name = "Global\\";
			name += SegNameOp::get_name();
			m_mapping = osapi::create_file_mapping( name, 0,
				PAGE_READWRITE, mem_size );
			if( !m_mapping )
			{
				throw std::runtime_error( "file mapping creation failed" );
			}

			m_base = osapi::map_view_of_file_ex( m_mapping, FILE_MAP_ALL_ACCESS,
				mem_size);
			if( !m_base )
			{
				throw std::runtime_error( "memory mapping failed" );
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
	};

#elif defined( MGR_LINUX_PLATFORM )
	
	//Posix shared memory allocator to SegmentAllocatorConcept 
	template<class SegNameOp>
	class shared_allocator: public detail::shared_allocator_base
	{
		typedef detail::shared_allocator_base base_type;
		std::string m_name;
		const size_t m_size;
	public:			
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size )
			:m_size( mem_size ),
			m_name( SegNameOp::get_name() )
		{
			helpers::add_leading_slash( m_name );
			int oflag = 0;
			
			//read-write mode
			oflag |= O_RDWR;

			//Create new or open existent
			oflag |= O_CREAT;

			m_mapping = shm_open(m_name.c_str(), oflag, S_IRWXO | S_IRWXG | S_IRWXU);
			if( base_type::m_mapping == -1 )
			{
				throw std::runtime_error( "file mapping creation failed" );
			}

			if( ftruncate( base_type::m_mapping, m_size ) != 0 )
			{
				throw std::runtime_error( "failed to resize file mapping" );
			}

			m_base = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, base_type::m_mapping, 0 );

			if( m_base == MAP_FAILED )
			{
				throw std::runtime_error( "memory mapping failed" );
			}
		}

		~shared_allocator()
		{		
			if( m_base != MAP_FAILED )
			{
				munmap( m_base, m_size );
			}			 
			
			if( base_type::m_mapping != -1 )
			{
				shm_unlink( m_name.c_str() );
				osapi::close_handle( m_mapping );
			}

		}
	};
#endif

	struct name_returner
	{
		static inline const char* get_name( const size_t/* id */= 0)
		{ return "seg_name"; }
	};

#define MGR_SEGMENT_NAME( var_name ) var_name##_name_returner

#define MGR_DECLARE_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline const char* get_name( const size_t/* id */= 0)\
		{ return "memory_mgr-"segment_name; }\
	};

MGR_DECLARE_SEGMENT_NAME( default, "default_segment" );
	
	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr, class SegNameOp = MGR_SEGMENT_NAME( default ) >	
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
