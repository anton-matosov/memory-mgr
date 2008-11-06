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

#include <gstl/allocator>
#include <gstl/iterator>
#include <gstl/detail/dynamic_sequence.hpp>

namespace gstl
{
	//23.2.4 Class template vector
	//page 489 (real page - 515)
	template
		<
			class T,
			class Alloc = allocator<T>,
			class PtrTraits = typename Alloc::pointer_traits_type
		>
	class vector
		:public detail::dynamic_sequence<T, Alloc, detail::default_sequence_traits<T>, PtrTraits>
	{
	public:
		typedef detail::dynamic_sequence<T, Alloc,
			detail::default_sequence_traits<T>, PtrTraits>	base_type;

		// types:
		typedef vector								self_type;
		typedef typename base_type::ptr_traits		ptr_traits;

		typedef typename base_type::allocator_type	allocator_type;

		typedef typename base_type::value_type		value_type;
		typedef typename base_type::pointer			pointer;
		typedef typename base_type::const_pointer	const_pointer;
		typedef typename base_type::reference		reference;
		typedef typename base_type::const_reference	const_reference;

		typedef typename base_type::size_type		size_type;
		typedef typename base_type::difference_type	difference_type;


		//////////////////////////////////////////////////////////////////////////
		typedef value_type* iterator; //See 23.1
		typedef const value_type* const_iterator; // See 23.1
		//typedef typename pointer iterator; //See 23.1
		//typedef typename const_pointer const_iterator; // See 23.1
		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;

		// 23.2.4.1 construct/copy/destroy:
		explicit vector(const allocator_type& = allocator_type());
		explicit vector(size_type n, const T& value = T(),
			const allocator_type& = allocator_type());

		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const allocator_type& = allocator_type());

		vector(const self_type& x);
		~vector();

		self_type& operator=(const self_type& x);

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
		void swap( self_type& );
		void clear();
	};



	template<class value_type, class allocator, class pointer_traits>
	bool operator==(const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	bool operator< (const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	bool operator!=(const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	bool operator> (const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	bool operator>=(const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	bool operator<=(const vector<value_type, allocator, pointer_traits>& x,
		const vector<value_type, allocator, pointer_traits>& y);

	template<class value_type, class allocator, class pointer_traits>
	void swap(vector<value_type, allocator, pointer_traits>& x, vector<value_type, allocator, pointer_traits>& y);

}

#endif GSTL_VECTOR_HEADER