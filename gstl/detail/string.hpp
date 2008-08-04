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

#ifndef GSTL_STRING_HEADER
#define GSTL_STRING_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <iosfwd>
#include <gstl/detail/allocator.hpp>
#include <gstl/detail/utility.hpp>
#include <gstl/detail/char_traits.hpp>

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
				class Alloc,
				class PtrTraits
			>
		class string_base
		{
		public:
			enum{ min_buff_size = 4 };

			typedef CharT										char_type;
			typedef Traits										char_traits_type;
			typedef typename Alloc::rebind<char_type>::other	allocator_type;


			typedef typename allocator_type::value_type			value_type;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;

			typedef typename allocator_type::size_type			size_type;
			typedef typename allocator_type::difference_type	difference_type;

			typedef PtrTraits									ptr_traits;

			
			typedef typename ptr_traits::pointer				impl_pointer;
			typedef typename ptr_traits::const_pointer			impl_const_pointer;
			typedef typename ptr_traits::reference				impl_reference;
			typedef typename ptr_traits::const_reference		impl_const_reference;

			impl_pointer	m_str;
			size_type		m_size;
			size_type		m_reserved;
			allocator_type	m_alloc;

			
			string_base( const allocator_type& alloc = allocator_type() )
				:m_str( ptr_traits::null_ptr ),
				m_size(0),
				m_reserved(0),
				m_alloc( alloc )
			{
			}

			void set_end( size_type n )
			{
				m_size = n;
				m_str[m_size] = char_type();
			}
		};

	}

	template
	<
		class CharT,
		class Traits = char_traits<CharT>,
		class Alloc = allocator<CharT>,
		class PtrTraits = typename Alloc::pointer_traits_type
	>
	class basic_string
		:private detail::string_base<CharT, Traits, Alloc, PtrTraits>
	{
		typedef detail::string_base<CharT, Traits, Alloc, PtrTraits> base_type;
	public:
		//////////////////////////////////////////////////////////////////////////
		// types:
		typedef typename base_type::char_type			char_type;
		typedef typename base_type::char_traits_type	char_traits_type;
		typedef typename base_type::allocator_type		allocator_type;


		typedef typename base_type::ptr_traits		ptr_traits;

		typedef typename base_type::value_type		value_type;
		typedef typename base_type::pointer			pointer;
		typedef typename base_type::const_pointer	const_pointer;
		typedef typename base_type::reference		reference;
		typedef typename base_type::const_reference	const_reference;

		typedef typename base_type::size_type		size_type;
		typedef typename base_type::difference_type	difference_type;


		//////////////////////////////////////////////////////////////////////////
		typedef char* iterator; //See 23.1
		typedef const char* const_iterator; // See 23.1
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		static const size_type npos = static_cast<size_type>( -1 );
		
		// 21.3.1 construct/copy/destroy:
		/**
		@brief Constructs an object of class basic_string. The postconditions of this function are indicated
			in Table 38:
			data() a non-null char_type* that is copyable and can have 0 added to it
			size() 0
			capacity() an unspecified value
		*/
		explicit basic_string( const allocator_type& alloc = allocator_type() )
			:base_type( alloc )
		{
			reserve( min_buff_size );
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

		basic_string(const char_type* s,
			size_type n, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
		}

		basic_string(const char_type* s, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
		}

		basic_string(size_type n, char_type c, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
		}

		/*		
		Effects: If InputIterator is an integral type, equivalent to
		basic_string(static_cast<size_type>(begin), static_cast<value_type>(end))
		Otherwise constructs a string from the values in the range [begin, end), as indicated in the Sequence
		Requirements table (see 23.1.1).
		*/
		template<class InputIterator>
		basic_string(InputIterator begin, InputIterator end,
			const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
		}

		~basic_string()
		{
			m_alloc.deallocate( m_str, m_reserved );
		}

		basic_string& operator=(const basic_string& str)
		{
			return *this;
		}
		basic_string& operator=(const char_type* s)
		{
			return *this;
		}
		basic_string& operator=(char_type c)
		{
			return *this;
		}
		// 21.3.2 iterators:
		iterator begin()
		{
			return my_str();
		}

		const_iterator begin() const
		{
			return my_str();
		}

		iterator end()
		{
			return begin() + m_size;
		}
		
		const_iterator end() const
		{
			return begin() + m_size;
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
		size_type size() const
		{
			return m_size;
		}

		size_type length() const
		{
			return m_size;
		}
		
		size_type max_size() const
		{
			return m_alloc.max_size();
		}

		void resize( size_type n, char_type c )
		{
			if( n > m_size )
			{
				//TODO: grow
				reserve( n );
				char_traits_type::assign( my_str() + m_size, n - m_size, c );
			}
			set_end( n );

		}

		void resize(size_type n)
		{
			resize( n, char_type() );
		}


		size_type capacity() const
		{
			return m_reserved;
		}


		void reserve( size_type n = 0 )
		{
			if( n > m_reserved )
			{
				n = floor( n, min_buff_size );
				impl_pointer new_str = m_alloc.allocate( n );
				impl_pointer old_str = m_str;
				if( !ptr_traits::is_null( m_str ) )
				{
					char_traits_type::move( m_alloc.address( new_str ), my_str(), m_size );
				}
				m_str = new_str;
				m_alloc.deallocate( old_str, m_reserved );
				m_reserved = n;
			}
			set_end( m_size );
		}

		void clear()
		{
			erase( begin(), end() );
		}
		bool empty() const
		{
			return begin() == end();
		}
		// 21.3.4 element access:
		const_reference operator[](size_type pos) const
		{
			return my_str()[ pos ];
		}
		reference operator[](size_type pos)
		{
			return my_str()[ pos ];
		}
		const_reference at(size_type n) const
		{
			if( n > size() )
			{
				throw_out_of_range()
			}
			return my_str()[n];
		}
		reference at(size_type n)
		{
			if( n > size() )
			{
				throw_out_of_range();
			}
			return my_str()[n];
		}
		// 21.3.5 modifiers:
		basic_string& operator+=(const basic_string& str)
		{
			return *this;
		}
		basic_string& operator+=(const char_type* s)
		{
			return *this;
		}
		basic_string& operator+=(char_type c)
		{
			return *this;
		}
		basic_string& append(const basic_string& str)
		{
			return *this;
		}
		basic_string& append(const basic_string& str, size_type pos,
			size_type n)
		{
			return *this;
		}
		basic_string& append(const char_type* s, size_type n)
		{
			return *this;
		}
		basic_string& append(const char_type* s)
		{
			return *this;
		}
		basic_string& append(size_type n, char_type c)
		{
			return *this;
		}
		template<class InputIterator>
		basic_string& append(InputIterator first, InputIterator last)
		{
			return *this;
		}
		void push_back(char_type c)
		{
			insert( end(), c );
		}
		
		//////////////////////////////////////////////////////////////////////////
		basic_string& assign(const basic_string& str)
		{
			return assign( str, 0, npos );
		}

		basic_string& assign(const basic_string& str, size_type pos,
			size_type n)
		{
			if( pos > str.size() )
			{
				throw_out_of_range();

			}
			size_type rlen = min( n, str.size() - pos );
			return assign( str.begin() + pos, str.begin() + pos + rlen );
		}

		basic_string& assign(const char_type* s, size_type n)
		{
			GSTL_ASSERT( s != 0 );
			GSTL_ASSERT( n < npos );

			return assign( s, s + n );
		}
		basic_string& assign(const char_type* s)
		{
			return *this;
		}
		basic_string& assign(size_type n, char_type c)
		{
			return *this;
		}

		template<class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last)
		{
// 			GSTL_ASSERT( first <= last );
// 
// 			size_type rlen = last - first;
// 
// 			grow( rlen );
// 			char_traits_type::copy( my_str(), &*first, rlen );
// 			set_end( rlen );

			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& insert(size_type pos1, const basic_string& str)
		{
			return *this;
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
			if( pos1 > size() || pos2 > str.size() )
			{
				throw_out_of_range();
			}
			size_type rlen = std::min( n, str.size() - pos2 );
			if( size() > npos - rlen )
			{
				throw_length_error();
			}
			grow( size() + rlen );
			char_traits_type::copy( my_str() + pos1 + rlen, my_str() + pos1, rlen );
			char_traits_type::copy( my_str() + pos1, str.c_str() + pos2, rlen );

			return *this;
		}
		basic_string& insert(size_type pos, const char_type* s, size_type n)
		{
			return *this;
		}
		basic_string& insert(size_type pos, const char_type* s)
		{
			return *this;
		}
		basic_string& insert(size_type pos, size_type n, char_type c)
		{
			return *this;
		}
		iterator insert(iterator p, char_type c)
		{
			return p;
		}
		void insert(iterator p, size_type n, char_type c)
		{//Most generic
			p;n;c;
		}

		template<class InputIterator>
		void insert(iterator p, InputIterator first, InputIterator last)
		{//Most generic

			//Length of new str
			difference_type xlen = std::distance( first, last );
			//Insert position
			size_type	pos = p - begin();
			grow( size() + xlen );
			char_traits_type::copy( my_str() + pos + xlen, my_str() + pos, xlen );

		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& erase( size_type pos = 0, size_type n = npos )
		{
			if( pos > size() )
			{
				throw_out_of_range();
			}
			iterator first = begin() + pos;
			iterator last = first + std::min( n, size() - pos );
			
			erase( first, last );
			return *this;
		}
		iterator erase( iterator position )
		{
			return erase( position, position + 1 );
		}
		iterator erase( iterator first, iterator last )
		{//Most generic
			GSTL_DEBUG_RANGE( first, last )
			if( first != last )
			{
				//TODO: erase chars
			}
			return last;
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str)
		{
			return *this;
		}
		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2)
		{
			return *this;
		}
		basic_string& replace(size_type pos, size_type n1, const char_type* s,
			size_type n2)
		{
			return *this;
		}
		basic_string& replace(size_type pos, size_type n1, const char_type* s)
		{
			return *this;
		}
		basic_string& replace(size_type pos, size_type n1, size_type n2,
			char_type c)
		{
			return *this;
		}
		basic_string& replace(iterator i1, iterator i2,
			const basic_string& str)
		{
			return *this;
		}
		basic_string& replace(iterator i1, iterator i2, const char_type* s,
			size_type n)
		{
			return *this;
		}
		basic_string& replace(iterator i1, iterator i2, const char_type* s)
		{
			return *this;
		}
		basic_string& replace(iterator i1, iterator i2,
			size_type n, char_type c)
		{//Most generic
			return *this;
		}
		template<class InputIterator>
		basic_string& replace(iterator i1, iterator i2,
			InputIterator j1, InputIterator j2)
		{//Most generic

		}
		size_type copy(char_type* s, size_type n, size_type pos = 0) const
		{
			return n;
		}
		void swap(basic_string& str)
		{

		}

		// 21.3.6 string operations:
		const char_type* c_str() const // explicit
		{
			return my_str();
		}

		const char_type* data() const
		{
			return my_str();
		}

		allocator_type get_allocator() const
		{
			return m_alloc;
		}
		
		size_type find (const basic_string& str, size_type pos = 0) const
		{
			return npos;
		}
		size_type find (const char_type* s, size_type pos, size_type n) const
		{
			return npos;
		}
		size_type find (const char_type* s, size_type pos = 0) const
		{
			return npos;
		}
		size_type find (char_type c, size_type pos = 0) const
		{
			return npos;
		}
		size_type rfind(const basic_string& str, size_type pos = npos) const
		{
			return npos;
		}
		size_type rfind(const char_type* s, size_type pos, size_type n) const
		{
			return npos;
		}
		size_type rfind(const char_type* s, size_type pos = npos) const
		{
			return npos;
		}
		size_type rfind(char_type c, size_type pos = npos) const
		{
			return npos;
		}
		size_type find_first_of(const basic_string& str,
			size_type pos = 0) const
		{
			return npos;
		}
		size_type find_first_of(const char_type* s,
			size_type pos, size_type n) const
		{
			return npos;
		}
		size_type find_first_of(const char_type* s, size_type pos = 0) const
		{
			return npos;
		}
		size_type find_first_of(char_type c, size_type pos = 0) const
		{
			return npos;
		}
		size_type find_last_of (const basic_string& str,
			size_type pos = npos) const
		{
			return npos;
		}
		size_type find_last_of (const char_type* s,
			size_type pos, size_type n) const
		{
			return npos;
		}
		size_type find_last_of (const char_type* s, size_type pos = npos) const
		{
			return npos;
		}
		size_type find_last_of (char_type c, size_type pos = npos) const
		{
			return npos;
		}
		size_type find_first_not_of(const basic_string& str,
			size_type pos = 0) const
		{
			return npos;
		}
		size_type find_first_not_of(const char_type* s, size_type pos,
			size_type n) const
		{
			return npos;
		}
		size_type find_first_not_of(const char_type* s, size_type pos = 0) const
		{
			return npos;
		}
		size_type find_first_not_of(char_type c, size_type pos = 0) const
		{
			return npos;
		}
		size_type find_last_not_of (const basic_string& str,
			size_type pos = npos) const
		{
			return npos;
		}
		size_type find_last_not_of (const char_type* s, size_type pos,
			size_type n) const
		{
			return npos;
		}
		size_type find_last_not_of (const char_type* s,
			size_type pos = npos) const
		{
			return npos;
		}
		size_type find_last_not_of (char_type c, size_type pos = npos) const
		{
			return npos;
		}
		basic_string substr(size_type pos = 0, size_type n = npos) const
		{
			return *this;
		}
		int compare(const basic_string& str) const
		{
			return -1;
		}
		int compare(size_type pos1, size_type n1,
			const basic_string& str) const
		{
			return -1;
		}
		int compare(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2) const
		{
			return -1;
		}
		int compare(const char_type* s) const
		{
			return -1;
		}
		int compare(size_type pos1, size_type n1,
			const char_type* s) const
		{
			return -1;
		}
		int compare(size_type pos1, size_type n1,
			const char_type* s, size_type n2) const
		{
			return -1;
		}

	private:
		void grow( size_type n )
		{
			reserve( n + 1 );
		}

		char_type* my_str()
		{
			return m_alloc.address( m_str );
		}

		const char_type* my_str() const
		{
			return m_alloc.address( m_str );
		}

		void throw_out_of_range()
		{
			throw std::out_of_range( "invalid position" );
		}

		void throw_length_error()
		{
			throw std::length_error( "invalid length" );
		}
	};


	template<class char_type, class traits, class allocator_type, class pointer_traits>
	basic_string<char_type, traits, allocator_type, pointer_traits>
		operator+(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	basic_string<char_type,traits,allocator_type, pointer_traits>
		operator+(const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	basic_string<char_type,traits,allocator_type, pointer_traits>
		operator+(char_type lhs, const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	basic_string<char_type,traits,allocator_type, pointer_traits>
		operator+(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	basic_string<char_type,traits,allocator_type, pointer_traits>
		operator+(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs, char_type rhs);
	
	template<class char_type, class traits, class allocator_type, class pointer_traits >
	bool operator==(const basic_string<char_type, traits, allocator_type, pointer_traits>& lhs,
		const basic_string<char_type, traits, allocator_type, pointer_traits>& rhs)
	{
		return lhs.compare( rhs ) == 0;
	}

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator==(const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs)
	{
		return rhs.compare( lhs ) == 0;
	}
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator==(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs)
	{
		return lhs.compare( rhs ) == 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator!=(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator!=(const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator!=(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs);

	//////////////////////////////////////////////////////////////////////////
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator< (const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator< (const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator< (const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	//////////////////////////////////////////////////////////////////////////
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator> (const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator> (const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator> (const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	//////////////////////////////////////////////////////////////////////////
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator>=(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	bool operator>=(const basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		const char_type* rhs);
	template<class char_type, class traits, class allocator_type, class pointer_traits>

	bool operator>=(const char_type* lhs,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	//////////////////////////////////////////////////////////////////////////
	// 21.3.7.8:
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	void swap(basic_string<char_type,traits,allocator_type, pointer_traits>& lhs,
		basic_string<char_type,traits,allocator_type, pointer_traits>& rhs);

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	std::basic_istream<char_type,traits>&
		operator>>( std::basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type, pointer_traits>& str );
	

	template<class char_type, class traits, class stream_traits, class allocator_type, class pointer_traits>
	std::basic_ostream<char_type, stream_traits>&
		operator<<( std::basic_ostream<char_type, stream_traits>& os,
		const basic_string<char_type,traits,allocator_type, pointer_traits>& str )
	{
		return os << str.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	template<class char_type, class traits, class allocator_type, class pointer_traits>
	std::basic_istream<char_type,traits>&
		getline( std::basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type, pointer_traits>& str,
		char_type delim );

	template<class char_type, class traits, class allocator_type, class pointer_traits>
	std::basic_istream<char_type,traits>&
		getline( std::basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type, pointer_traits>& str );
	

	typedef basic_string<char, char_traits<char>, allocator<char> > string;
	typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;
}
// namespace std
// {
// 
// 	template<class char_type, class traits, class stream_traits, class allocator_type, class pointer_traits>
// 	std::basic_ostream<char_type, stream_traits>&
// 		operator<<( std::basic_ostream<char_type, stream_traits>& os,
// 		const gstl::basic_string<char_type,traits,allocator_type, pointer_traits>& str )
// 	{
// 		return os << str.c_str();
// 	}
// }

#endif //GSTL_STRING_HEADER