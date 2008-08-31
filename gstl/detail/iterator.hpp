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

Table 71 - Relations among iterator categories
+-------------------------------------------------------+
|	Random access -> Bidirectional -> Forward -> Input	|
|											-> Output	|
+-------------------------------------------------------+
*/

#include <boost/concept_check.hpp>
#include <boost/type_traits.hpp>
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
	class Pointer = typename boost::type_traits::add_pointer<T>::type,
	class Reference = typename boost::type_traits::add_reference<T>type>
	struct iterator
	{
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef Category iterator_category;
	};

#define GSTL_ITER_CAT( IterT ) typename iterator_traits<IterT>::iterator_category()

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

		template <class ForwardIterator>
		typename iterator_traits<ForwardIterator>::difference_type
			distance(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
		{
			return distance( first, last, input_iterator_tag );
		}

		template <class BidirectionalIterator>
		typename iterator_traits<BidirectionalIterator>::difference_type
			distance(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag)
		{
			return distance( first, last, input_iterator_tag );
		}

		template <class RandIterator>
		typename iterator_traits<RandIterator>::difference_type
			distance(RandIterator first, RandIterator last, random_access_iterator_tag)
		{
			return last - first;
		}
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
	template <class Container> class back_insert_iterator;
	template <class Container>
	back_insert_iterator<Container> back_inserter(Container& x);

	//////////////////////////////////////////////////////////////////////////
	template <class Container> class front_insert_iterator;
	template <class Container>
	front_insert_iterator<Container> front_inserter(Container& x);

	//////////////////////////////////////////////////////////////////////////
	template <class Container> class insert_iterator;
	template <class Container, class Iterator>
	insert_iterator<Container> inserter(Container& x, Iterator i);


	// 24.5, stream iterators:
	template <class T, class charT = char, class traits = char_traits<charT>,
	class Distance = ptrdiff_t>
	class istream_iterator;
	template <class T, class charT, class traits, class Distance>
	bool operator==(const istream_iterator<T,charT,traits,Distance>& x,
		const istream_iterator<T,charT,traits,Distance>& y);
	template <class T, class charT, class traits, class Distance>
	bool operator!=(const istream_iterator<T,charT,traits,Distance>& x,
		const istream_iterator<T,charT,traits,Distance>& y);
	template <class T, class charT = char, class traits = char_traits<charT> >
	class ostream_iterator;
	template<class charT, class traits = char_traits<charT> >
	class istreambuf_iterator;
	template <class charT, class traits>
	bool operator==(const istreambuf_iterator<charT,traits>& a,
		const istreambuf_iterator<charT,traits>& b);
	template <class charT, class traits>
	bool operator!=(const istreambuf_iterator<charT,traits>& a,
		const istreambuf_iterator<charT,traits>& b);
	template <class charT, class traits = char_traits<charT> >
	class ostreambuf_iterator;
}

#endif //GSTL_ITERATOR_HEADER