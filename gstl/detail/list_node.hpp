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

#ifndef GSTL_LIST_NODE_HEADER
#define GSTL_LIST_NODE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace gstl
{

	namespace detail
	{
		/**
			@brief		list<> node
			@details	List node for list<> template class
						This class is holder the value and links
		*/
		template
			<
				class T,
				class Alloc = allocator<T>,
				class PtrTraits = typename Alloc::pointer_traits_type
			>
		class list_node
		{
		public:
			typedef list_node	self_type;

			typedef typename Alloc::template rebind<T>::other			allocator_type;
			typedef typename Alloc::template rebind<self_type>::other	node_allocator_type;


			typedef typename allocator_type::value_type			value_type;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;

			typedef typename allocator_type::size_type			size_type;
			typedef typename allocator_type::difference_type	difference_type;

			typedef PtrTraits									ptr_traits;

			typedef typename ptr_traits::template rebind<self_type>::other	node_ptr_traits;


			typedef typename node_ptr_traits::pointer					node_pointer;
			typedef typename node_ptr_traits::const_pointer				node_const_pointer;

			typedef typename Alloc::template 
								rebind<node_pointer>::other				node_ptr_allocator_type;

			typedef typename node_ptr_allocator_type::reference			node_ptr_reference;

			//////////////////////////////////////////////////////////////////////////
			// There should be no constructor/destructor
			// because they will never be called
			//////////////////////////////////////////////////////////////////////////

			node_pointer		prev_;
			node_pointer		next_;

			mutable value_type	value_;

		};

	}
}

#endif //GSTL_LIST_NODE_HEADER