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

#ifndef GSTL_VECTOR_HEADER
#define GSTL_VECTOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/allocator>
#include <gstl/algorithm>
#include <gstl/memory>
#include <gstl/iterator>
#include <gstl/detail/container_helpers.hpp>
#include <gstl/detail/dynamic_buffer.hpp>
#include <gstl/detail/fill_iterator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>

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
		:public detail::dynamic_buffer<T, Alloc, detail::default_sequence_traits<T>, PtrTraits>
	{
	public:
		typedef detail::dynamic_buffer<T, Alloc,
			detail::default_sequence_traits<T>, PtrTraits>	base_type;
		typedef base_type							internal_buffer_type;

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
		typedef detail::declare_sequence_iterator<self_type> iter_helper;

		typedef typename iter_helper::iterator			iterator; //See 23.1
		typedef typename iter_helper::const_iterator	const_iterator; // See 23.1

		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;

		// 23.2.4.1 construct/copy/destroy:
		explicit vector(const allocator_type& a = allocator_type())
			:base_type( a )
		{}

		explicit vector(size_type n, const T& value = T(),
			const allocator_type& a = allocator_type())
			:base_type( a )
		{
			insert( begin(), n, value );
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const allocator_type& a = allocator_type())
			:base_type( a )
		{
			insert( begin(), first, last );
		}

		vector( const self_type& x )
			:base_type( x.get_allocator() )
		{
			*this = x;
		}

		~vector()
		{
			destroy( begin(), end() );
		}

		self_type& operator=( const self_type& x )
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

		// 21.3.2 iterators:
		iterator begin()
		{
			return iter_helper::build_iter( get_buffer() );
		}

		const_iterator begin() const
		{
			return iter_helper::build_iter( get_buffer() );
		}

		iterator end()
		{
			return begin() + size_;
		}

		const_iterator end() const
		{
			return begin() + size_;
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

		using base_type::get_allocator;

		// 23.2.4.2 capacity:
		using base_type::max_size;
		using base_type::size;
		using base_type::capacity;
		using base_type::reserve;
		using base_type::empty;
		
		void resize(size_type sz, T c = T())
		{
			detail::container::resize( this, sz, c );
		}

		// element access:
		using base_type::operator[];
		using base_type::at;

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
		
		void push_back( const value_type& x )
		{
			detail::container::push_back( this, x );
		}

		void pop_back()
		{
			detail::container::pop_back( this );
		}

		iterator insert( iterator position, const value_type& x )
		{
			return do_insert( position, &x, &x + 1, random_access_iterator_tag() );
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
			do_insert( position, first, last, GSTL_ITER_CAT( InputIterator ) );
		}

		iterator erase( iterator position )
		{
			return erase( position, position + 1 );
		}

		iterator erase( iterator first, iterator last )
		{
			GSTL_DEBUG_RANGE( first, last );
			if( first != last )
			{
				detail::move( &*first, &*last, end() - last );
				set_size( size() - (last - first) );
				destroy( first, last );
			}
			return first;
		}
		
		void swap( self_type& rhs )
		{
			base_type::swap( rhs );
		}

		void clear()
		{
			detail::container::clear( this );
		}

	private:
		template <class InputIterator>
		void do_insert( iterator position,
			InputIterator n, InputIterator x, integral_iterator_tag )
		{
			insert( position, static_cast<size_type>( n ), static_cast<const value_type&>( x ) );
		}

		template <class InputIterator>
		iterator do_insert( iterator position,
			InputIterator first, InputIterator last, input_iterator_tag )
		{
			size_type pos = position - begin();
			while( first != last )
			{
				insert( position, *first );
				++first;
				++position;
			}
			return iterator( begin() + pos );
		}

		template <class FwdIterator>
		iterator do_insert( iterator position,
			FwdIterator first, FwdIterator last, forward_iterator_tag )
		{
			size_type new_items_count = gstl::distance( first, last );
			size_type new_size = size() + new_items_count;

			iterator result_pos = position;
			if( new_size > capacity() )
			{
				internal_buffer_type tmp_buff;
				tmp_buff.reserve( new_size + 10 );
				value_type* tmp_begin = tmp_buff.get_buffer();

				value_type* tmp_pos = tmp_begin;
				try
				{
					//Copy prefix
					tmp_pos = uninitialized_copy( begin(), position, tmp_pos );
					//Copy new
					tmp_pos = uninitialized_copy( first, last, tmp_pos );
					//Copy suffix
					tmp_pos = uninitialized_copy( position, end(), tmp_pos );
				}
				catch(...)
				{
					destroy( tmp_begin, tmp_pos );
					throw;
				}
				destroy( begin(), end() );
				result_pos = iter_helper::build_iter( 
					tmp_begin + gstl::distance( begin(), position ) );
				
				base_type::swap( tmp_buff );
			}
			else
			{
				//Buffer size is enough to hold new items
				iterator vec_end = end();
				//Insert them past the end of existent items
				iterator new_end = uninitialized_copy( first, last, vec_end );
				
				//put new items on their position
				reverse( position, vec_end );
				reverse( vec_end, new_end );
				reverse( position, new_end );
			}
			set_size( new_size );
			return result_pos;
		}

		template<class InputIterator>
		bool check_overlap( InputIterator first, InputIterator last )
		{
			return false;
		}

		bool check_overlap( iterator first, iterator last )
		{
			return detail::check_overlap( &*begin(), &*end(), &*first, &*last ) != 0;
		}

		bool check_overlap( const value_type* first, const value_type* last )
		{
			return detail::check_overlap( &*begin(), &*end(), first, last ) != 0;
		}

		void destroy( iterator first, iterator last )
		{
			destroy( pointer(&*first), pointer(&*last) );
		};

		void destroy( pointer first, pointer last )
		{
			while( first != last )
			{
				alloc_.destroy( first );
				++first;
			}
		}

		template<class T>
		void destroy( T* first, T* last, 
			typename boost::disable_if< boost::is_pointer<pointer>, T >::type* = 0 )
		{
			destroy( iter_helper::build_iter( first ),
				iter_helper::build_iter( last ) );
		};
	};



	template<class value_type, class allocator, class pointer_traits>
	bool operator==( const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return ( lhs.size() == rhs.size() ) && gstl::equal( lhs.begin(), lhs.end(), rhs.begin() );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator< (const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return gstl::lexicographical_compare( lhs.begin(), lhs.end(),
				rhs.begin(), rhs.end());
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator!=( const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator>( const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator >( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator>=( const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator >=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	bool operator<=( const vector<value_type, allocator, pointer_traits>& lhs,
		const vector<value_type, allocator, pointer_traits>& rhs )
	{
		return rel_ops::operator <=( lhs, rhs );
	}

	template<class value_type, class allocator, class pointer_traits>
	void swap( vector<value_type, allocator, pointer_traits>& lhs,
		vector<value_type, allocator, pointer_traits>& rhs )
	{
		lhs.swap( rhs );
	}

}

#endif GSTL_VECTOR_HEADER