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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#ifndef GSTL_UTILITY_HEADER
#define GSTL_UTILITY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace gstl {
	// 20.2.1, operators:
	namespace rel_ops
	{
		template<class T>
		bool operator!=( const T& x, const T& y )
		{
			//EqualityComparable(T);
			return !(x == y);
		}

		template<class T>
		bool operator> ( const T& x, const T& y )
		{
			//LessThanComparable(T)
			return y < x;
		}

		template<class T>
		bool operator<=( const T& x, const T& y )
		{
			//LessThanComparable(T)
			return !(y < x);
		}

		template<class T>
		bool operator>=( const T& x, const T& y )
		{
			//LessThanComparable(T)
			return !(x < y);
		}
	}

	// 20.2.2, pairs:
	template <class T1, class T2>
	struct pair
	{
		typedef T1 first_type;
		typedef T2 second_type;
		T1 first;
		T2 second;
		pair()
			:first( T1() ),
			second( T2() )
		{}

		pair( const T1& x, const T2& y )
			:first( x ),
			second( y )
		{}

		template<class U, class V> 
		pair( const pair<U, V> &p )
			:first( p.first ),
			second( p.second )
		{}
	};


	template <class T1, class T2>
	bool operator==( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return x.first == y.first && x.second == y.second;
	}

	template <class T1, class T2>
	bool operator<( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
	}

	template <class T1, class T2>
	bool operator!=( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return rel_ops::operator !=( x, y );
	}

	template <class T1, class T2>
	bool operator> ( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return rel_ops::operator >( x, y );
	}

	template <class T1, class T2>
	bool operator>=( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return rel_ops::operator >=( x, y );
	}

	template <class T1, class T2>
	bool operator<=( const pair<T1, T2>& x, const pair<T1, T2>& y )
	{
		return rel_ops::operator <=( x, y );
	}

	template <class T1, class T2>
	pair<T1, T2> make_pair( const T1& x, const T2& y )
	{
		return pair<T1, T2>( x, y );
	}

}




#endif //GSTL_UTILITY_HEADER

