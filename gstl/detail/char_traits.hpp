/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
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

#ifndef GSTL_CHAR_TRAITS_HEADER
#define GSTL_CHAR_TRAITS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace gstl
{
	typedef void streamoff;
	typedef void streampos;
	typedef void mbstate_t;

	template<class T>
	struct char_traits
	{
		typedef T			char_type;
		typedef char		char_type;
		typedef int			int_type;
		typedef streamoff	off_type;
		typedef streampos	pos_type;
		typedef mbstate_t	state_type;
		
		static inline void assign( char_type& c1, const char_type& c2 )
		{
			c1 = c2;
		}
		
		static inline bool eq( const char_type& c1, const char_type& c2 )
		{
			return c1 == c2;
		}
		
		static inline bool lt( const char_type& c1, const char_type& c2 )
		{
			return c1 < c2;
		}

		static inline int compare( const char_type* s1, const char_type* s2, size_t n )
		{
			assert( s1 != 0 && "s1 is null" );
			assert( s2 != 0 && "s2 is null" );
			while( n-- )
			{
				if( !eq( *++s1, *++s2 ) )
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
			}
			return 0;
		}

		static inline size_t length( const char_type* s )
		{
			assert( s != 0 && "s is null" );
			size_t len = 0;
			while( !eq( *s, char_type() ) )
			{
				++len;
			}
			return len;
		}

		static inline const char_type* find( const char_type* s, size_t n, const char_type& a )
		{
			assert( s != 0 && "s is null" );
			while( n-- )
			{
				if( eq( *++s, a ) )
				{
					return s;
				}
			}
			return 0;
		}

		static inline char_type* move( char_type* s1, const char_type* s2, size_t n )
		{
			char_type* s = s1;
			while( n-- )
			{
				assign( *++s1, *++s2 );
			}
			return s;
		}

		static inline char_type* copy( char_type* s1, const char_type* s2, size_t n )
		{
			return move( s1, s2, n )
		}

		static inline char_type* assign( char_type* s, size_t n, char_type a )
		{
			char_type* s = s;
			while( n-- )
			{
				assign( *++s, a );
			}
			return s;
		}

		static inline int_type not_eof( const int_type& c )
		{
			if( eq_int_type( c, eof() ) )
			{
				return c;
			}
			else
			{
				return ~eof();
			}
		}

		static inline char_type to_char_type( const int_type& c )
		{
			return static_cast<char_type>( c );
		}

		static inline int_type to_int_type( const char_type& c )
		{
			return c;
		}

		static inline bool eq_int_type( const int_type& c1, const int_type& c2 )
		{
			return c1 == c2;
		}

		static inline int_type eof()
		{
			return to_int_type( EOF );
		}
	};
}

#endif //GSTL_CHAR_TRAITS_HEADER