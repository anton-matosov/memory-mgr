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

#ifndef GSTL_ITERATOR_DECLARER_HEADER
#define GSTL_ITERATOR_DECLARER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <boost/iterator/iterator_adaptor.hpp>

namespace gstl
{
	namespace detail
	{
		template
		<
			class ContainerT,
			template <class,class>	class IteratorT
		>
		class iterator_declarer
		{
		public:
			typedef ContainerT									container_type;
			typedef typename container_type::value_type			value_type;
			typedef typename container_type::pointer			pointer;
			typedef typename container_type::const_pointer		const_pointer;

			typedef IteratorT<pointer, container_type>			iterator;
			typedef IteratorT<const_pointer, container_type>	const_iterator;

			static inline iterator build_iter( value_type* p )
			{
				iterator it( p );
				return it;
			}

			static inline const_iterator build_iter( const value_type* p )
			{
				const_iterator it( p );
				return it;
			}
		};
	}
}

#endif //GSTL_ITERATOR_DECLARER_HEADER
