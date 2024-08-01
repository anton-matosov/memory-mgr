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

#ifndef GSTL_POINTER_TRAITS_HEADER
#define GSTL_POINTER_TRAITS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace gstl
{
	template<class T>
	struct pointer_traits
	{
		typedef T value_type;

		typedef value_type*			pointer;
		typedef const value_type* 	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		template<class Other>
		struct rebind
		{	// convert an pointer_traits<T> to an pointer_traits <Other>
			typedef typename pointer_traits<Other> other;
		};
	};

	template<class T>
	struct pointer_traits<const T>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits<volatile T>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits<T*>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits<const T*>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits< T* const>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits<volatile T*>
		:public pointer_traits<T>
	{
	};

	template<class T>
	struct pointer_traits<T* volatile>
		:public pointer_traits<T>
	{
	};

}

#endif GSTL_POINTER_TRAITS_HEADER