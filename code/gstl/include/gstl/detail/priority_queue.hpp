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

namespace gstl
{
	template 
	<
		class T,
		class Container = vector<T>,
		class Compare = less<typename Container::value_type>
	>
	class priority_queue
	{
	public:
		typedef Container	container_type;
		typedef Compare		compare_type;

		typedef typename container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		
	protected:
		container_type	container_;
		compare_type	comp_;
	public:
		explicit priority_queue(const compare_type& cmp = compare_type(),
			const container_type& cont = container_type())
			:comp_( cmp ),
			container_( cont )
		{

		}

		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last,
			const compare_type& cmp = compare_type(),
			const container_type& cont = container_type() )
			:comp_( cmp ),
			container_( cont )
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

		const value_type& top() const
		{
			return container_.front();
		}

		void push(const value_type& x);
		void pop();
	};
	// no equality is provided
}



