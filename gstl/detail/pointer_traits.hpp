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
#define GSTL_VECTOR_HEADER

#ifdef PRAGMA_ONCE_SUPPORTED
#	pragma once
#endif

#include <gstl/detail/allocator.hpp>

namespace gstl
{
	template<class T> 
	struct pointer_traits
	{
	public:
		typedef T									value_type;
		typedef pointer_traits< value_type >		self_type;

		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;

		template<class Other>
		struct rebind
		{	// convert an pointer_traits<T> to an pointer_traits <Other>
			typedef typename pointer_traits< Other > other;
		};

		static pointer null_ptr;

		static inline bool is_null( const_pointer ptr )
		{
			return ptr == null_ptr;
		}
	};


// 	template<class T> 
// 	struct pointer_traits<T*>
// 		:public pointer_traits<T>
// 	{};


	template<class T>
	typename pointer_traits<T>::pointer pointer_traits<T>::null_ptr = 0;
}

#endif GSTL_POINTER_TRAITS_HEADER