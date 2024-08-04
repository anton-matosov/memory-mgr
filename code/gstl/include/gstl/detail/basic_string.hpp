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

#include <iosfwd>
#include <gstl/algorithm>
#include <gstl/utility>
#include <gstl/iterator>
#include <gstl/allocator>
#include <gstl/detail/char_traits.hpp>
#include <gstl/detail/dynamic_buffer.hpp>
#include <gstl/detail/container_helpers.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/or.hpp>

#pragma warning (disable: 4100)

namespace gstl
{
	namespace detail
	{
		/*
		21 Strings library [lib.strings]
		1 This clause describes components for manipulating sequences of “characters,” where characters may be of
		any POD (3.9) type. In this clause such types are called char-like types, and objects of char-like types are
		called char-like objects or simply “characters.”
		*/

		template
			<
				class CharT,
				class Traits,
				class Alloc
			>
		class string_base
			:public detail::dynamic_buffer<CharT, Alloc, Traits>
		{
		public:
			enum{ min_buff_size = 4 };

			typedef detail::dynamic_buffer<CharT, Alloc, Traits>	base_type;

			typedef CharT									value_type;
			typedef Traits									traits_type;

			typedef typename base_type::allocator_type		allocator_type;

			typedef typename base_type::pointer				pointer;
			typedef typename base_type::const_pointer		const_pointer;
			typedef typename base_type::reference			reference;
			typedef typename base_type::const_reference		const_reference;

			typedef typename base_type::size_type			size_type;
			typedef typename base_type::difference_type		difference_type;
									
			string_base( const allocator_type& alloc = allocator_type() )
				:base_type( alloc )
			{
				this->reserve( min_buff_size );
				set_end(0);
			}

			void set_end( size_type n )
			{
				this->size_ = n;
				this->buffer_[n] = value_type();
			}
			
			void reset_ptr( pointer new_str, size_type new_size, size_type new_reserved )
			{
				base_type::reset_buffer( new_str, new_size, new_reserved );
				set_end( new_size );
			}
		};

	}

	template
	<
		class CharT,
		class Traits = char_traits<CharT>,
		class Alloc = allocator<CharT>
	>
	class basic_string
		:private detail::string_base<CharT, Traits, Alloc>
	{
		typedef detail::string_base<CharT, Traits, Alloc> base_type;
	public:
		//////////////////////////////////////////////////////////////////////////
		// types:
		typedef basic_string			self_type;
		typedef typename base_type::value_type			value_type;
		typedef typename base_type::traits_type	traits_type;
		typedef typename base_type::allocator_type		allocator_type;

		typedef typename base_type::pointer			pointer;
		typedef typename base_type::const_pointer	const_pointer;
		typedef typename base_type::reference		reference;
		typedef typename base_type::const_reference	const_reference;

		typedef typename base_type::size_type		size_type;
		typedef typename base_type::difference_type	difference_type;


		//////////////////////////////////////////////////////////////////////////
		//typedef value_type*			iterator; //See 23.1
		//typedef const value_type*	const_iterator; // See 23.1
		typedef detail::declare_sequence_iterator<self_type> iter_helper;

		typedef typename iter_helper::iterator			iterator; //See 23.1
		typedef typename iter_helper::const_iterator		const_iterator; // See 23.1

		//typedef typename pointer iterator; //See 23.1
		//typedef typename const_pointer const_iterator; // See 23.1
		typedef gstl::reverse_iterator<iterator> reverse_iterator;
		typedef gstl::reverse_iterator<const_iterator> const_reverse_iterator;
		static const size_type npos = static_cast<size_type>( -1 );
		
		// 21.3.1 construct/copy/destroy:
		/**
		@brief Constructs an object of class basic_string. The postconditions of this function are indicated
			in Table 38:
			data() a non-null value_type* that is copyable and can have 0 added to it
			size() 0
			capacity() an unspecified value
		*/
		explicit basic_string( const allocator_type& alloc = allocator_type() )
			:base_type( alloc )
		{
			
		}

		/**
			@brief 
			Requires: pos <= str.size()
			Throws: out_of_range if pos > str.size().
			Effects: Constructs an object of class basic_string and determines the effective length rlen of the
				initial string value as the smaller of n and str.size() - pos, as indicated in Table 39. In the first
				form, the Allocator value used is copied from str.get_allocator().
			Table 39:
			data() points at the first element of an
				allocated copy of rlen consecutive
				elements of the string controlled
				by str beginning at position pos
			size() rlen
			capacity() a value at least as large as size()
		*/
		basic_string(const basic_string& str)
			:base_type( str.get_allocator() )
		{
			assign( str, 0, npos );
		}

		/**
		   @brief the same as basic_string(const basic_string& str)
		*/
		basic_string(const basic_string& str, size_type pos, size_type n = npos,
			const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			assign( str, pos, n );
		}

		basic_string(const value_type* s,
			size_type n, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			assign( s, n );
		}

		basic_string(const value_type* s, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			assign( s );
		}

		basic_string(size_type n, value_type c, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			assign( n, c );
		}

		/*		
		Effects: If InputIterator is an integral type, equivalent to
		basic_string(static_cast<size_type>(begin), static_cast<value_type>(end))
		Otherwise constructs a string from the values in the range [begin, end), as indicated in the Sequence
		Requirements table (see 23.1.1).
		*/
		template<class InputIterator>
		basic_string(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
			assign( first, last );
		}

		~basic_string()
		{}

		basic_string& operator=(const basic_string& str)
		{
			return assign( str );
		}

		basic_string& operator=(const value_type* s)
		{
			return assign( s );
		}

		basic_string& operator=(value_type c)
		{
			return assign( 1, c );
		}

		// 21.3.2 iterators:
		iterator begin()
		{
			return iter_helper::build_iter( this->get_buffer(), this );
		}

		const_iterator begin() const
		{
			return iter_helper::build_const_iter( this->get_buffer(), this );
		}

		iterator end()
		{
			return begin() + this->size_;
		}
		
		const_iterator end() const
		{
			return begin() + this->size_;
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

		// 21.3.3 capacity:
		size_type length() const
		{
			return this->size_;
		}

		void resize( size_type n, value_type c )
		{
			detail::container::resize( this, n, c );
		}

		void resize(size_type n)
		{
			resize( n, value_type() );
		}

		using base_type::get_allocator;
		using base_type::max_size;
		using base_type::size;
		using base_type::capacity;
		using base_type::reserve;
		using base_type::empty;
		
		void clear()
		{
			detail::container::clear( this );
		}

		// 21.3.4 element access:
		using base_type::operator[];
		using base_type::at;
		

		// 21.3.5 modifiers:
		basic_string& operator+=(const basic_string& str)
		{
			return append( str );
		}

		basic_string& operator+=(const value_type* s)
		{
			return append( s );
		}

		basic_string& operator+=(value_type c)
		{
			return append( 1, c );
		}

		basic_string& append(const basic_string& str)
		{
			return append( str, 0, npos );
		}

		basic_string& append(const basic_string& str, size_type pos,
			size_type n)
		{
			size_type rlen = _validate_pos_and_off( pos, str, n );

			insert( end(), str.begin() + pos, str.begin() + pos + rlen );
			return *this;
		}

		basic_string& append(const value_type* s, size_type n)
		{
			insert( end(), s, s + n );
			return *this;
		}

		basic_string& append(const value_type* s)
		{
			insert( end(), s, s + traits_type::length( s ) );
			return *this;
		}

		basic_string& append(size_type n, value_type c)
		{
			insert( end(), n, c );
			return *this;
		}

		template<class InputIterator>
		basic_string& append(InputIterator first, InputIterator last)
		{
			insert( end(), first, last );
			return *this;
		}

		void push_back( const value_type& x )
		{
			detail::container::push_back( this, x );
		}
		
		//////////////////////////////////////////////////////////////////////////
		basic_string& assign(const basic_string& str)
		{
			return assign( str, 0, npos );
		}

		basic_string& assign(const basic_string& str, size_type pos,
			size_type n)
		{
			size_type rlen = _validate_pos_and_off( pos, str, n );
			return assign( str.begin() + pos, str.begin() + pos + rlen );
		}

		basic_string& assign(const value_type* s, size_type n)
		{
			GSTL_ASSERT( s != 0 );
			GSTL_ASSERT( n < npos );

			this->set_end( 0 );
			insert( 0, s, n );
			return *this;
		}
		basic_string& assign(const value_type* s)
		{
			this->set_end( 0 );
			insert( 0, s );
			return *this;
		}
		basic_string& assign(size_type n, value_type c)
		{
			this->set_end( 0 );
			insert( begin(), n, c );
			return *this;
		}

		template<class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last)
		{
			this->set_end( 0 );
			insert( begin(), first, last );
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& insert(size_type pos1, const basic_string& str)
		{
			return insert( pos1, str, 0, npos );
		}
		basic_string& insert(size_type pos1, const basic_string& str,
			size_type pos2, size_type n)
		{
			/*
			Requires pos1 <= size() and pos2 <= str.size()
			3 Throws: out_of_range if pos1 > size() or pos2 > str.size().
			4 Effects: Determines the effective length rlen of the string to insert as the smaller of n and str.size()
			- pos2. Then throws length_error if size() >= npos - rlen.
			Otherwise, the function replaces the string controlled by *this with a string of length size() +
			rlen whose first pos1 elements are a copy of the initial elements of the original string controlled by
			*this, whose next rlen elements are a copy of the elements of the string controlled by str beginning
			at position pos2, and whose remaining elements are a copy of the remaining elements of the original
			string controlled by *this.
			5 Returns: *this.
			*/
			
			_verify_pos( pos1, *this );
			size_type rlen = _validate_pos_and_off( pos2, str, n );
			if( size() > npos - rlen )
			{
				this->throw_length_error();
			}
			insert( begin() + pos1, str.begin() + pos2, str.begin() + pos2 + rlen );
			//grow( size() + rlen );
			//traits_type::copy( get_buffer() + pos1 + rlen, get_buffer() + pos1, rlen );
			//traits_type::copy( get_buffer() + pos1, str.c_str() + pos2, rlen );

			return *this;
		}
		basic_string& insert(size_type pos, const value_type* s, size_type n)
		{			
			_verify_pos( pos, *this );
			insert( begin() + pos, s, s + n );
			return *this;
		}

		basic_string& insert(size_type pos, const value_type* s)
		{
			return insert( pos, s, traits_type::length( s ) );
		}

		basic_string& insert(size_type pos, size_type n, value_type c)
		{			
			_verify_pos( pos, *this );
			insert( begin() + pos, n, c );
			return *this;
		}

		iterator insert(iterator p, value_type c)
		{			
			return insert( p, 1, c );
		}

		//Differs in return value from the C++ Standard
		iterator insert(iterator p, size_type n, value_type c)
		{//Most generic
			return _do_replace( p, p, n, boost::bind( traits_type::assign, _1, n, c ) );
		}

		template<class InputIterator>
		void insert(iterator p, InputIterator first, InputIterator last)
		{//Most generic
			replace( p, p, first, last );			
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& erase( size_type pos = 0, size_type n = npos )
		{
			if( pos > size() )
			{
				this->throw_out_of_range();
			}
			iterator first = begin() + pos;
			iterator last = first + min( n, size() - pos );
			
			erase( first, last );
			return *this;
		}
		iterator erase( iterator position )
		{
			return erase( position, position + 1 );
		}
		iterator erase( iterator first, iterator last )
		{//Most generic
			GSTL_DEBUG_RANGE( first, last );
			if( first != last )
			{
				//TODO: erase chars
				//size_type rlen = last - first;
				traits_type::copy( &*first, &*last, end() - last );
				set_end( size() - (last - first) );
			}
			return last;
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str)
		{
			return replace(pos1, n1, str, 0, npos);
		}

		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2)
		{
			size_type xlen = _validate_pos_and_off( pos1, *this, n1 );
			size_type rlen = _validate_pos_and_off( pos2, str, n2 );
			//Throws: length_error if size() - xlen >= npos - rlen
			if( size() - xlen >= npos - rlen )
			{
				this->throw_length_error();
			}
			return replace( begin() + pos1, begin() + pos1 + xlen,
				str.begin() + pos2, str.begin() + pos2 + rlen);
		}

		basic_string& replace(size_type pos, size_type n1, const value_type* s,
			size_type n2)
		{
			size_type xlen = _validate_pos_and_off( pos, *this, n1 );

			return replace( begin() + pos, begin() + pos + xlen,
				s, s + n2 );
		}
		basic_string& replace(size_type pos, size_type n1, const value_type* s)
		{
			return replace( pos, n1, s, traits_type::length( s ) );
		}

		basic_string& replace(size_type pos, size_type n1, size_type n2,
			value_type c)
		{
			size_type xlen = _validate_pos_and_off( pos, *this, n1 );

			return replace( begin() + pos, begin() + pos + xlen,
				n2, c );
		}

		basic_string& replace(iterator i1, iterator i2,
			const basic_string& str)
		{
			return replace( i1, i2,	str.begin(), str.end() );
		}

		basic_string& replace(iterator i1, iterator i2, const value_type* s,
			size_type n)
		{
			return replace( i1, i2,	s, s + n );
		}

		basic_string& replace(iterator i1, iterator i2, const value_type* s)
		{
			return replace( i1, i2,	s, traits_type::length( s ) );
		}

		basic_string& replace( iterator first, iterator last,
			size_type n, value_type c)
		{//Most generic
			_do_replace( first, last, n, boost::bind( traits_type::assign, _1, n, c ) );
			return *this;
		}
	


		template<class InputIterator>
		basic_string& replace( iterator first, iterator last,
			InputIterator first2, InputIterator last2 )
		{//Most generic
			GSTL_DEBUG_RANGE( j1, j2 );
			//Length of new str
			difference_type xlen = gstl::distance( first2, last2 );
			
			_do_replace( first, last, xlen, boost::bind( gstl::copy<InputIterator, value_type*>, first2, last2, _1 ),
				_check_overlap( first2, last2 ) );
			return *this;
		}

		template<class PutNew>
		iterator _do_replace( iterator i1, iterator i2, difference_type xlen, PutNew put_new_fn, bool overlaped = false )
		{
			GSTL_DEBUG_RANGE( i1, i2 );
			difference_type pos1 = i1 - begin();
			difference_type pos2 = i2 - begin();
			difference_type removed_size = i2 - i1;
			size_type new_size = size() + xlen - removed_size;

			pointer new_str;
			if( new_size >= this->reserved_ || overlaped )
			{//New string will be larger than current
				if( new_size >= max_size() )
				{
					this->throw_length_error();
				}
				new_str = this->alloc_.allocate( new_size + 1 );
				//Copy to the new string [begin, i1) chars 
				traits_type::move( &*new_str, this->get_buffer(), pos1 );
			}
			else
			{//Existing string is long enough to store new one
				new_str = this->buffer_;
			}
			iterator new_i1 = iter_helper::build_iter( &*( new_str + pos1 ), this );
			//iterator new_i2 = iterator( alloc_.address( new_str + pos2 ) );
			iterator res = new_i1 + xlen;
			traits_type::move( &*res, this->get_buffer() + pos2, size() - pos2 );

			put_new_fn( &*new_i1 );

			reset_ptr( &*new_str, new_size, new_size + 1 );
			return res;
		}

		size_type copy(value_type* s, size_type n, size_type pos = 0) const
		{
			GSTL_ASSERT( s != 0 );
			
			size_type rlen = _validate_pos_and_off( pos, *this, n );
			traits_type::move( s, this->get_buffer(), rlen );
			return rlen;
		}

		void swap(basic_string& str)
		{
			base_type::swap( str );
		}

		// 21.3.6 string operations:
		const value_type* c_str() const // explicit
		{
			return this->get_buffer();
		}

		const value_type* data() const
		{
			return this->get_buffer();
		}
		
		size_type find (const basic_string& str, size_type pos = 0) const
		{
			return find( str.c_str(), pos, str.size() );
		}


		size_type find (const value_type* s, size_type pos = 0) const
		{
			return find( s, pos, traits_type::length( s ) );
		}

		size_type find (const value_type* s, size_type pos, size_type n) const
		{//Most generic
			if( n == 0 )
			{//Null string always matches, if it is inside the string
				return pos < this->size_ ? pos : npos;
			}
 
 			size_type sub_size = this->size_ - pos;
 			if( pos < this->size_ && n <= sub_size )
 			{	
				//Search for sub string
 				const value_type* fres_str;
				const value_type* my_sub_str =this->get_buffer() + pos;
				//Truncate search size 
				sub_size -= n - 1;
 				while( ( fres_str = traits_type::find( my_sub_str, sub_size, *s ) ) != 0 )
				{
					if( traits_type::compare(fres_str, s, n) == 0 )
					{
						//Match found
 						return fres_str - this->get_buffer();	
					}
					sub_size -= fres_str - my_sub_str + 1;
					my_sub_str = fres_str + 1;
				}
 			}

			//No match
			return npos;	
		}

		size_type find (value_type c, size_type pos = 0) const
		{			
			return find( &c, pos, 1);
		}

		size_type rfind(const basic_string& str, size_type pos = npos) const
		{
			return rfind( str.c_str(), pos, str.size() );
		}

		size_type rfind(const value_type* s, size_type pos, size_type n) const
		{//Most generic
 			if( n == 0 )
			{//Null string always matches, if it is inside the string
				return pos < this->size_ ? pos : npos;
			}

 			if( n <= this->size_ )
 			{	
				//Search for sub string in the reverse direction
				const value_type* fres_str = this->get_buffer() + (gstl::min)( pos, this->size_ - n );
				for( ;; --fres_str )
				{
 					if( traits_type::eq( *fres_str, *s )
 						&& traits_type::compare( fres_str, s, n ) == 0 )
					{
						//Match found
						return fres_str - this->get_buffer();
					}
					if( fres_str == this->get_buffer() )
					{
						//Beginning of the string reached
						break;
					}
				}
 			}

			//No match
			return npos;	
		}

		size_type rfind(const value_type* s, size_type pos = npos) const
		{
			return rfind( s, pos, traits_type::length( s ) );
		}

		size_type rfind(value_type c, size_type pos = npos) const
		{
			return rfind( &c, pos, 1);
		}

		size_type find_first_of(const basic_string& str,
			size_type pos = 0) const
		{
			return find_first_of( str.c_str(), pos, str.size() );
		}

		size_type find_first_of(const value_type* s,
			size_type pos, size_type n) const
		{
 			if( n > 0 && pos < this->size_ )
 			{	
				//Search for the first occurrence of any character from s in *this
 				const value_type *const my_end = this->get_buffer() + this->size_;
 				for( const value_type *curr_pos = this->get_buffer() + pos; curr_pos < my_end; ++curr_pos )
				{
					if( traits_type::find( s, n, *curr_pos ) != 0 )
					{
						//Match found
						return curr_pos - this->get_buffer();
					}
				}
 			}

			//No match
			return npos;
		}

		size_type find_first_of(const value_type* s, size_type pos = 0) const
		{
			return find_first_of(  s, pos, traits_type::length( s ) );
		}

		size_type find_first_of(value_type c, size_type pos = 0) const
		{
			return find_first_of( &c, pos, 1);
		}

		size_type find_last_of (const basic_string& str,
			size_type pos = npos) const
		{
			return find_last_of( str.c_str(), pos, str.size() );
		}

		size_type find_last_of (const value_type* s,
			size_type pos, size_type n) const
		{
			if( n > 0 && this->size_ )
			{
				//Search for the last occurrence of any character from s in *this
				const value_type *curr_pos = this->get_buffer() + (gstl::min)( pos, this->size_ - 1 );
				for( ;; --curr_pos )
				{
					if( traits_type::find( s, n, *curr_pos ) != 0 )
					{
						//Match found
						return curr_pos - this->get_buffer();
					}
 					if( curr_pos == this->get_buffer() )
					{
						//Beginning of the string reached
						break;
					}
				}
			}

			//no match
			return npos;
		}

		size_type find_last_of (const value_type* s, size_type pos = npos) const
		{
			return find_last_of( s, pos, traits_type::length( s ) );
		}

		size_type find_last_of (value_type c, size_type pos = npos) const
		{
			return find_last_of( &c, pos, 1);
		}

		size_type find_first_not_of(const basic_string& str,
			size_type pos = 0) const
		{
			return find_first_not_of( str.c_str(), pos, str.size() );
		}

		size_type find_first_not_of(const value_type* s, size_type pos,
			size_type n) const
		{
			if( n > 0 && pos < this->size_ )
			{
				//Search for the first occurrence of any character 
				//in *this not from s
 				const value_type *const my_end = this->get_buffer() + this->size_;
 				for( const value_type *curr_pos = this->get_buffer() + pos; curr_pos < my_end; ++curr_pos )
				{
					if( traits_type::find(s, n, *curr_pos) == 0 )
					{
						//Match found
						return curr_pos - this->get_buffer();
					}
				}
 			}

			//No match
			return npos;
		}

		size_type find_first_not_of(const value_type* s, size_type pos = 0) const
		{
			return find_first_not_of( s, pos, traits_type::length( s ) );
		}

		size_type find_first_not_of(value_type c, size_type pos = 0) const
		{
			return find_first_not_of( &c, pos, 1);
		}

		size_type find_last_not_of (const basic_string& str,
			size_type pos = npos) const
		{
			return find_last_not_of( str.c_str(), pos, str.size() );
		}

		size_type find_last_not_of (const value_type* s, size_type pos,
			size_type n) const
		{
			if( n > 0 && this->size_ )
			{
				//Search for the last occurrence of any character 
				//in *this not from s
				const value_type *curr_pos = this->get_buffer() + (gstl::min)( pos, this->size_ - 1 );
 				for( ;; --curr_pos )
				{
					if( traits_type::find( s, n, *curr_pos ) == 0 )
					{
						//Match found
						return curr_pos - this->get_buffer();
					}
 					if( curr_pos == this->get_buffer() )
					{
						//Beginning of the string reached
						break;
					}
				}
			}

			//No match
			return npos;
		}

		size_type find_last_not_of (const value_type* s,
			size_type pos = npos) const
		{
			return find_last_not_of( s, pos, traits_type::length( s ) );
		}


		size_type find_last_not_of (value_type c, size_type pos = npos) const
		{
			return find_last_not_of( &c, pos, 1);
		}

		basic_string substr(size_type pos = 0, size_type n = npos) const
		{
			return basic_string( *this, pos, n, this->alloc_ ) ;
		}

		int compare(const basic_string& str) const
		{
			return compare( 0, size(), str, 0, str.size() );
		}

		int compare(size_type pos1, size_type n1,
			const basic_string& str) const
		{
			return compare( pos1, n1, str, 0, str.size() );
		}

		int compare(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2) const
		{
			_verify_pos( pos2, str );
			return compare( pos1, n1, str.c_str() + pos2, n2 );
		}

		int compare(const value_type* s) const
		{
			return compare( 0, size(), s );
		}

		int compare(size_type pos1, size_type n1,
			const value_type* s) const
		{
			return compare( pos1, n1, s, traits_type::length( s ) );
		}

		int compare(size_type pos1, size_type n1,
			const value_type* s, size_type n2) const
		{
			//Most generic
			size_type len1 = _validate_pos_and_off( pos1, *this, n1 );
			size_type rlen = (gstl::min)( len1, n2 );
			int cmp_res = traits_type::compare(data() + pos1, s, rlen);
			if( !cmp_res )
			{
				//size() < str.size() | < 0
				//size() == str.size()| 0
				//size() > str.size() | > 0
				if( len1 < n2 )
				{
					return -1;
				}
				else if( len1 > n2 )
				{
					return 1;
				}
				return 0;
			}
			return cmp_res;
		}

	private:
		void _verify_pos( size_type pos, const basic_string& str ) const
		{
			if( pos > str.size() )
			{
				this->throw_out_of_range();
			}
		}
		size_type _validate_pos_and_off( size_type pos, const basic_string& str, size_type n ) const
		{
			_verify_pos( pos, str );
			return min( n, str.size() - pos );
		}

		template<class InputIterator>
		bool _check_overlap( InputIterator first, InputIterator last )
		{
			return _check_overlap( first, last, GSTL_ITER_CAT( InputIterator ) );
		}


		template<class InputIterator>
		bool _check_overlap( InputIterator first, InputIterator last, input_iterator_tag,
			typename boost::disable_if< boost::is_pointer<InputIterator> >::type* = 0 )
		{
			return false;
		}

		template<class RanIterator>
		bool _check_overlap( RanIterator first, RanIterator last, random_access_iterator_tag,
			typename boost::disable_if< 
			typename boost::mpl::or_<boost::is_pointer<RanIterator>, 
			boost::is_convertible<RanIterator, const_iterator>, boost::mpl::true_
			>::type >::type* = 0 )
		{
			return false;
		}

		bool _check_overlap( iterator first, iterator last, random_access_iterator_tag tag )
		{
			return check_overlap_iter( first, last, tag );
		}

		bool _check_overlap( const_iterator first, const_iterator last, random_access_iterator_tag tag )
		{
			return check_overlap_iter( first, last, tag );
		}

		bool check_overlap_iter( const_iterator first, const_iterator last, random_access_iterator_tag tag )
		{
			bool result = false;
			if( first != last )
			{
				//Trick to prevent dereferencing of the last iterator
				difference_type len = last - first;
				result = _check_overlap( &*first, &*first + len, tag ) != 0;
			}
			return result;
		}

		bool _check_overlap( const void* first, const void* last, random_access_iterator_tag )
		{
			return _inside_string( static_cast<const value_type*>( first) )
				|| _inside_string( static_cast<const value_type*>( last ) );
		}

		bool _inside_string( const value_type* p )
		{
			difference_type d = p - this->get_buffer();
			return d >= 0 && d < static_cast<difference_type>( this->size_ );
		}
	};


	template<class value_type, class traits, class allocator_type>
	basic_string<value_type, traits, allocator_type>
		operator+(const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return basic_string<value_type,traits,allocator_type>( lhs ) += rhs;
	}

	template<class value_type, class traits, class allocator_type>
	basic_string<value_type,traits,allocator_type>
		operator+(const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return basic_string<value_type,traits,allocator_type>( lhs ) += rhs;
	}

	template<class value_type, class traits, class allocator_type>
	basic_string<value_type,traits,allocator_type>
		operator+(value_type lhs, const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return basic_string<value_type,traits,allocator_type>( lhs ) += rhs;
	}

	template<class value_type, class traits, class allocator_type>
	basic_string<value_type,traits,allocator_type>
		operator+(const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return basic_string<value_type,traits,allocator_type>( lhs ) += rhs;
	}

	template<class value_type, class traits, class allocator_type>
	basic_string<value_type,traits,allocator_type>
		operator+(const basic_string<value_type,traits,allocator_type>& lhs, value_type rhs)
	{
		return basic_string<value_type,traits,allocator_type>( lhs ) += rhs;
	}
	
	template<class value_type, class traits, class allocator_type >
	bool operator==(const basic_string<value_type, traits, allocator_type>& lhs,
		const basic_string<value_type, traits, allocator_type>& rhs)
	{
		return lhs.compare( rhs ) == 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator==(const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return rhs.compare( lhs ) == 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator==(const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return lhs.compare( rhs ) == 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	bool operator!=(const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class traits, class allocator_type>
	bool operator!=(const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	template<class value_type, class traits, class allocator_type>
	bool operator!=(const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return rel_ops::operator !=( lhs, rhs );
	}

	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	bool operator< (const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return lhs.compare( rhs ) < 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator< (const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return lhs.compare( rhs ) < 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator< (const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{//Params are in back order
		return rhs.compare( lhs ) > 0;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	bool operator> (const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return lhs.compare( rhs ) > 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator> (const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return lhs.compare( rhs ) > 0;
	}


	template<class value_type, class traits, class allocator_type>
	bool operator> (const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{//Params are in back order
		return rhs.compare( lhs ) < 0;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	bool operator<=(const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return lhs.compare( rhs ) <= 0;
	}


	template<class value_type, class traits, class allocator_type>
	bool operator<=(const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return lhs.compare( rhs ) <= 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator<=(const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{//Params are in back order
		return rhs.compare( lhs ) >= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	bool operator>=(const basic_string<value_type,traits,allocator_type>& lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{
		return lhs.compare( rhs ) >= 0;
	}


	template<class value_type, class traits, class allocator_type>
	bool operator>=(const basic_string<value_type,traits,allocator_type>& lhs,
		const value_type* rhs)
	{
		return lhs.compare( rhs ) >= 0;
	}

	template<class value_type, class traits, class allocator_type>
	bool operator>=(const value_type* lhs,
		const basic_string<value_type,traits,allocator_type>& rhs)
	{//Params are in back order
		return rhs.compare( lhs ) <= 0;
	}

	

	//////////////////////////////////////////////////////////////////////////
	// 21.3.7.8:
	template<class value_type, class traits, class allocator_type>
	void swap(basic_string<value_type,traits,allocator_type>& lhs,
		basic_string<value_type,traits,allocator_type>& rhs)
	{
		lhs.swap( rhs );
	}

	template<class value_type, class traits, class stream_traits, class allocator_type>
	std::basic_istream<value_type, stream_traits>&
		operator>>( std::basic_istream<value_type, stream_traits>& is,
		basic_string<value_type, traits, allocator_type>& str )
	{
		std::basic_string<value_type, traits, allocator_type> tmp;
		is >> tmp;
		str = tmp.c_str();
		return is;
	}
	

	template<class value_type, class traits, class stream_traits, class allocator_type>
	std::basic_ostream<value_type, stream_traits>&
		operator<<( std::basic_ostream<value_type, stream_traits>& os,
		const basic_string<value_type,traits,allocator_type>& str )
	{
		return os << str.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	template<class value_type, class traits, class allocator_type>
	std::basic_istream<value_type,traits>&
		getline( std::basic_istream<value_type,traits>& is,
		basic_string<value_type,traits,allocator_type>& str,
		value_type delim );

	template<class value_type, class traits, class allocator_type>
	std::basic_istream<value_type,traits>&
		getline( std::basic_istream<value_type,traits>& is,
		basic_string<value_type,traits,allocator_type>& str );
	

	typedef basic_string<char, char_traits<char>, allocator<char> > string;
	typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;
}


