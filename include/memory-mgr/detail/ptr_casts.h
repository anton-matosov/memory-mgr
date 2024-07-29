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

#ifndef MGR_PTR_CASTS_HEADER
#define MGR_PTR_CASTS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/types.h>

namespace memory_mgr
{
	namespace detail
	{
		static inline char* char_cast( void* p )
		{
			return static_cast< char* >( p );
		}

		static inline const char* char_cast( const void* p )
		{
			return static_cast< const char* >( p );
		}

		static inline char* unconst_char( const char* p )
		{
			return const_cast< char* >( p );
		}

		static inline wchar_t* unconst_wchar( const wchar_t* p )
		{
			return const_cast< wchar_t* >( p );
		}

		static inline void* unconst_void( const void* p )
		{
			return const_cast< void* >( p );
		}

		static inline ptrdiff_t diff( const void* p1, const void* p2 )
		{
			return char_cast(p1) - char_cast(p2);
		}

		template<typename ResultT>
		static inline ResultT diff( const void* p1, const void* p2 )
		{
			return static_cast<ResultT>(diff(p1, p2));
		}

		static inline void* shift( void* p, ulonglong offset )
		{
			return char_cast(p) + offset;
		}

		static inline const void* shift( const void* p, const ulonglong offset )
		{
			return char_cast(p) + offset;
		}

		static inline size_t* size_cast( void* p )
		{
			return static_cast< size_t* >( p );
		}

		static inline const size_t* size_cast( const void* p )
		{
			return static_cast< const size_t* >( p );
		}
	}// namespace detail

}// namespace memory_mgr

#endif //MGR_PTR_CASTS_HEADER
