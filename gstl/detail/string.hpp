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

#include <gstl/detail/allocator.hpp>
#include <gstl/detail/char_traits.hpp>

namespace gstl
{
	namespace detail
	{
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

		template<class T>
		const T smaller( const T v1, const T v2 )
		{
			return v1 < v2 ? v1 : v2;
		}

		template<class T1, class T2>
		const T1 floor( const T1 val, const T2 base )
		{
			return (val/base) * base + ( (val % base) ? base : 0 );
		}
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
			data() a non-null pointer that is copyable and can have 0 added to it
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

		basic_string(const_pointer s,
			size_type n, const allocator_type& alloc = allocator_type())
			:base_type( alloc )
		{
		}

		basic_string(const_pointer s, const allocator_type& alloc = allocator_type())
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

		basic_string& operator=(const basic_string& str);
		basic_string& operator=(const_pointer s);
		basic_string& operator=(char_type c);
		// 21.3.2 iterators:
		iterator begin()
		{
			return m_alloc.address( m_str );
		}

		const_iterator begin() const
		{
			return m_alloc.address( m_str );
		}

		iterator end();
		const_iterator end() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		// 21.3.3 capacity:
		size_type size() const
		{
			return m_size;
		}

		size_type length() const
		{
			return m_size;
		}
		
		size_type max_size() const;

		void resize( size_type n, char_type c )
		{
			if( n > m_size )
			{
				//TODO: grow
				reserve( n );
				char_traits_type::assign( m_alloc.address( m_str ) + m_size, n - m_size, c );
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
					char_traits_type::move( m_alloc.address( new_str ), m_alloc.address( m_str ), m_size );
				}
				m_str = new_str;
				m_alloc.deallocate( old_str, m_reserved );
				m_reserved = n;
			}
			set_end( m_size );
		}

		void clear();
		bool empty() const;
		// 21.3.4 element access:
		const_reference operator[](size_type pos) const;
		reference operator[](size_type pos);
		const_reference at(size_type n) const;
		reference at(size_type n);
		// 21.3.5 modifiers:
		basic_string& operator+=(const basic_string& str);
		basic_string& operator+=(const_pointer s);
		basic_string& operator+=(char_type c);
		basic_string& append(const basic_string& str);
		basic_string& append(const basic_string& str, size_type pos,
			size_type n);
		basic_string& append(const_pointer s, size_type n);
		basic_string& append(const_pointer s);
		basic_string& append(size_type n, char_type c);
		template<class InputIterator>
		basic_string& append(InputIterator first, InputIterator last);
		void push_back(char_type c);
		
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
				throw std::out_of_range( "invalid position" );
			}
			size_type rlen = smaller( n, str.size() - pos );
			return assign( str.begin() + pos, str.begin() + pos + rlen );
		}

		basic_string& assign(const_pointer s, size_type n)
		{
			GSTL_ASSERT( s != 0 );
			GSTL_ASSERT( n < npos );

			return assign( s, s + n );
		}
		basic_string& assign(const_pointer s);
		basic_string& assign(size_type n, char_type c)
		{

		}

		template<class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last)
		{
			GSTL_ASSERT( first <= last );

			size_type rlen = last - first;

			grow( rlen );
			char_traits_type::copy( &*m_str, &*first, rlen );
			set_end( rlen );

			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		basic_string& insert(size_type pos1, const basic_string& str);
		basic_string& insert(size_type pos1, const basic_string& str,
			size_type pos2, size_type n);
		basic_string& insert(size_type pos, const_pointer s, size_type n);
		basic_string& insert(size_type pos, const_pointer s);
		basic_string& insert(size_type pos, size_type n, char_type c);
		iterator insert(iterator p, char_type c);
		void insert(iterator p, size_type n, char_type c);
		template<class InputIterator>
		void insert(iterator p, InputIterator first, InputIterator last);

		//////////////////////////////////////////////////////////////////////////
		basic_string& erase(size_type pos = 0, size_type n = npos);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//////////////////////////////////////////////////////////////////////////
		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str);
		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2);
		basic_string& replace(size_type pos, size_type n1, const_pointer s,
			size_type n2);
		basic_string& replace(size_type pos, size_type n1, const_pointer s);
		basic_string& replace(size_type pos, size_type n1, size_type n2,
			char_type c);
		basic_string& replace(iterator i1, iterator i2,
			const basic_string& str);
		basic_string& replace(iterator i1, iterator i2, const_pointer s,
			size_type n);
		basic_string& replace(iterator i1, iterator i2, const_pointer s);
		basic_string& replace(iterator i1, iterator i2,
			size_type n, char_type c);
		template<class InputIterator>
		basic_string& replace(iterator i1, iterator i2,
			InputIterator j1, InputIterator j2);
		size_type copy(pointer s, size_type n, size_type pos = 0) const;
		void swap(basic_string& str);

		// 21.3.6 string operations:
		const_pointer c_str() const // explicit
		{
			return data();
		}

		const_pointer data() const
		{
			return m_alloc.address( m_str );
		}

		allocator_type get_allocator() const
		{
			return m_alloc;
		}
		
		size_type find (const basic_string& str, size_type pos = 0) const;
		size_type find (const_pointer s, size_type pos, size_type n) const;
		size_type find (const_pointer s, size_type pos = 0) const;
		size_type find (char_type c, size_type pos = 0) const;
		size_type rfind(const basic_string& str, size_type pos = npos) const;
		size_type rfind(const_pointer s, size_type pos, size_type n) const;
		size_type rfind(const_pointer s, size_type pos = npos) const;
		size_type rfind(char_type c, size_type pos = npos) const;
		size_type find_first_of(const basic_string& str,
			size_type pos = 0) const;
		size_type find_first_of(const_pointer s,
			size_type pos, size_type n) const;
		size_type find_first_of(const_pointer s, size_type pos = 0) const;
		size_type find_first_of(char_type c, size_type pos = 0) const;
		size_type find_last_of (const basic_string& str,
			size_type pos = npos) const;
		size_type find_last_of (const_pointer s,
			size_type pos, size_type n) const;
		size_type find_last_of (const_pointer s, size_type pos = npos) const;
		size_type find_last_of (char_type c, size_type pos = npos) const;
		size_type find_first_not_of(const basic_string& str,
			size_type pos = 0) const;
		size_type find_first_not_of(const_pointer s, size_type pos,
			size_type n) const;
		size_type find_first_not_of(const_pointer s, size_type pos = 0) const;
		size_type find_first_not_of(char_type c, size_type pos = 0) const;
		size_type find_last_not_of (const basic_string& str,
			size_type pos = npos) const;
		size_type find_last_not_of (const_pointer s, size_type pos,
			size_type n) const;
		size_type find_last_not_of (const_pointer s,
			size_type pos = npos) const;
		size_type find_last_not_of (char_type c, size_type pos = npos) const;
		basic_string substr(size_type pos = 0, size_type n = npos) const;
		int compare(const basic_string& str) const;
		int compare(size_type pos1, size_type n1,
			const basic_string& str) const;
		int compare(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2) const;
		int compare(const_pointer s) const;
		int compare(size_type pos1, size_type n1,
			const_pointer s) const;
		int compare(size_type pos1, size_type n1,
			const_pointer s, size_type n2) const;

	private:
		void grow( size_type n )
		{
			reserve( n + 1 );
		}
	};
/*

	template<class char_type, class traits, class allocator_type>
	basic_string<char_type,traits,allocator_type>
		operator+(const basic_string<char_type,traits,allocator_type>& lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	basic_string<char_type,traits,allocator_type>
		operator+(const_pointer lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	basic_string<char_type,traits,allocator_type>
		operator+(char_type lhs, const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	basic_string<char_type,traits,allocator_type>
		operator+(const basic_string<char_type,traits,allocator_type>& lhs,
		const_pointer rhs);
	template<class char_type, class traits, class allocator_type>
	basic_string<char_type,traits,allocator_type>
		operator+(const basic_string<char_type,traits,allocator_type>& lhs, char_type rhs);
	*/
	template<class char_type, class traits, class allocator_type>
	bool operator==(const basic_string<char_type, traits, allocator_type>& lhs,
		const basic_string<char_type, traits, allocator_type>& rhs)
	{
		return lhs.compare( rhs ) == 0;
	}

	template<class char_type, class traits, class allocator_type>
	bool operator==(const char_type* lhs,
		const basic_string<char_type,traits,allocator_type>& rhs)
	{
		return rhs.compare( lhs ) == 0;
	}
	template<class char_type, class traits, class allocator_type>
	bool operator==(const basic_string<char_type,traits,allocator_type>& lhs,
		const char_type* rhs)
	{
		return lhs.compare( rhs ) == 0;
	}
	/*
	template<class char_type, class traits, class allocator_type>
	bool operator!=(const basic_string<char_type,traits,allocator_type>& lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator!=(const_pointer lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator!=(const basic_string<char_type,traits,allocator_type>& lhs,
		const_pointer rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator< (const basic_string<char_type,traits,allocator_type>& lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator< (const basic_string<char_type,traits,allocator_type>& lhs,
		const_pointer rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator< (const_pointer lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator> (const basic_string<char_type,traits,allocator_type>& lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator> (const basic_string<char_type,traits,allocator_type>& lhs,
		const_pointer rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator> (const_pointer lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);

	template<class char_type, class traits, class allocator_type>
	bool operator>=(const basic_string<char_type,traits,allocator_type>& lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator>=(const basic_string<char_type,traits,allocator_type>& lhs,
		const_pointer rhs);
	template<class char_type, class traits, class allocator_type>
	bool operator>=(const_pointer lhs,
		const basic_string<char_type,traits,allocator_type>& rhs);
	// 21.3.7.8:
	template<class char_type, class traits, class allocator_type>
	void swap(basic_string<char_type,traits,allocator_type>& lhs,
		basic_string<char_type,traits,allocator_type>& rhs);
	template<class char_type, class traits, class allocator_type>
	basic_istream<char_type,traits>&
		operator>>(basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type>& str);
	template<class char_type, class traits, class allocator_type>
	basic_ostream<char_type, traits>&
		operator<<(basic_ostream<char_type, traits>& os,
		const basic_string<char_type,traits,allocator_type>& str);
	template<class char_type, class traits, class allocator_type>
	basic_istream<char_type,traits>&
		getline(basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type>& str,
		char_type delim);
	template<class char_type, class traits, class allocator_type>
	basic_istream<char_type,traits>&
		getline(basic_istream<char_type,traits>& is,
		basic_string<char_type,traits,allocator_type>& str);
	typedef basic_string<char> string;
	typedef basic_string<wchar_t> wstring;*/

	typedef gstl::basic_string<char, char_traits<char>, allocator<char> > string;
	typedef gstl::basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;
}

#endif //GSTL_STRING_HEADER