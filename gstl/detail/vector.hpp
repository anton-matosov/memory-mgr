/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

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

#ifndef GSTL_VECTOR_HEADER
#define GSTL_VECTOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/allocator.hpp>

namespace gstl
{
	template <class T, class Allocator = allocator<T> >
	class vector {
	public:
		// types:
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef implementation defined iterator; // See 23.1
		typedef implementation defined const_iterator; // See 23.1
		typedef implementation defined size_type; // See 23.1
		typedef implementation defined difference_type;// See 23.1
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		// 23.2.4.1 construct/copy/destroy:
		explicit vector(const Allocator& = Allocator());
		explicit vector(size_type n, const T& value = T(),
			const Allocator& = Allocator());
		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const Allocator& = Allocator());
		vector(const vector<T,Allocator>& x);
		~vector();
		vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const T& u);
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
		// 23.2.4.2 capacity:
		size_type size() const;
		size_type max_size() const;
		void resize(size_type sz, T c = T());
		size_type capacity() const;
		bool empty() const;
		void reserve(size_type n);
		// element access:
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		const_reference at(size_type n) const;
		reference at(size_type n);
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		
		void push_back(const T& x);
		void pop_back();
		iterator insert(iterator position, const T& x);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert(iterator position,
			InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(vector<T,Allocator>&);
		void clear();
	};



	template <class T, class Allocator>
	bool operator==(const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	bool operator< (const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	bool operator!=(const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	bool operator> (const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	bool operator>=(const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	bool operator<=(const vector<T,Allocator>& x,
		const vector<T,Allocator>& y);
	template <class T, class Allocator>
	void swap(vector<T,Allocator>& x, vector<T,Allocator>& y);

	template <class Allocator> class vector<bool,Allocator>;
	template <class Allocator>
	bool operator==(const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	bool operator< (const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	bool operator!=(const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	bool operator> (const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	bool operator>=(const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	bool operator<=(const vector<bool,Allocator>& x,
		const vector<bool,Allocator>& y);
	template <class Allocator>
	void swap(vector<bool,Allocator>& x, vector<bool,Allocator>& y);
}

#endif GSTL_VECTOR_HEADER