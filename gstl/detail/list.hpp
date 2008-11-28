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
		// types:
		typedef detail::list_node<T, Alloc, PtrTraits>	node_type;

		typedef list								self_type;
		typedef typename node_type::ptr_traits		ptr_traits;

		typedef typename node_type::allocator_type	allocator_type;

		typedef typename node_type::value_type		value_type;
		typedef typename node_type::pointer			pointer;
		typedef typename node_type::const_pointer	const_pointer;
		typedef typename node_type::reference		reference;
		typedef typename node_type::const_reference	const_reference;

		typedef typename node_type::size_type		size_type;
		typedef typename node_type::difference_type	difference_type;

		typedef typename node_type::node_pointer		node_pointer;
		typedef typename node_type::node_const_pointer	node_const_pointer;

		//////////////////////////////////////////////////////////////////////////
		typedef detail::declare_list_iterator<self_type> iter_helper;

		typedef typename iter_helper::iterator			iterator; //See 23.1
		typedef typename iter_helper::const_iterator	const_iterator; // See 23.1

		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;
		
		// 23.2.2.1 construct/copy/destroy:
		explicit list(const allocator_type& = allocator_type());

		explicit list(size_type n, const value_type& value = value_type(),
			const allocator_type& = allocator_type());

		template <class InputIterator>
		list(InputIterator first, InputIterator last,
			const allocator_type& = allocator_type());

		list(const self_type& x);
		~list();

		self_type& operator=(const self_type& x);

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const value_type& t);
		allocator_type get_allocator() const;

		// iterators:
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		
		// 23.2.2.2 capacity:
		bool empty() const;
		size_type size() const;
		size_type max_size() const;
		void resize(size_type sz, value_type c = value_type());

		// element access:
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;

		// 23.2.2.3 modifiers:
		void push_front(const value_type& x);
		void pop_front();
		void push_back(const value_type& x);
		void pop_back();

		iterator insert(iterator position, const value_type& x);
		void insert(iterator position, size_type n, const value_type& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first,
			InputIterator last);

		iterator erase(iterator position);
		iterator erase(iterator position, iterator last);

		void swap(self_type&);
		void clear();

		// 23.2.2.4 list operations:
		void splice(iterator position, self_type& x);
		void splice(iterator position, self_type& x, iterator i);
		void splice(iterator position, self_type& x, iterator first,
			iterator last);

		void remove(const value_type& value);

		//////////////////////////////////////////////////////////////////////////
		template <class Predicate> void remove_if(Predicate pred);
		void unique();

		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);

		void merge(self_type& x);

		template <class Compare> void merge(self_type& x, Compare comp);
		void sort();

		template <class Compare> void sort(Compare comp);
		void reverse();
	};

		template <class value_type, class allocator_type>
		bool operator==(const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		template <class value_type, class allocator_type>
		bool operator< (const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		template <class value_type, class allocator_type>
		bool operator!=(const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		template <class value_type, class allocator_type>
		bool operator> (const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		template <class value_type, class allocator_type>
		bool operator>=(const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		template <class value_type, class allocator_type>
		bool operator<=(const list<value_type,allocator_type>& x, const list<value_type,allocator_type>& y);

		// specialized algorithms:
		template <class value_type, class allocator_type>
		void swap(list<value_type,allocator_type>& x, list<value_type,allocator_type>& y);
}


#endif //GSTL_LIST_HEADER


