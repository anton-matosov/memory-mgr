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


#pragma once

#include <gstl/allocator>
#include <gstl/iterator>
#include <gstl/detail/list_base.hpp>
#include <gstl/detail/container_helpers.hpp>
#include <gstl/detail/list_iterator.hpp>
#include <gstl/detail/fill_iterator.hpp>

namespace gstl
{


	template
	<
		class T,
		class Alloc = allocator<T>
	>
	class list
		:private detail::list_base<T, Alloc>
	{
	public:
		
		//////////////////////////////////////////////////////////////////////////
		//Node types
		typedef detail::list_base<T, Alloc>		base_type;
		typedef typename base_type::node_type				node_type;
		typedef typename base_type::node_allocator_type		node_allocator_type;
		typedef typename base_type::node_pointer			node_pointer;
		typedef typename base_type::node_const_pointer		node_const_pointer;

		//////////////////////////////////////////////////////////////////////////
		//Standard types
		typedef list										self_type;

		typedef typename base_type::allocator_type			allocator_type;

		typedef typename base_type::value_type				value_type;
		typedef typename base_type::pointer					pointer;
		typedef typename base_type::const_pointer			const_pointer;
		typedef typename base_type::reference				reference;
		typedef typename base_type::const_reference			const_reference;

		typedef typename base_type::size_type				size_type;
		typedef typename base_type::difference_type			difference_type;


		//////////////////////////////////////////////////////////////////////////
		//Iterator types
		template <class NodePtrT, class ContainerT>
		friend class detail::list_iterator;

		typedef detail::declare_list_iterator<self_type>	iter_helper;

		typedef typename iter_helper::iterator				iterator; //See 23.1
		typedef typename iter_helper::const_iterator		const_iterator; // See 23.1

		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;
		//////////////////////////////////////////////////////////////////////////
		
		// 23.2.2.1 construct/copy/destroy:
		explicit list(const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{

		}

		explicit list(size_type n, const value_type& value = value_type(),
			const allocator_type& alloc = allocator_type() )
			:base_type( alloc )
		{
			insert( begin(), n, value );
		}

		template <class InputIterator>
		list(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			insert( begin(), first, last );
		}

		list( const self_type& x )
			:base_type( x.get_allocator() )
		{
			*this = x;
		}

		~list()
		{
			clear();			
		}

		self_type& operator=(const self_type& x)
		{
			assign( x.begin(), x.end() );
			return *this;
		}

		template <class InputIterator>
		void assign( InputIterator first, InputIterator last )
		{
			erase( begin(), end() );
			insert( begin(), first, last );
		}

		void assign( size_type n, const T& t )
		{
			erase( begin(), end() );
			insert( begin(), n, t );
		}

		allocator_type get_allocator() const
		{
			return alloc_;
		}

		// iterators:
		// 21.3.2 iterators:
		iterator begin()
		{
			return iter_helper::build_iter( base_type::_next( tail_ ), this );
		}

		const_iterator begin() const
		{
			return iter_helper::build_const_iter( this->_next( tail_ ), this );
		}

		iterator end()
		{
			return iter_helper::build_iter( tail_, this );
		}

		const_iterator end() const
		{
			return iter_helper::build_const_iter( tail_, this );
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator( end() );
		}

		const_reverse_iterator rbegin() const
		{			
			return  const_reverse_iterator( end() );
		}

		reverse_iterator rend()
		{
			return reverse_iterator( begin() );
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator( begin() );
		}

		void resize(size_type sz, T c = T())
		{
			detail::container::resize( this, sz, c );
		}
		
		// 23.2.2.2 capacity:
		bool empty() const
		{
			return detail::container::empty( this );
		}

		size_type size() const
		{
			return size_;
		}

		size_type max_size() const
		{
			size_type max = alloc_.max_size();
			return max <= 1 ? 1 : max - 1;
		}

		// element access:
		reference front()
		{
			return detail::container::front( this );
		}

		const_reference front() const
		{
			return detail::container::front( this );
		}

		reference back()
		{
			return detail::container::back( this );
		}

		const_reference back() const
		{
			return detail::container::back( this );
		}


		// 23.2.2.3 modifiers:
		void push_front( const value_type& x )
		{
			detail::container::push_front( this, x );
		}

		void pop_front()
		{
			detail::container::pop_front( this );
		}

		void push_back( const value_type& x )
		{
			detail::container::push_back( this, x );
		}

		void pop_back()
		{
			detail::container::pop_back( this );
		}

		//////////////////////////////////////////////////////////////////////////
		//Insert methods
		iterator insert( iterator position, const value_type& x )
		{
			return _do_insert( position, &x, &x + 1, random_access_iterator_tag() );
		}

		void insert( iterator position, size_type n, const value_type& x )
		{
			typedef fill_iterator_ref<const value_type>  fill_iter;
			insert( position,  fill_iter( x ),  fill_iter( x, n ) );
		}

		template <class InputIterator>
		void insert( iterator position,
			InputIterator first, InputIterator last )
		{
			_do_insert( position, first, last, GSTL_ITER_CAT( InputIterator ) );
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//Erase methods
		iterator erase( iterator position )
		{
			node_pointer node = position.base();
			++position;

			//If node is not the tail (end) entry of the list
			if( node != tail_ )
			{
				//We can remove it
				this->_remove_node( node );
				this->_free_node( node );
				--size_;
			}
			return position;
		}

		iterator erase( iterator position, iterator last )
		{
			while( position != last )
			{
				position = erase( position );
			}
			return position;
		}
		//////////////////////////////////////////////////////////////////////////

		void swap( self_type& rhs )
		{
			base_type::swap( rhs );
		}
		
		void clear()
		{
			detail::container::clear( this );
		}

		// 23.2.2.4 list operations:
		void splice(iterator position, self_type& x);
		void splice(iterator position, self_type& x, iterator i);
		void splice(iterator position, self_type& x, iterator first,
			iterator last);

		void remove( const value_type& value )
		{
			iterator fres = gstl::find( begin(), end(), value );
			if( fres != end() )
			{
				erase( fres );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		template <class Predicate>
		void remove_if( Predicate pred )
		{
			iterator fres = gstl::find_if( begin(), end(), pred );
			if( fres != end() )
			{
				erase( fres );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		void unique();

		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);

		//////////////////////////////////////////////////////////////////////////
		void merge(self_type& x);

		template <class Compare>
		void merge(self_type& x, Compare comp);
		
		//////////////////////////////////////////////////////////////////////////
		void sort()
		{
			
		}

		template <class Compare>
		void sort( Compare comp )
		{

		}

		//////////////////////////////////////////////////////////////////////////
		void reverse()
		{
			node_pointer	first = first_;
			node_pointer	last = last_;
// 			for( ; first != last && first != --last; ++first )
// 			{
// 				iter_swap( first, last );
// 			}
		}
	private:

		template <class InputIterator>
		void _do_insert( iterator position,
			InputIterator n, InputIterator x, integral_iterator_tag )
		{
			insert( position, static_cast<size_type>( n ), static_cast<const value_type&>( x ) );
		}

		template <class InputIterator>
		iterator _do_insert( iterator position,
			InputIterator first, InputIterator last, input_iterator_tag )
		{
			iterator pos = position;
			while( first != last )
			{
				insert( pos, *first );
				++first;
				++pos;
			}
			return position;
		}

		template <class FwdIterator>
		iterator _do_insert( iterator position,
			FwdIterator first, FwdIterator last, forward_iterator_tag )
		{
			//size_type new_items_count = gstl::distance( first, last );
			

			node_pointer curr = position.base();
			while( first != last )
			{
				node_ptr_reference prev = _prev( curr );
				node_pointer new_node = _create_node( curr, prev, *first );

				//curr->prev_ = new_node
				prev = new_node;
				//new_node->prev_->next_ = new_node
				_next( _prev( new_node ) ) = new_node;
				

				++first;
				++size_;
			}
			
			return position;
		}

		void _destroy( iterator first, iterator last )
		{
			while( first != last )
			{
				alloc_.destroy( &this->_value( first.base() ) );
				++first;
			}
		};
	};

	template<class value_type, class allocator>
	bool operator==( const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return ( lhs.size() == rhs.size() ) && gstl::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	template<class value_type, class allocator>
	bool operator< (const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return gstl::lexicographical_compare( lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}

	template<class value_type, class allocator>
	bool operator!=( const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator>( const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return rel_ops::operator >( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator>=( const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return rel_ops::operator >=( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator<=( const list<value_type, allocator>& lhs,
		const list<value_type, allocator>& rhs )
	{
		return rel_ops::operator <=( lhs, rhs );
	}

	template<class value_type, class allocator>
	void swap( list<value_type, allocator>& lhs,
		list<value_type, allocator>& rhs )
	{
		lhs.swap( rhs );
	}
}




