/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007 Anton (shikin) Matosov

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

#ifndef MGR_PERF_TIMER_HEADER
#define MGR_PERF_TIMER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "config/config.h"

#ifdef MGR_WINDOWS_PLATFORM
#	include "detail/win_perf_timer.h"
namespace memory_mgr
{	
	typedef detail::win_perf_timer perf_timer;
}
#elif defined( MGR_LINUX_PLATFORM )
#	include "detail/posix_perf_timer.h"
namespace memory_mgr
{
	typedef detail::posix_perf_timer perf_timer;
}
#endif //MGR_WINDOWS_PLATFORM

#endif//MGR_PERF_TIMER_HEADER
