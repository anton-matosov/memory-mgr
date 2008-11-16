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

#ifndef GSTL_CHAR_TRAITS_HEADER
#define GSTL_CHAR_TRAITS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/assert.hpp>
#include <gstl/detail/buffer_helpers.hpp>
#include <stdio.h>

namespace gstl
{
	//FIXME: remove this declaration when real types will be declared
	typedef std::streamoff	streamoff;
	typedef std::streampos	streampos;
	typedef std::mbstate_t	mbstate_t;

	//implementation 
	template<class T>
	struct char_traits
	{
		typedef T			char_type;
		typedef int			int_type;
		typedef streamoff	off_type;
		typedef streampos	pos_type;
		typedef mbstate_t	state_type;
		
		/**
		   @brief assigns c = d
		*/
		static inline void assign( char_type& c, const char_type& d )
		{
			c = d;
		}
		
		/**
			@brief yields: whether c is to be treated as equal to d.
		*/
		static inline bool eq( const char_type& c, const char_type& d )
		{
			return c == d;
		}
		
		/**
			@brief yields: whether c is to be treated as less than to d
		*/
		static inline bool lt( const char_type& c, const char_type& d )
		{
			return c < d;
		}

		/**
			@brief yields: 0 if for each i in [0,n),
				X::eq(p[i],q[i]) is true; else, a negative
				value if, for some j in [0,n),
				X::lt(p[j],q[j]) is true and for each i in
				[0,j) X::eq(p[i],q[i]) is true; else a positive values
		*/
		static inline int compare( const char_type* s1, const char_type* s2, size_t n )
		{
			GSTL_ASSERT( s1 != 0 && "s1 is null" );
			GSTL_ASSERT( s2 != 0 && "s2 is null" );

			//GSTL_ASSERT( n <= length( s1 ) + 1 && "n exceeds length of the first string (s1)" );
			//GSTL_ASSERT( n <= length( s2 ) + 1 && "n exceeds length of the second string (s2)" );

			while( n-- )
			{
				if( !eq( *s1, *s2 ) )
				{
					if( lt( *s1, *s2 ) )
					{
						return -1;
					}
					else
					{
						return 1;
					}
				}
				++s1;
				++s2;
			}
			return 0;
		}

		/**
			@brief yields: the smallest i such that 
				X::eq(p[i], charT()) is true.
		*/
		static inline size_t length( const char_type* s )
		{
			GSTL_ASSERT( s != 0 && "s is null" );
			size_t len = 0;
			while( !eq( *s, char_type() ) )
			{
				++s;
				++len;
			}
			return len;
		}

		/**
			@brief yields: the smallest q in [p,p+n) such that
				X::eq(*q,c) is true, zero otherwise
		*/
		static inline const char_type* find( const char_type* s, size_t n, const char_type& a )
		{
			GSTL_ASSERT( s != 0 && "s is null" );
			//GSTL_ASSERT( n <= length( s ) + 1 && "n exceeds length of the string" );
			while( n-- )
			{
				if( eq( *s, a ) )
				{
					return s;
				}
				++s;
			}
			return 0;
		}

		/**
			@brief Moves n chars of the src string into dst
			
			@detail For each i in [0,n), performs X::assign(drc[i],dst[i]). 
					Copies correctly even where the ranges [p, p+n) and [s, s+n) overlap. 
			@return dst pointer.
		*/
		static inline char_type* move( char_type* dst, const char_type* src, size_t n )
		{
			return detail::move( dst, src, n );
		}

		/**
			@brief pre: p not in [s,s+n). yields: s. for each i in
				[0,n), performs X::assign(s[i],p[i]).
		*/
		static inline char_type* copy( char_type* s1, const char_type* s2, size_t n )
		{
			return move( s1, s2, n );
		}

		/**
			@brief for each i in [0,n), performs
				X::assign(s[i],c). yields:s.
		*/
		static inline char_type* assign( char_type* s, size_t n, char_type a )
		{
			GSTL_ASSERT( s != 0 && "s is null" );

			char_type* str = s;
			while( n-- )
			{
				assign( *s++, a );
			}
			return str;
		}

		/**
			@brief yields: e if X::eq_int_type(e,X::eof()) is
				false, otherwise a value f such that
				X::eq_int_type(f,X::eof())is false.
		*/
		static inline int_type not_eof( const int_type& c )
		{
			if( eq_int_type( c, eof() ) )
			{
				return ~eof();
			}
			else
			{
				return c;
			}
		}

		/**
			@brief yields: if for some c,
				X::eq_int_type(e,X::to_int_type(c))
				is true, c; else some unspecified value.
		*/
		static inline char_type to_char_type( const int_type& c )
		{
			return static_cast<char_type>( c );
		}

		/**
			@brief yields: some value e, constrained by the definitions
				of to_char_type and eq_int_type
		*/
		static inline int_type to_int_type( const char_type& c )
		{
			return c;
		}

		/**
			@brief yields: for all c and d, X::eq(c,d) is equal to
				X::eq_int_type(X::to_int_type(c),
				X::to_int_type(d)); otherwise, yields true if
				e and f are both copies of X::eof(); otherwise,
				yields false if one of e and f are copies of
				X::eof() and the other is not; otherwise the value
				is unspecified.
		*/
		static inline bool eq_int_type( const int_type& c1, const int_type& c2 )
		{
			return c1 == c2;
		}

		/**
			@brief yields: a value e such that
				X::eq_int_type(e,X::to_int_type(c))
				is false for all values c. 
		*/
		static inline int_type eof()
		{
			return to_int_type( EOF );
		}
	};
}

#endif //GSTL_CHAR_TRAITS_HEADER