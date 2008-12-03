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

#ifndef GSTL_LIST_HEADER
#define GSTL_LIST_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/allocator>
#include <gstl/iterator>
#include <gstl/detail/list_node.hpp>
#include <gstl/detail/list_iterator.hpp>

namespace gstl
{


	template
	<
		class T,
		class Alloc = allocator<T>,
		class PtrTraits = typename Alloc::pointer_traits_type
	>
	class list {
	public:
		
		//////////////////////////////////////////////////////////////////////////
		//Node types
		typedef detail::list_node<T, Alloc, PtrTraits>		node_type;
		typedef typename node_type::node_allocator_type		node_allocator_type;
		typedef typename node_type::node_pointer			node_pointer;
		typedef typename node_type::node_const_pointer		node_const_pointer;

		//////////////////////////////////////////////////////////////////////////
		//Standard types
		typedef list										self_type;
		typedef typename node_type::ptr_traits				ptr_traits;

		typedef typename node_type::allocator_type			allocator_type;

		typedef typename node_type::value_type				value_type;
		typedef typename node_type::pointer					pointer;
		typedef typename node_type::const_pointer			const_pointer;
		typedef typename node_type::reference				reference;
		typedef typename node_type::const_reference			const_reference;

		typedef typename node_type::size_type				size_type;
		typedef typename node_type::difference_type			difference_type;


		//////////////////////////////////////////////////////////////////////////
		//Iterator types
		typedef detail::declare_list_iterator<self_type>	iter_helper;

		typedef typename iter_helper::iterator				iterator; //See 23.1
		typedef typename iter_helper::const_iterator		const_iterator; // See 23.1

		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;
		//////////////////////////////////////////////////////////////////////////
		
		// 23.2.2.1 construct/copy/destroy:
		explicit list(const allocator_type& alloc = allocator_type())
			:alloc_( alloc )
		{

		}

		explicit list(size_type n, const value_type& value = value_type(),
			const allocator_type& alloc = allocator_type() )
			:alloc_( alloc )
		{
			insert( begin(), n, value );
		}

		template <class InputIterator>
		list(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type())
			:alloc_( alloc )
		{
			insert( begin(), first, last );
		}

		list( const self_type& x )
			:alloc_( x.get_allocator() )
		{
			*this = x;
		}

		~list()
		{
			destroy( begin(), end() );
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
			return iter_helper::build_iter( first_ );
		}

		const_iterator begin() const
		{
			return iter_helper::build_const_iter( first_ );
		}

		iterator end()
		{
			return iter_helper::build_iter( last_ );
		}

		const_iterator end() const
		{
			return iter_helper::build_const_iter( last_ );
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
			detail::container::empty( this );
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
		iterator insert(iterator position, const value_type& x);

		void insert(iterator position, size_type n, const value_type& x);

		template <class InputIterator>
		void insert(iterator position, InputIterator first,
			InputIterator last);
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//Erase methods
		iterator erase( iterator position );
		iterator erase( iterator position, iterator last );
		//////////////////////////////////////////////////////////////////////////

		void swap(self_type&);
		
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
				erase( end() );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		template <class Predicate>
		void remove_if( Predicate pred )
		{
			iterator fres = gstl::find_if( begin(), end(), pred );
			if( fres != end() )
			{
				erase( end() );
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
		node_pointer	first_;
		node_pointer	last_;

		allocator_type	alloc_;
		size_type		size_;

		void destroy( iterator first, iterator last )
		{
			while( first != last )
			{
				alloc_.destroy( first.base()->value_ptr_ );
				++first;
			}
		};
	};

	template<class value_type, class allocator, class pointer_traits>
	bool operator==( const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return ( lhs.size() == rhs.size() ) && gstl::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator< (const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return gstl::lexicographical_compare( lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator!=( const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator>( const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator >( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator>=( const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator >=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator<=( const list<value_type, allocator, pointer_traits>& lhs,
		const list<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator <=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	void swap( list<value_type, allocator, pointer_traits>& lhs,
		list<value_type, allocator, pointer_traits>& rhs )
	{
		lhs.swap( rhs );
	}
}


#endif //GSTL_LIST_HEADER


