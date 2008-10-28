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

#ifndef GSTL_ITERATOR_HEADER
#define GSTL_ITERATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/*
Starts on page 541 C++ std 2003
curr 556

Table 71 - Relations among iterator categories
+-------------------------------------------------------+
|	Random access -> Bidirectional -> Forward -> Input	|
|											-> Output	|
+-------------------------------------------------------+
*/

#include <gstl/detail/char_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/config.hpp>

//If there is no STL installed
#ifndef BOOST_STDLIB_CONFIG
//Define iterator tags in std namespace
namespace std 
{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag: public input_iterator_tag {};
	struct bidirectional_iterator_tag: public forward_iterator_tag {};
	struct random_access_iterator_tag: public bidirectional_iterator_tag {};
}
#else
//Include iterator header otherwise
#include <iterator>
#endif

namespace gstl
{
 	typedef std::input_iterator_tag				input_iterator_tag;
 	typedef std::output_iterator_tag			output_iterator_tag;
 	typedef std::forward_iterator_tag			forward_iterator_tag;
 	typedef std::bidirectional_iterator_tag		bidirectional_iterator_tag;
 	typedef std::random_access_iterator_tag		random_access_iterator_tag;


	template<class iterator_type> 
	struct iterator_traits
	{
		typedef typename iterator_type::difference_type difference_type;
		typedef typename iterator_type::value_type value_type;
		typedef typename iterator_type::pointer pointer;
		typedef typename iterator_type::reference reference;
		typedef typename iterator_type::iterator_category iterator_category;
	};

	template<class T>
	struct iterator_traits<T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template<class T> 
	struct iterator_traits<const T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef random_access_iterator_tag iterator_category;
	};

#ifdef GSTL_HAS_FAR_PTR
	template<class T> 
	struct iterator_traits<T __far*>
	{
		typedef long difference_type;
		typedef T value_type;
		typedef T __far* pointer;
		typedef T __far& reference;
		typedef random_access_iterator_tag iterator_category;
	};
#endif GSTL_HAS_FAR_PTR

	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = typename boost::add_pointer<T>::type,
	class Reference = typename boost::add_reference<T>::type>
	struct iterator
	{
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef Category iterator_category;
	};

#define GSTL_ITER_CAT( IterT ) typename iterator_traits<IterT>::iterator_category()
#define GSTL_ITER_VALUE_TYPE( IterT ) typename iterator_traits<IterT>::value_type

	namespace detail
	{
		template <class InputIterator, class Distance>
		void advance(InputIterator& i, Distance n, input_iterator_tag)
		{
			advance( i, n, forward_iterator_tag() );
		}

		template <class ForwardIterator, class Distance>
		void advance(ForwardIterator& i, Distance n, forward_iterator_tag)
		{
			GSTL_ASSERT( n >= 0 );
			while( n-- )
			{
				++i;
			}
		}

		template <class BidirectionalIterator, class Distance>
		void advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
		{
			if( n < 0 )
			{
				while( n++ )
				{
					--i;
				}
			}
			else
			{
				advance( i, n, forward_iterator_tag() );
			}
		}

		template <class RandIterator, class Distance>
		void advance(RandIterator& i, Distance n, random_access_iterator_tag)
		{
			i += n;
		}

		template <class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
			distance(InputIterator first, InputIterator last, input_iterator_tag)
		{
			typename iterator_traits<InputIterator>::difference_type dist = 
				typename iterator_traits<InputIterator>::difference_type();
			for( ; first != last; ++first )
			{
				++dist;
			}
			return dist;
		}

		template <class RandIterator>
		typename iterator_traits<RandIterator>::difference_type
			distance(RandIterator first, RandIterator last, random_access_iterator_tag)
		{
			return last - first;
		}

		template <class Container>
		class container_iterator_traits
		{
		public:
			typedef iterator_traits<typename Container::iterator>	traits;

			typedef typename traits::value_type				value_type;
			typedef typename traits::difference_type		difference_type;
			typedef typename traits::pointer				pointer;
			typedef typename traits::reference				reference;
			typedef typename traits::iterator_category		iterator_category;
		};

		template <class Container, class Category,
		class Traits = container_iterator_traits<Container> >
		class iterator_from_container_base
			:public iterator<Category,
			typename Traits::value_type,
			typename Traits::difference_type,
			typename Traits::pointer,
			typename Traits::reference>
		{
		};
	}

	// 24.3.4, iterator operations:
	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n)
	{
		detail::advance( i, n, GSTL_ITER_CAT(InputIterator) );
	}

	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		return detail::distance( first, last, GSTL_ITER_CAT(InputIterator) );
	}
	

	// 24.4, predefined iterators:
	//template <class Iterator> class reverse_iterator;
	using boost::reverse_iterator;

	

	//////////////////////////////////////////////////////////////////////////
	template <class Container>
	class back_insert_iterator
		:public detail::iterator_from_container_base<Container, output_iterator_tag>
	{
	public:
		typedef Container									container_type;
		typedef typename container_type::const_reference	const_reference;

		typedef back_insert_iterator<container_type>		self_type;

		explicit back_insert_iterator(container_type& x)
			:container_( &x )
		{}

		self_type& operator=( const_reference value )
		{
			container_->push_back( value );
			return *this;
		}

		self_type& operator*()
		{
			return *this;
		}

		self_type& operator++()
		{
			return *this;
		}

		self_type& operator++(int)
		{
			return *this;
		}
	protected:
		Container* container_;
	};

	template <class Container>
	back_insert_iterator<Container> back_inserter(Container& x)
	{
		return back_insert_iterator<Container>( x );
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Container>
	class front_insert_iterator 
		:public detail::iterator_from_container_base<Container, output_iterator_tag>
	{
	public:
		typedef Container									container_type;
		typedef typename container_type::const_reference	const_reference;

		typedef front_insert_iterator<container_type>		self_type;

		explicit front_insert_iterator(container_type& x)
			:container_( &x )
		{}

		self_type& operator=( const_reference value )
		{
			container_->push_front( value );
			return *this;
		}

		self_type& operator*()
		{
			return *this;
		}

		self_type& operator++()
		{
			return *this;
		}

		self_type& operator++(int)
		{
			return *this;
		}
	protected:
		Container* container_;
	};

	template <class Container>
	front_insert_iterator<Container> front_inserter(Container& x)
	{
		return front_insert_iterator<Container>( x );
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Container>
	class insert_iterator
		: public detail::iterator_from_container_base<Container, output_iterator_tag>
	{
	public:
		typedef Container									container_type;
		typedef typename container_type::iterator			iterator;
		typedef typename container_type::const_reference	const_reference;

		typedef insert_iterator<container_type>				self_type;

		explicit insert_iterator( container_type& x, iterator i )
			:container_( &x ),
			iter_( i )
		{}
		
		self_type& operator=( const_reference value )
		{
			iter_ = container_->insert( iter_, value );
			++iter_;
			return *this;
		}

		self_type& operator*()
		{
			return *this;
		}

		self_type& operator++()
		{
			return *this;
		}

		self_type& operator++(int)
		{
			return *this;
		}
	protected:
		Container*	container_;
		iterator	iter_;
	};

	template <class Container, class Iterator>
	insert_iterator<Container> inserter(Container& x, Iterator i)
	{
		return insert_iterator<Container>( x, i );
	}
	//////////////////////////////////////////////////////////////////////////

	// 24.5, stream iterators:
	template <class T, class charT = char, class traits = char_traits<charT>,
	class Distance = ptrdiff_t>
	class istream_iterator:
		public iterator<input_iterator_tag, T, Distance, const T*, const T&>
	{
		typedef iterator<input_iterator_tag, T, Distance, const T*, const T&> base_type;
	public:
		typedef typename base_type::value_type		value_type;
		typedef charT								char_type;
		typedef traits								traits_type;
		typedef typename base_type::difference_type	difference_type;

		typedef istream_iterator<T,charT,traits,Distance>	self_type;
		//TODO: Replace by gstl::basic_istream when implemented
		typedef std::basic_istream<char_type, traits_type> istream_type;

		istream_iterator()
			:in_stream_( 0 )
		{}

		istream_iterator( istream_type& s )
			:in_stream_( &s )
		{
			//read initial value
			++*this;
		}

		~istream_iterator()
		{}

		const T& operator*() const
		{
			if( at_the_end() )
			{
				GSTL_ASSERT( !at_the_end() );
				//TODO: Implement library debugging
				//iterator is not dereferencable
			}
			return value_;
		}

		const T* operator->() const
		{
			return &**this;
		}

		self_type& operator++()
		{
			if( !at_the_end() )
			{
				*in_stream_ >> value_;
			}
			
			return *this;
		}

		self_type operator++(int)
		{
			self_type tmp = *this;
			++*this;
			return tmp;
		}
	private:
		istream_type*	in_stream_; //exposition only
		value_type		value_; //exposition only

		bool at_the_end() const
		{
			if( !in_stream_ || !*in_stream_ )
			{
				//Guaranties that "Two end-of-stream iterators are always equal."
				//and "An end-of-stream iterator is not equal to a non-end-ofstream iterator."
				const_cast<self_type*>( this )->in_stream_ = 0;
				return true;
			}
			return false;
		}

		template <class T, class charT, class traits, class Distance>
		friend bool operator==(const istream_iterator<T,charT,traits,Distance>& x,
			const istream_iterator<T,charT,traits,Distance>& y)
		{
			return x.in_stream_ == y.in_stream_;
		}
	};
	

	template <class T, class charT, class traits, class Distance>
	bool operator!=(const istream_iterator<T,charT,traits,Distance>& x,
		const istream_iterator<T,charT,traits,Distance>& y)
	{
		return !(x == y);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class charT, class traits = char_traits<charT> >
	class istreambuf_iterator
		: public iterator<input_iterator_tag, charT,
		typename traits::off_type, charT*, charT&> {
	public:
		typedef charT									char_type;
		typedef traits									traits_type;
		typedef typename traits::int_type				int_type;
		//TODO: Replace by gstl::basic_istream when implemented
		typedef std::basic_streambuf<char_type, traits_type>	streambuf_type;
		typedef std::basic_istream<char_type, traits_type>		istream_type;
		typedef istreambuf_iterator								self_type;

	public:
		istreambuf_iterator() throw()
			:sbuf_( 0 )
		{}

		istreambuf_iterator( istream_type& s ) throw()
			:sbuf_( s.rdbuf() ),
			valid_( false )
		{}

		istreambuf_iterator( streambuf_type* s ) throw()
			:sbuf_( s ),
			valid_( false )
		{}

		
		char_type operator*() const
		{
			read();
			if( at_the_end() )
			{
				GSTL_ASSERT( !at_the_end() );
				//TODO: Implement library debugging
				//iterator is not dereferencable
			}
			return ' ';
		}

		self_type& operator++()
		{
			next();
			return *this;
		}

 		self_type operator++(int)
 		{
			self_type tmp = *this;
			++*this;
			return tmp;
 		}

		bool equal( istreambuf_iterator& rhs ) const
		{
			//Insure that iterators have valid values
			read();
			rhs.read();

			//true if and only if both iterators are at end-of-stream,
			//or neither is at end-of-stream, regardless
			//of what streambuf object they use.
			return ( at_the_end() && rhs.at_the_end() ) 
				|| ( !at_the_end() && !rhs.at_the_end() );
		}
	private:
		///Traverse to the next char
		void next()
		{	
			if( at_the_end() || traits_type::eq_int_type( traits_type::eof(),
				sbuf_->sbumpc() ) )
			{
				end_reached();
			}
			else
			{
				//Invalidate, so next read will update value
				valid_ = false;
			}
		}

		void read() const
		{
			const_cast<self_type*>( this )->read();
		}

		///Read current char
		void read()
		{
			if( !valid_ )
			{
				int_type tmp;
				if( at_the_end() || traits_type::eq_int_type( traits_type::eof(),
					tmp = sbuf_->sgetc() ) )
				{
					end_reached();
				}
				else
				{
					value_ = traits_type::to_char_type( tmp );
					valid_ = true;
				}
			}
		}

		bool at_the_end() const
		{
			return sbuf_ == 0;
		}

		void end_reached()
		{
			sbuf_ = 0;
			valid_ = true;
		}

		streambuf_type* sbuf_;
		char_type		value_;
		bool			valid_;
	};

	template <class charT, class traits>
	bool operator==(const istreambuf_iterator<charT,traits>& lhs,
		const istreambuf_iterator<charT,traits>& rhs)
	{
		return lhs.equal( rhs );
	}

	template <class charT, class traits>
	bool operator!=(const istreambuf_iterator<charT,traits>& lhs,
		const istreambuf_iterator<charT,traits>& rhs)
	{
		return !( lhs == rhs );
	}

	//////////////////////////////////////////////////////////////////////////
	template <class T, class charT = char, class traits = char_traits<charT> >
	class ostream_iterator
		: public iterator<output_iterator_tag, T>
	{
		typedef iterator<output_iterator_tag, T>	base_type;
	public:
		typedef typename base_type::value_type		value_type;
		typedef charT								char_type;
		typedef traits								traits_type;
		typedef typename base_type::difference_type	difference_type;
		//TODO: Replace by gstl::basic_ostream when implemented
		typedef std::basic_ostream<char_type, traits_type>	ostream_type;
		typedef ostream_iterator<T,charT,traits>		self_type;

		ostream_iterator( ostream_type& s, const char_type* delimiter = 0 )
			:out_stream_( &s ),
			delim_( delimiter )
		{}

		~ostream_iterator()
		{}

		self_type& operator=( const T& value )
		{
			*out_stream_ << value;
			if( delim_ )
			{
				*out_stream_ << delim_;
			}
			
			return *this;
		}
		self_type& operator*()
		{
			return *this;
		}

		self_type& operator++()
		{
			return *this;
		}

		self_type& operator++(int)
		{
			return *this;
		}
	private:
		ostream_type*		out_stream_;
		const char_type*	delim_;
	};

	//////////////////////////////////////////////////////////////////////////
	template <class charT, class traits = char_traits<charT> >
	class ostreambuf_iterator:
		public iterator<output_iterator_tag, charT>
	{
	public:
		typedef charT											char_type;
		typedef traits											traits_type;
		//TODO: Replace by gstl::basic_ostream when implemented
		typedef std::basic_streambuf<char_type, traits_type>	streambuf_type;
		typedef std::basic_ostream<char_type, traits_type>		ostream_type;

		ostreambuf_iterator( ostream_type& s ) throw()
			:sbuf_( s.rdbuf() ),
			fail_( false )
		{
			GSTL_ASSERT( sbuf_ != 0 );
		}
		ostreambuf_iterator( streambuf_type* s ) throw()
			:sbuf_( s ),
			fail_( false )
		{
			GSTL_ASSERT( sbuf_ != 0 );
		}

		ostreambuf_iterator& operator=( char_type c )
		{
			if( !failed() )
			{
				fail_ = traits_type::eq_int_type( sbuf_->sputc( c ), traits_type::eof() );
			}
			return *this;
		}

		ostreambuf_iterator& operator*()
		{
			return *this;
		}

		ostreambuf_iterator& operator++()
		{
			return *this;
		}

		ostreambuf_iterator& operator++(int)
		{
			return *this;
		}


		bool failed() const throw()
		{
			return fail_;
		}
	private:
		streambuf_type* sbuf_;
		bool fail_;
	};
}

#endif //GSTL_ITERATOR_HEADER