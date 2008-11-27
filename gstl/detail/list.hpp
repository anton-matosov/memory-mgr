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

namespace gstl
{
	template <class T, class Allocator = allocator<T> >
	class list {
	public:
		// types:
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
 		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		typedef value_type* iterator; // See 23.1
		typedef const value_type* const_iterator; // See 23.1
		typedef size_t size_type; // See 23.1
		typedef ptrdiff_t difference_type;// See 23.1

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		// 23.2.2.1 construct/copy/destroy:
		explicit list(const Allocator& = Allocator());
		explicit list(size_type n, const T& value = T(),
			const Allocator& = Allocator());
		template <class InputIterator>
		list(InputIterator first, InputIterator last,
			const Allocator& = Allocator());
		list(const list<T,Allocator>& x);
		~list();
		list<T,Allocator>& operator=(const list<T,Allocator>& x);
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const T& t);
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
		void resize(size_type sz, T c = T());
		// element access:
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		// 23.2.2.3 modifiers:
		void push_front(const T& x);
		void pop_front();
		void push_back(const T& x);
		void pop_back();
		iterator insert(iterator position, const T& x);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position, InputIterator first,
			InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator position, iterator last);
		void swap(list<T,Allocator>&);
		void clear();
		// 23.2.2.4 list operations:
		void splice(iterator position, list<T,Allocator>& x);
		void splice(iterator position, list<T,Allocator>& x, iterator i);
		void splice(iterator position, list<T,Allocator>& x, iterator first,
			iterator last);
		void remove(const T& value);
		template <class Predicate> void remove_if(Predicate pred);
		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list<T,Allocator>& x);
		template <class Compare> void merge(list<T,Allocator>& x, Compare comp);
		void sort();
		template <class Compare> void sort(Compare comp);
		void reverse();
	};

		template <class T, class Allocator>
		bool operator==(const list<T,Allocator>& x, const list<T,Allocator>& y);
		template <class T, class Allocator>
		bool operator< (const list<T,Allocator>& x, const list<T,Allocator>& y);
		template <class T, class Allocator>
		bool operator!=(const list<T,Allocator>& x, const list<T,Allocator>& y);
		template <class T, class Allocator>
		bool operator> (const list<T,Allocator>& x, const list<T,Allocator>& y);
		template <class T, class Allocator>
		bool operator>=(const list<T,Allocator>& x, const list<T,Allocator>& y);
		template <class T, class Allocator>
		bool operator<=(const list<T,Allocator>& x, const list<T,Allocator>& y);
		// specialized algorithms:
		template <class T, class Allocator>
		void swap(list<T,Allocator>& x, list<T,Allocator>& y);
}


#endif //GSTL_LIST_HEADER


