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


#pragma once

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
		template<class RanIter1, class RanIter2>
		static inline int check_overlap( RanIter1 first1, RanIter1 last1,
			RanIter2 first2, RanIter2 last2 )
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

		template<class T1, class T2, class SizeType>
		static inline int check_overlap( const T1* buf1, const T2* buf2, SizeType n )
		{
			return check_overlap( buf1, buf1 + n, buf2, buf2 + n );
		}

		template<class DstItem, class SrcItem, class SizeType>
		static inline DstItem* move( DstItem* dst, const SrcItem* src, SizeType n )
		{
			GSTL_ASSERT( dst != 0 && "dst is null" );
			GSTL_ASSERT( src != 0 && "src is null" );

			typedef DstItem*		dst_pointer_type;
			typedef const SrcItem*	src_pointer_type;
			
			int overlap = check_overlap( src, dst, n );
			dst_pointer_type dst_begin = dst;
			if( overlap == -1 )
			{//copy backward
				src_pointer_type src_end = src + n;
				dst_pointer_type dst_end = dst + n;
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
			return dst_begin;
		}
	}
}








