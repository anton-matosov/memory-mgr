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

//#define DEBUG_MEMORY_POOLS
// #define MGR_ENABLE_ASSERTS
#define MGR_ENABLE_32_64_BIT_PROCESSES_INTEROPERABILITY

#if defined(linux) || defined(__linux) || defined(__linux__)
// linux:
#	include "memory-mgr/config/platform/linux.h"
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	include "memory-mgr/config/platform/win32.h"
#elif defined(__APPLE__)
#	include "memory-mgr/config/platform/apple.h"
#else
#	error "Unsupported platform. PRs are welcome."
#endif

#include "memory-mgr/detail/helpers.h"
#include "memory-mgr/detail/assert.h"

#include <boost/static_assert.hpp>

#define MGR_STR( expr ) #expr
#define MGR_STRINGIZE( expr ) MGR_STR( expr )
#define MGR_MESSAGE(desc) message(__FILE__ "(" MGR_STRINGIZE(__LINE__) "):" MGR_STRINGIZE(desc) )

#define MGR_INLINE inline

#ifdef _MSC_VER
/*
MSVC++ 11.0 _MSC_VER = 1700 (Visual Studio 2011)
MSVC++ 10.0 _MSC_VER = 1600 (Visual Studio 2010)
MSVC++ 9.0  _MSC_VER = 1500 (Visual Studio 2008)
MSVC++ 8.0  _MSC_VER = 1400 (Visual Studio 2005)
MSVC++ 7.1  _MSC_VER = 1310 (Visual Studio 2003.Net)
MSVC++ 7.0  _MSC_VER = 1300 (Visual Studio 2003)
MSVC++ 6.0  _MSC_VER = 1200 (Visual Studio 6)
MSVC++ 5.0  _MSC_VER = 1100 (Visual Studio 5)
*/
#	if (_MSC_VER == 1700)
#		define MGR_VISUAL_STUDIO_VERSION 2011
#	elif (_MSC_VER == 1600)
#		define MGR_VISUAL_STUDIO_VERSION 2010
#	elif (_MSC_VER == 1500)
#		define MGR_VISUAL_STUDIO_VERSION 2008
#		define MGR_NEEDS_TO_CLEANUP_TYPEINFO_LEAKS
#	elif (_MSC_VER == 1400)
#		define MGR_VISUAL_STUDIO_VERSION 2005
#	elif (_MSC_VER == 1310)
#		define MGR_VISUAL_STUDIO_VERSION 2003.1
#	elif (_MSC_VER == 1300)
#		define MGR_VISUAL_STUDIO_VERSION 2003
#	elif (_MSC_VER == 1200)
#		define MGR_VISUAL_STUDIO_VERSION 1998
#	elif (_MSC_VER == 1100)
#		define MGR_VISUAL_STUDIO_VERSION 1996
#	endif

#else
#	define MGR_VISUAL_STUDIO_VERSION 0
#endif

namespace memory_mgr
{
	/**
	   @brief OS specific API wrappers
	*/
	namespace osapi
	{
		/**
		   @brief Call this function to get path to folder from which executable was launched
		   @return path to folder from which executable was launched
		*/
		static inline std::string get_exe_dir()
		{
			return helpers::get_parent_dir( get_executable_path() );
		}
	}
}
