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
#include "gstl/detail/algorithm.hpp"
#include <iterator>

namespace gstl
{
	template
	<
		class T,
		class Alloc = allocator<T>
	>
	class deque
	{
	public:
		// types:
		typedef typename Alloc::reference reference;
		typedef typename Alloc::const_reference const_reference;
		

		typedef T value_type;
		typedef Alloc allocator_type;
		typedef typename Alloc::pointer pointer;
		typedef typename Alloc::const_pointer const_pointer;

		typedef value_type* iterator; // See 23.1
		typedef const value_type* const_iterator; // See 23.1
		typedef size_t size_type; // See 23.1
		typedef ptrdiff_t difference_type;// See 23.1

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		// 23.2.1.1 construct/copy/destroy:
		explicit deque(const Alloc& = Alloc());
		explicit deque(size_type n, const T& value = T(),
			const Alloc& = Alloc());
		template <class InputIterator>
		deque(InputIterator first, InputIterator last,
			const Alloc& = Alloc());
		deque(const deque<T,Alloc>& x);
		~deque();
		deque<T,Alloc>& operator=(const deque<T,Alloc>& x);
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
		// 23.2.1.2 capacity:
		size_type size() const;
		size_type max_size() const;
		void resize(size_type sz, T c = T());
		bool empty() const;
		// element access:
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		reference at(size_type n);
		const_reference at(size_type n) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		// 23.2.1.3 modifiers:
		void push_front(const T& x);
		void push_back(const T& x);
		iterator insert(iterator position, const T& x);
		void insert(iterator position, size_type n, const T& x);
		template <class InputIterator>
		void insert (iterator position,
			InputIterator first, InputIterator last);
		void pop_front();
		void pop_back();
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(deque<T,Alloc>&);
		void clear();
	};
	

	template<class value_type, class allocator>
	bool operator==( const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return ( lhs.size() == rhs.size() ) && gstl::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	template<class value_type, class allocator>
	bool operator< (const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return gstl::lexicographical_compare( lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}

	template<class value_type, class allocator>
	bool operator!=( const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator>( const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return rel_ops::operator >( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator>=( const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return rel_ops::operator >=( lhs, rhs );
	}

	template<class value_type, class allocator>
	bool operator<=( const deque<value_type, allocator>& lhs,
		const deque<value_type, allocator>& rhs )
	{
		return rel_ops::operator <=( lhs, rhs );
	}

	template<class value_type, class allocator>
	void swap( deque<value_type, allocator>& lhs,
		deque<value_type, allocator>& rhs )
	{
		lhs.swap( rhs );
	}
}




