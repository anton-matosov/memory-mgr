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
#include <boost/pointer_to_other.hpp>

namespace gstl
{
	namespace detail
	{
		template
		<
			class ContainerT,
			template <class,class>	class IteratorT,
			class PointerType = typename ContainerT::pointer,
			class ConstPointerType = typename ContainerT::const_pointer
		>
		class iterator_declarer
		{
		public:
			typedef ContainerT				container_type;

			typedef PointerType				pointer;
			typedef ConstPointerType		const_pointer;
			typedef typename boost::pointer_to_other<pointer, const container_type>::type container_const_pointer;

			typedef IteratorT<pointer, container_type>			iterator;
			typedef IteratorT<const_pointer, container_type>	const_iterator;

			static inline iterator build_iter( pointer p )
			{
				iterator it( p );
				return it;
			}

			static inline const_iterator build_const_iter( const_pointer p )
			{
				const_iterator it( p );
				return it;
			}

			static inline iterator build_iter( pointer p, container_const_pointer cont )
			{
				iterator it( p, cont );
				return it;
			}

			static inline const_iterator build_const_iter( const_pointer p, container_const_pointer cont )
			{
				const_iterator it( p, cont );
				return it;
			}
		};
	}
}

#endif //GSTL_ITERATOR_DECLARER_HEADER
