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
			class Alloc
		>
		class list_base
		{
		public:

			//////////////////////////////////////////////////////////////////////////
			//Node types
			typedef detail::list_node<T, Alloc>					node_type;
			typedef typename node_type::node_allocator_type		node_allocator_type;
			typedef typename node_type::node_ptr_allocator_type	node_ptr_allocator_type;

			typedef typename node_type::node_pointer			node_pointer;
			typedef typename node_type::node_const_pointer		node_const_pointer;
			typedef typename node_type::node_ptr_reference		node_ptr_reference;

			//////////////////////////////////////////////////////////////////////////
			//Standard types
			typedef list_base									self_type;

			typedef typename node_type::allocator_type			allocator_type;

			typedef typename node_type::value_type				value_type;
			typedef typename node_type::pointer					pointer;
			typedef typename node_type::const_pointer			const_pointer;
			typedef typename node_type::reference				reference;
			typedef typename node_type::const_reference			const_reference;

			typedef typename node_type::size_type				size_type;
			typedef typename node_type::difference_type			difference_type;

			//node_pointer	head_;
			node_pointer	tail_;

			allocator_type	alloc_;
			node_allocator_type	node_alloc_;
			node_ptr_allocator_type	node_ptr_alloc_;
			size_type		size_;

			list_base(const allocator_type& alloc )
				:alloc_( alloc ),
				node_alloc_( alloc ),
				node_ptr_alloc_( alloc ),
				tail_( node_pointer() ),
				size_( 0 )
			{
				//This initialization can't be performed in the
				//initialization list, 'cause it uses member allocators
 				tail_ = _create_node();
			}

			~list_base()
			{
				GSTL_ASSERT( !! tail_ );
				node_ptr_alloc_.destroy( &_next( tail_ ) );
				node_ptr_alloc_.destroy( &_prev( tail_ ) );
				node_alloc_.deallocate( tail_, 1 );
			}

			void _free_node( node_pointer node )
			{
				GSTL_ASSERT( !! node );	

				//Automatically will call destructor for value_
				this->node_alloc_.destroy( node );
				this->node_alloc_.deallocate( node, 1 );
			}

			void _link_nodes( node_pointer left, node_pointer right )
			{
				_next( left ) = right;
				_prev( right ) = left;
			}

			void _remove_node( node_pointer node )
			{
				_link_nodes( node->prev_, node->next_ );
			}

			node_pointer _create_node()
			{
				node_pointer node = node_alloc_.allocate( 1 );
				bool next_linked = false;

				try
				{
					node_ptr_alloc_.construct( &_next( node ), node );
					next_linked = true;
					node_ptr_alloc_.construct( &_prev( node ), node );
				}
				catch(...)
				{
					if( next_linked )
					{
						node_ptr_alloc_.destroy( &_next( node ) );
					}
					node_alloc_.deallocate( node, 1 );
					throw;
				}

				return node;
			}

			/**
			   @brief Call this method to create new node
			  
			   @param	next	pointer that will be set as next_ in new node
			   @param	prev	pointer that will be set as prev_ in new node
			   @param	val	(parameter description)
			   @exception	newer throws by itself, but if exception is 
							thrown by allocator::allocate method or copy constructor of 
							node_pointer or value, this exception will be passed out of function
			  
			   @return pointer to the new node
			*/
			node_pointer _create_node(node_pointer next,
				node_pointer prev, const_reference val )
			{
				node_pointer node = node_alloc_.allocate( 1 );
				bool next_linked = false;
				bool prev_linked = false;

				try
				{
					node_ptr_alloc_.construct( &_next( node ), next );
					next_linked = true;
					
					node_ptr_alloc_.construct( &_prev( node ), prev );
					prev_linked = true;

					alloc_.construct( &_value( node ), val );
				}
				catch(...)
				{
					if( next_linked )
					{
						node_ptr_alloc_.destroy( &_next( node ) );
					}
					if( prev_linked )
					{
						node_ptr_alloc_.destroy( &_prev( node ) );
					}
					node_alloc_.deallocate( node, 1 );
					throw;
				}
				return node;
			}

			static node_ptr_reference _next( node_pointer node )
			{
				return (*node).next_;
			}

			static node_ptr_reference _prev( node_pointer node )
			{
				return (*node).prev_;
			}

			static reference _value( node_pointer node )
			{
				return (*node).value_;
			}

			void swap( self_type& rhs )
			{
				if( this != &rhs )
				{
					//Swap internal representation
					gstl::swap( tail_,				rhs.tail_ );
					gstl::swap( alloc_,				rhs.alloc_ );
					gstl::swap( node_alloc_,		rhs.node_alloc_ );
					gstl::swap( node_ptr_alloc_,	rhs.node_ptr_alloc_ );
					gstl::swap( size_,				rhs.size_ );
				}
			}
		};
	}
	
}

#endif //GSTL_LIST_BASE_HEADER



