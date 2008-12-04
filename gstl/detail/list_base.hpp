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

#ifndef GSTL_LIST_BASE_HEADER
#define GSTL_LIST_BASE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/algorithm>
#include <gstl/detail/list_node.hpp>

namespace gstl
{
	namespace detail
	{
		/**
		@brief 
		@details 
		*/
		template
		<
			class T,
			class Alloc,
			class PtrTraits
		>
		class list_base
		{
		public:

			//////////////////////////////////////////////////////////////////////////
			//Node types
			typedef detail::list_node<T, Alloc, PtrTraits>		node_type;
			typedef typename node_type::node_allocator_type		node_allocator_type;
			typedef typename node_type::node_ptr_traits			node_ptr_traits;
			typedef typename node_type::node_pointer			node_pointer;
			typedef typename node_type::node_const_pointer		node_const_pointer;

			//////////////////////////////////////////////////////////////////////////
			//Standard types
			typedef list_base									self_type;
			typedef typename node_type::ptr_traits				ptr_traits;

			typedef typename node_type::allocator_type			allocator_type;

			typedef typename node_type::value_type				value_type;
			typedef typename node_type::pointer					pointer;
			typedef typename node_type::const_pointer			const_pointer;
			typedef typename node_type::reference				reference;
			typedef typename node_type::const_reference			const_reference;

			typedef typename node_type::size_type				size_type;
			typedef typename node_type::difference_type			difference_type;

			node_pointer	first_;
			node_pointer	last_;

			allocator_type	alloc_;
			node_allocator_type	node_alloc_;
			size_type		size_;

			list_base(const allocator_type& alloc )
				:alloc_( alloc ),
				node_alloc_( alloc ),
				size_( 0 )
			{
// 				first_ = last_ = _create_node();
// 				
// 				last_->left_ = first_;
				//_link_nodes( first_, last_ );
			}

			void _link_nodes( node_pointer left, node_pointer right )
			{
				left->right_ = right;
				right->left_ = left;
			}

			void _insert_node( node_pointer node, node_pointer right )
			{
				//Link right and node
				_link_nodes( node, right );
				
				if( right == first_ )
				{
					first_ = node;
				}
				else
				{
					//Link left and node
					_link_nodes( right->left_, node );
				}
			}

// 			void _insert_node( node_pointer node, node_pointer left, node_pointer right )
// 			{
// 				//Link right and node
// 				_link_nodes( node, right );
// 
// 				//Link left and node
// 				_link_nodes( left, node );
// 			}

			void _remove_node( node_pointer node )
			{
				_link_nodes( node->left_, node->right_ );
			}

			node_pointer _create_node()
			{
				node_pointer node = node_alloc_.allocate( 1 );
				node_alloc_.construct( node, node_type() );
				return node;
			}

			void _create_node_value( node_pointer node, const value_type& value )
			{
				node->value_ptr_ = alloc_.allocate( 1 );
				alloc_.construct( node->value_ptr_, value );
			}

			void swap( self_type& rhs )
			{
				gstl::swap( *this, rhs );
			}
		};
	}
	
}

#endif //GSTL_LIST_BASE_HEADER



