/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#ifndef GSTL_WIN32_HEADER
#define GSTL_WIN32_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#define GSTL_WINDOWS_PLATFORM
#include <windows.h>
#include <string>
#include <gstl/detail/types.hpp>
#include <gstl/detail/temp_buffer.hpp>

namespace gstl
{
	template<class T>
	ulong ulong_cast( T val )
	{
		return static_cast<ulong>( val );
	}

	namespace osapi
	{
		static inline std::string get_executable_path()
		{			
			detail::char_buffer path( 512 );
			while( GetModuleFileNameA( 0, path, ulong_cast( path.count() ) ) == path.count() )
			{
				path.reallocate( path.count() * 2 );
			}	

			return path.get();
		}
	}
}

#endif //GSTL_WIN32_HEADER
