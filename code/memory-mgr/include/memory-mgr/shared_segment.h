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

#include "memory-mgr/config/config.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/memory_segment.h"
#include "memory-mgr/segment_traits.h"

#include "memory-mgr/detail/helpers.h"

#include <sstream>
#include <stdexcept>


namespace memory_mgr
{	
	namespace detail
	{
		class shared_allocator_base
		{
		public:
			typedef std::size_t size_type;

			shared_allocator_base( const size_type mem_size, const std::string& name )
				:m_mapping( osapi::invalid_mapping_handle ),
				m_base( osapi::invalid_mapping_address ),
				m_name( name ),
				m_size( mem_size )
			{}

			
			//Returns address of allocated segment
			void* segment_base()
			{ 
				if( m_base == osapi::invalid_mapping_address && m_size )
				{
					create_segment();
				}
				return m_base; 
			}

			typedef shared_memory_tag	memory_type;

		private:
			void create_segment()
			{
				//Create file mapping
				this->m_mapping = osapi::create_file_mapping( this->m_name, this->get_open_flags(),
					this->get_access_mode(), this->m_size );
				if( this->m_mapping == osapi::invalid_mapping_handle )
				{
					const auto lastError = osapi::get_error_code();
					std::stringstream ss;
					ss << "file mapping creation failed. name: " << this->m_name << ". error: " << lastError;
					throw std::runtime_error( ss.str() );
				}

				//Resize file mapping
				if ( osapi::resize_file_mapping( this->m_mapping, this->m_size ) != 0 )
				{
					const auto lastError = osapi::get_error_code();
					std::stringstream ss;
					ss << "failed to resize file mapping. name: " << this->m_name << ". error: " << lastError;
					throw std::runtime_error( ss.str() );
				}

				//Map file to memory
				this->m_base = osapi::map_view_of_file( this->m_mapping, 
					this->get_file_access_mask(), this->m_size );

				if( this->m_base == osapi::invalid_mapping_address )
				{
					const auto lastError = osapi::get_error_code();
					std::stringstream ss;
					ss << "memory mapping failed. name: " << this->m_name << ". error: " << lastError;
					throw std::runtime_error( ss.str() );
				}
			}
		protected:
			osapi::mapping_handle_t m_mapping;
			void*					m_base;
			std::string				m_name;
			size_type				m_size;

			virtual osapi::mode_t get_access_mode() = 0;
			virtual int get_file_access_mask() = 0;

			virtual int get_open_flags()
			{ return 0;	}

			virtual ~shared_allocator_base()
			{
				if( this->m_base != osapi::invalid_mapping_address )
				{
					osapi::unmap_view_of_file( m_base, m_size );
				}			 

				if( this->m_mapping != osapi::invalid_mapping_handle )
				{
					osapi::close_file_mapping( m_name, m_mapping );
				}
			}

			shared_allocator_base( const shared_allocator_base& );
			shared_allocator_base& operator=( const shared_allocator_base& );
		};
	}


#define MGR_SEGMENT_NAME( var_name ) var_name##_name_returner


#ifdef MGR_WINDOWS_PLATFORM

#define MGR_DECLARE_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline std::string get_name( const size_t id = 0 )\
		{\
			std::stringstream name;\
			name << "Global\\mmgr-" << segment_name << "-" << id;\
			return name.str();\
		}\
	};

#define MGR_DECLARE_LOCAL_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline std::string get_name( const size_t id = 0 )\
		{\
			std::stringstream name;\
			name << "Local\\mmgr-" << segment_name << "-" << id;\
			return name.str();\
		}\
	};

	//Windows shared memory allocator to SegmentAllocatorConcept 
	template<class SegmentParams>
	class shared_allocator: public detail::shared_allocator_base
	{
		typedef SegmentParams					segment_params;
		typedef detail::shared_allocator_base	base_type;		

		osapi::mode_t get_access_mode()
		{
			return PAGE_READWRITE;
		}

		int get_file_access_mask()
		{
			return FILE_MAP_ALL_ACCESS;
		}
	public:
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size, const size_t id = 0 )
			:base_type( mem_size, segment_params::get_name( id ) )
		{}
	};

#elif defined( MGR_LINUX_PLATFORM ) || defined( MGR_APPLE_PLATFORM )

#define MGR_DECLARE_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline std::string get_name( const size_t id = 0 )\
		{\
			std::stringstream name;\
			name << "/mmgr-" << segment_name << "-" << id;\
			return name.str();\
		}\
	};

#define MGR_DECLARE_LOCAL_SEGMENT_NAME( var_name, segment_name )\
	struct MGR_SEGMENT_NAME(var_name)\
	{\
		static inline std::string get_name( const size_t id = 0 )\
		{\
			std::stringstream name;\
			name << "/mmgr-" << getpid() << "-" << segment_name << "-" << id;\
			return name.str();\
		}\
	};

	//Posix shared memory allocator to SegmentAllocatorConcept 
	template<class SegmentParams>
	class shared_allocator: public detail::shared_allocator_base
	{
		typedef SegmentParams					segment_params;
		typedef detail::shared_allocator_base	base_type;		

		int get_open_flags()
		{ 
			return O_RDWR //read-write mode
				| O_CREAT;//Create new or open existent
		}

		osapi::mode_t get_access_mode()
		{
			return S_IRWXO  //read, write, execute/search by others 
				| S_IRWXG	//read, write, execute/search by group
				| S_IRWXU;	//read, write, execute/search by owner
		}

		int get_file_access_mask()
		{
			return PROT_READ | PROT_WRITE;
		}
	public:	
		//Default constructor, allocates mem_size bytes
		//in segment with name returned by SegNameOp function		
		shared_allocator( const size_t mem_size, const size_t id = 0 )
			:base_type( mem_size, segment_params::get_name( id ) )
		{}
	};
#endif


MGR_DECLARE_SEGMENT_NAME( default, "default_segment" );
	
	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr, class SegNameOp = MGR_SEGMENT_NAME( default ) >	
	class shared_segment 
		: public memory_segment< shared_allocator<SegNameOp>, MemMgr >
	{
		typedef memory_segment< shared_allocator<SegNameOp>, MemMgr > base_type;
	public:
		shared_segment( void* segment_base )
			:base_type( segment_base )
		{}

		shared_segment( const size_t id = 0 )
			:base_type( id )
		{}
	};

	template< class MemMgr, class SegNameOp >
	struct manager_traits< shared_segment< MemMgr, SegNameOp > > 
		: public manager_traits< memory_segment< shared_allocator<SegNameOp>, MemMgr > >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};
}
