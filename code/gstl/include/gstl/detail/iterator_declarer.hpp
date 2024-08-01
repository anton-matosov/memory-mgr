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

namespace memory_mgr
{
	template<class T1, class T2>
	class offset_pointer;
}

namespace gstl
{
	namespace detail
	{
		template<class PtrT>
		struct is_pointer_to_any_location
		{
			enum { value = 1 };
		};

		template<class T, class U>
		struct is_pointer_to_any_location< memory_mgr::offset_pointer<T, U> >
		{
			enum { value = 0 };
		};

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
			typedef IteratorT<pointer, container_type>			iterator;
			typedef IteratorT<const_pointer, container_type>	const_iterator;

			enum
			{
				can_store_conainer = is_pointer_to_any_location<pointer>::value
			};

			static inline iterator build_iter( pointer p, const container_type* cont )
			{
				iterator it( p, cont, can_store_conainer );
				return it;
			}

			static inline const_iterator build_const_iter( const_pointer p, const container_type* cont )
			{
				const_iterator it( p, cont, can_store_conainer );
				return it;
			}
		};
	}
}

#endif //GSTL_ITERATOR_DECLARER_HEADER
