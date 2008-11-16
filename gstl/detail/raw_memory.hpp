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

#ifndef GSTL_RAW_MEMORY_HEADER
#define GSTL_RAW_MEMORY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/utility.hpp>

namespace gstl 
{
	// 20.4.2, raw storage iterator:
	template <class OutputIterator, class T>
	class raw_storage_iterator
		: public iterator<output_iterator_tag,void,void,void,void> {
	public:
		explicit raw_storage_iterator(OutputIterator x);
		raw_storage_iterator<OutputIterator,T>& operator*();
		raw_storage_iterator<OutputIterator,T>& operator=(const T& element);
		raw_storage_iterator<OutputIterator,T>& operator++();
		raw_storage_iterator<OutputIterator,T> operator++(int);
	};

	// 20.4.3, temporary buffers:
	template <class T>
	pair<T*,ptrdiff_t> get_temporary_buffer( ptrdiff_t n )
	{
		return make_pair( static_cast<T*>(
			::operator new( sizeof( T ) * n ) ), n );
	}

	template <class T>
	void return_temporary_buffer( T* p )
	{
		::operator delete( p );
	}

	// 20.4.4, specialized algorithms:
	template <class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy( InputIterator first, InputIterator last,
		ForwardIterator result )
	{
		for (; first != last; ++result, ++first)
		{	new( static_cast<void*>( &*result ) )
				GSTL_ITER_VALUE_TYPE( ForwardIterator )(*first);
		}
		return result;
	}

	template <class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x)
	{
		for (; first != last; ++first)
		{
			new( static_cast<void*>( &*first ) )
				GSTL_ITER_VALUE_TYPE( ForwardIterator )(x);
		}
	}

	template <class ForwardIterator, class Size, class T>
	void uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{
		for (; n--; ++first)
		{
			new( static_cast<void*>( &*first ) )
				GSTL_ITER_VALUE_TYPE( ForwardIterator )(x);
		}
	}
}


#endif //GSTL_RAW_MEMORY_HEADER

