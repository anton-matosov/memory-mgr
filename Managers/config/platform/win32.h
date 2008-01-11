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

#ifndef MGR_WIN32_HEADER
#define MGR_WIN32_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#define MGR_WINDOWS_PLATFORM
#include <windows.h>

namespace memory_mgr
{
	namespace osapi
	{
		typedef CRITICAL_SECTION critical_section;

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
	}
}

#endif //MGR_WIN32_HEADER