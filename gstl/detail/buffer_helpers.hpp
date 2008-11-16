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

#ifndef GSTL_BUFFER_HELPERS_HEADER
#define GSTL_BUFFER_HELPERS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/assert.hpp>

namespace gstl
{
	namespace detail
	{
		/**
		   @brief checks two sequences on overlapping
		   @detail
			------------------------------------------
			|1)	[__1__] [__2__] == 0
			|		or	
			|	[__2__] [__1__] 
			+-----------------------------------------
			|2)	The the first sequence begins earlier or in the same position as the second
			|	and ends later then second starts
			|	[___1___]		== -1
			|		[___2___]
			|		or
			|	[___1___]		
			|	[___2_______]
			|		or
			|	[___1_______]	
			|	[___2___]
			+-----------------------------------------
			|3)		[___1___]	== 1
			|	[___2___]
			|		or
			|		[___1___]	
			|	[___2_______]
			------------------------------------------
		   @retval -1 if the end of the first sequence overlaps with the beginning of the second
		   @retval 0 if sequences don't overlap
		   @retval 1 if the beginning of the first sequence overlaps with the end of the second
		*/
		template<class T>
		static inline int check_overlap( const T* const first1, const T* const last1
			, const T* const first2, const T* const last2 )
		{
			if( first1 <= first2 )
			{//1) or 2)
				if( last1 > first2 )
				{// case 2
					return -1;
				}
			}
			else
			{//possibly 3)
				if( first1 < last2 )
				{//case 3)
					return 1;
				}
			}
			//case 1)
			return 0;
		}

		template<class T>
		static inline int check_overlap( const T* buf1, const T* buf2, size_t n )
		{
			return check_overlap( buf1, buf1 + n, buf2, buf2 + n );
		}

		template<class T>
		static inline T* move( T* dst, const T* src, size_t n )
		{
			GSTL_ASSERT( dst != 0 && "dst is null" );
			GSTL_ASSERT( src != 0 && "src is null" );
			
			int overlap = check_overlap( src, dst, n );
			T* s = dst;
			if( overlap == -1 )
			{//copy backward
				const T* src_end = src + n;
				T* dst_end = dst + n;
				while( n-- )
				{
					*--dst_end = *--src_end;
				}
			}
			else
			{//copy forward
				while( n-- )
				{
					*dst++ = *src++;
				}

			}
			return s;
		}
	}
}







#endif //GSTL_BUFFER_HELPERS_HEADER

