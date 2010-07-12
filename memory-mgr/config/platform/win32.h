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

#ifndef MGR_WIN32_HEADER
#define MGR_WIN32_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#define MGR_WINDOWS_PLATFORM
#include <windows.h>
#include <string>
#include <memory-mgr/detail/ptr_casts.h>
#include <memory-mgr/detail/types.h>
#include <memory-mgr/detail/temp_buffer.h>

namespace memory_mgr
{

	namespace osapi
	{
		typedef CRITICAL_SECTION critical_section;

		typedef void*			handle_t;
		typedef handle_t		file_handle_t;
		typedef handle_t		mutex_handle_t;
		typedef handle_t		mapping_handle_t;
		typedef ulong			mode_t;
		static const mapping_handle_t invalid_mapping_handle = 0;
		static void* invalid_mapping_address = 0;

		namespace detail
		{
			template<class T>
			ulong ulong_cast( T val )
			{
				return static_cast<ulong>( val );
			}

			static inline const char* get_name( const std::string& name )
			{
				return ( name.empty() ? 0 : name.c_str() );
			}

			static inline SECURITY_DESCRIPTOR null_sd()
			{
				SECURITY_DESCRIPTOR sd;
				InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
				SetSecurityDescriptorDacl(&sd, true, NULL, false);
				return sd;
			}

			static inline SECURITY_ATTRIBUTES init_sa( SECURITY_DESCRIPTOR& sd )
			{
				SECURITY_ATTRIBUTES sa;
				sa.lpSecurityDescriptor = &sd;
				sa.nLength = sizeof(SECURITY_ATTRIBUTES);
				sa.bInheritHandle = FALSE;

				return sa;
			}

			static inline SECURITY_ATTRIBUTES* null_sa()
			{
				static SECURITY_DESCRIPTOR sd = null_sd();
				static SECURITY_ATTRIBUTES sa = init_sa( sd );
				return &sa;
			}
		}

		static inline void initialize_critical_section( critical_section* cs )
		{
			return InitializeCriticalSection( cs );
		}

		static inline void delete_critical_section( critical_section* cs )
		{
			return DeleteCriticalSection( cs );
		}

		static inline void enter_critical_section( critical_section* cs )
		{
			return EnterCriticalSection( cs );
		}

		static inline void leave_critical_section( critical_section* cs )
		{
			return LeaveCriticalSection( cs );
		}

		static inline int close_handle(file_handle_t handle)
		{
			return CloseHandle(handle);
		}

		static inline file_handle_t create_file_mapping( const std::string& name, ulong access, ulong low_size, ulong high_size = 0, LPSECURITY_ATTRIBUTES file_mapping_attributes = 0, file_handle_t hFile = INVALID_HANDLE_VALUE )
		{
			return CreateFileMappingA( hFile, 
				file_mapping_attributes ? file_mapping_attributes : detail::null_sa(), access, high_size, low_size, name.c_str() );
		}

		static inline file_handle_t create_file_mapping( const std::string& name,
			int /*open_flag*/, mode_t access_mode, size_t size )
		{
			return create_file_mapping( name,
				access_mode, detail::ulong_cast( size ) );
		}

		static inline int resize_file_mapping( osapi::mapping_handle_t /*m_mapping*/, size_t /*size*/ )
		{ return 0;	}

		static inline file_handle_t open_file_mapping( ulong access, std::string& name )
		{
			return OpenFileMappingA( access, false, name.c_str() );
		}

		static inline void* map_view_of_file(mapping_handle_t handle, ulong file_access, std::size_t numbytes, ulong highoffset = 0, ulong lowoffset = 0, void *base_addr = 0 )
		{  
			return MapViewOfFileEx(handle, file_access, highoffset, lowoffset, numbytes, base_addr);
		}

		static inline int unmap_view_of_file(void* address, size_t /*size*/ = 0)
		{
			return UnmapViewOfFile(address);
		}

		static inline int close_file_mapping(const std::string& /*name*/, mapping_handle_t mapping)
		{
			return close_handle( mapping );
		}

		static inline ulong get_last_error()
		{			
			return GetLastError();
		}

		static inline std::string get_executable_path()
		{			
			::memory_mgr::detail::char_buffer path( 512 );
			while( GetModuleFileNameA( 0, path, detail::ulong_cast( path.count() ) ) == path.count() )
			{
				path.reallocate( path.count() * 2 );
			}	

			return path.get();
		}

		static inline mutex_handle_t create_mutex(const std::string& name )
		{  
			return CreateMutexA( detail::null_sa(), false, detail::get_name( name ) );
		}

		static inline mutex_handle_t open_mutex(const std::string& name, ulong access )
		{ 
			return OpenMutexA( access, false, detail::get_name( name ) );
		}

		static inline bool release_mutex(mutex_handle_t handle)
		{ 
			return ReleaseMutex(handle) != 0;  
		}

		enum lock_status{ lock_aquired, lock_abandoned, lock_timeout, lock_failed };

		static inline lock_status wait_for_single_object(handle_t handle, ulong time)
		{ 
			ulong status = WaitForSingleObject(handle, time);
			return	(status == WAIT_OBJECT_0) ? lock_aquired :
				(status == WAIT_FAILED ) ? lock_failed :
				(status == WAIT_ABANDONED ) ? lock_abandoned : lock_timeout; 
		}

		static inline lock_status lock_mutex( mutex_handle_t mutex )
		{
			return wait_for_single_object( mutex, INFINITE );
		}

		static inline bool unlock_mutex( mutex_handle_t mutex )
		{
			return ReleaseMutex( mutex ) != 0;
		}
	}
}

#endif //MGR_WIN32_HEADER
