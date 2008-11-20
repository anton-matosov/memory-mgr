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

#ifndef GSTL_QUEUE_HEADER
#define GSTL_QUEUE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace gstl
{
	template <class T, class Container = deque<T> >
	class queue
	{
	public:
		typedef Container container_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		
	public:
		explicit queue( const container_type& cont = container_type() )
			:container_( cont )
		{
		}

		bool empty() const
		{
			return container_.empty();
		}

		size_type size() const
		{
			return container_.size();
		}

		value_type& front()
		{
			return container_.front();
		}

		const value_type& front() const
		{
			return container_.front();
		}

		value_type& back()
		{
			return container_.back();
		}

		const value_type& back() const
		{
			return container_.back();
		}

		void push(const value_type& x)
		{
			container_.push_back(x);
		}

		void pop()
		{
			container_.pop_front();
		}
	protected:
		container_type container_;
	};
	
	template <class T, class Container>
	bool operator==(const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs == rhs;
	}

	template <class T, class Container>
	bool operator< (const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs < rhs;
	}

	template <class T, class Container>
	bool operator!=(const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs != rhs;
	}

	template <class T, class Container>
	bool operator> (const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs > rhs;
	}

	template <class T, class Container>
	bool operator>=(const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs >= rhs;
	}

	template <class T, class Container>
	bool operator<=(const queue<T, Container>& lhs,
		const queue<T, Container>& rhs)
	{
		return lhs <= rhs;
	}

}

#endif //GSTL_QUEUE_HEADER

