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

#ifndef GSTL_ALGORITHM_HEADER
#define GSTL_ALGORITHM_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/utility>
#include <gstl/iterator>

//#pragma warning(push)
#pragma warning( disable:4100 )
#include <boost/concept_check.hpp>

namespace gstl
{
/*
page 577
*/
	namespace detail
	{
		template<class RanIt1, class RanIt2, class Pred>
		RanIt1 find_end( RanIt1 first1, RanIt1 last1,
			RanIt2 first2, RanIt2 last2, Pred pred,
			random_access_iterator_tag /*Iterator1 category*/,
			random_access_iterator_tag /*Iterator2 category*/ )
		{
			RanIt1 it1, limit, ret;
			RanIt2 it2;

			limit = first1;
			advance( limit, 1 + distance( first1, last1 ) - distance( first2, last2 ) );
			ret = last1;

			while( first1 != limit )
			{
				it1 = first1;
				it2 = first2;
				while( pred( *it1, *it2 ) )
				{ 
					++it1;
					++it2;
					if( it2 == last2 )
					{
						ret = first1;
						break;
					} 
				}
				++first1;
			}
			return ret;
		}

		template<class ForwardIterator1, class ForwardIterator2,
		class BinaryPredicate>
			ForwardIterator1
			find_end( ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2,
			BinaryPredicate pred,
			forward_iterator_tag /*Iterator1 category*/,
			forward_iterator_tag /*Iterator2 category*/)
		{
			if( first2 == last2 )
			{//Second sequence is empty
				return last1;
			}
			else 
			{
				ForwardIterator1 result = last1;
				ForwardIterator1 new_result;
				for(;;) 
				{
					new_result = gstl::search( first1, last1, first2, last2, pred );
					if( new_result == last1 )
					{
						return result;
					}
					else 
					{
						result = new_result;
						first1 = new_result;
						++first1;
					}
				}
			}
		}

		template<class RanIt1, class RanIt2, class Pred>
		RanIt1 search( RanIt1 first1, RanIt1 last1, RanIt2 first2, RanIt2 last2, Pred pred, 
			random_access_iterator_tag /*Iterator1 category*/,
			random_access_iterator_tag /*Iterator2 category*/ )
		{
			RanIt1 it1, limit;
			RanIt2 it2;

			limit = first1; 
			advance( limit, 1 + distance( first1, last1 ) - distance( first2, last2 ) );

			while( first1 != limit )
			{
				it1 = first1;
				it2 = first2;
				while( pred( *it1, *it2 ) )
				{ 
					++it1;
					++it2;
					if( it2 == last2 )
					{
						return first1;
					}
				}
				++first1;
			}
			return last1;
		}

		template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
		ForwardIterator1 search( ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred, 
			forward_iterator_tag /*Iterator1 category*/,
			forward_iterator_tag /*Iterator2 category*/ )
		{
			// Test for empty ranges
			if (first1 == last1 || first2 == last2)
			{
				return first1;
			}

			// Test for a pattern of length 1.
			ForwardIterator2 p1( first2 );

			if ( ++p1 == last2 )
			{
				while( first1 != last1 && !pred( *first1, *first2 ) )
				{
					++first1;
				}
				return first1;
			}

			// General case.
			for ( ; ; ) // first1 != last1 will be checked below
			{ 
				while( first1 != last1 && !pred( *first1, *first2 ) )
				{
					++first1;
				}
				if( first1 == last1 ) 
				{
					return last1;
				}
				ForwardIterator2 p = p1;
				ForwardIterator1 current = first1;
				if( ++current == last1 )
				{
					return last1;
				}

				while( pred( *current, *p ) ) 
				{
					if ( ++p == last2 )
					{
						return first1;
					}
					if ( ++current == last1 )
					{
						return last1;
					}
				}

				++first1;
			}
			//return first1;
		}
	}

	// 25.1, non-modifying sequence operations:
	template<class InputIterator, class Function>
	Function for_each(InputIterator first, InputIterator last, Function f)
	{
		BOOST_CONCEPT_ASSERT(( boost::InputIterator<InputIterator> ));
		for( ; first != last; ++first )
		{
			f( *first );
		}
		return f;
	}
	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last,
		const T& value)
	{
		BOOST_CONCEPT_ASSERT(( boost::InputIterator<InputIterator> ));
		BOOST_CONCEPT_ASSERT(( boost::EqualityComparable<T> ));
		GSTL_DEBUG_RANGE( first, last );

		//Type T is EqualityComparable
		for( ; first != last && !(*first == value); ++first )
		{}
		return first;
	}
	template<class InputIterator, class Predicate>
	InputIterator find_if(InputIterator first, InputIterator last,
		Predicate pred)
	{
		BOOST_CONCEPT_ASSERT(( boost::InputIterator<InputIterator> ));
		BOOST_CONCEPT_ASSERT(( boost::UnaryPredicate<Predicate, 
			typename gstl::iterator_traits<InputIterator>::value_type > ));

		GSTL_DEBUG_RANGE( first, last );
		//Type T is EqualityComparable
		for( ; first != last; ++first )
		{
			if( pred( *first ) )
			{
				break;
			}
		}
		return first;
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1
		find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return find_end( first1, last1, first2, last2, 
			std::equal_to<GSTL_ITER_VALUE_TYPE(ForwardIterator1)>() );
	}

	/*
	Effects: Finds a subsequence of equal values in a sequence.

	Returns: The last iterator i in the range [first1, last1 - (last2-first2)) such that for any
		non-negative integer n < (last2-first2), the following corresponding conditions hold: *(i+n)
		== *(first2+n), pred(*(i+n),*(first2+n)) != false. Returns last1 if no such
		iterator is found.
	Complexity: At most (last2 - first2) * (last1 - first1 - (last2 - first2) + 1) 
		applications of the corresponding predicate.
	*/
	template<class ForwardIterator1, class ForwardIterator2,
	class BinaryPredicate>
		ForwardIterator1
		find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred)
	{
		GSTL_DEBUG_RANGE( first1, last1 );
		GSTL_DEBUG_RANGE( first2, last2 );
		BOOST_CONCEPT_ASSERT(( boost::ForwardIterator<ForwardIterator1> ));
		BOOST_CONCEPT_ASSERT(( boost::ForwardIterator<ForwardIterator2> ));
		BOOST_CONCEPT_ASSERT(( boost::BinaryPredicate<BinaryPredicate,
			typename gstl::iterator_traits<ForwardIterator1>::value_type,
			typename gstl::iterator_traits<ForwardIterator2>::value_type> ));


		return detail::find_end( first1, last1, first2, last2, pred,
			GSTL_ITER_CAT( ForwardIterator1 ), GSTL_ITER_CAT( ForwardIterator2 ) );
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1
		find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return find_first_of( first1, last1, first2, last2, 
			std::equal_to<GSTL_ITER_VALUE_TYPE(ForwardIterator1)>() );
	}

	template<class ForwardIterator1, class ForwardIterator2,
	class BinaryPredicate>
		ForwardIterator1
		find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred)
	{
		for( ; first1 != last1; ++first1 )
		{
			for( ForwardIterator2 it=first2; it != last2; ++it )
			{
				if( pred( *it,*first1 ) )
				{
					return first1;
				}
			}
		}
		return last1;
	}

		template<class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first,
		ForwardIterator last)
	{
		return adjacent_find( first, last, 
			std::equal_to<GSTL_ITER_VALUE_TYPE( ForwardIterator )> );
	}

	/**
	   @brief	Searches the range [first,last)
	   @details for the first occurrence of two consecutive equal elements
	   @return	an iterator to the first of these two elements.
	   Complexity
	   At most, performs as many applications of pred as the
	   number of elements in the range [first,last) minus one.
	*/
	template<class ForwardIterator, class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first,
		ForwardIterator last, BinaryPredicate pred)
	{
		if( first != last )
		{	
			ForwardIterator next = first;
			++next;
			while( next != last )
			{	
				if( pred( *first, *next ) )
				{
					return first;
				}
				++first;
				++next;
			}
		}
		return last;
	}

	template<class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value)
	{
		ptrdiff_t ret = 0;
		while( first != last )
		{
			if( *first == value )
			{
				++ret;
			}
			++first;
		}
		return ret;
	}

	template<class InputIterator, class Predicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred)
	{
		ptrdiff_t ret = 0;
		while( first != last )
		{
			if( pred( *first ) )
			{
				++ret;
			}
			++first;
		}
		return ret;
	}
	
	template<class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2)
	{
		return mismatch( first1, last1, first2, 
			std::not_equal_to<GSTL_ITER_VALUE_TYPE( InputIterator1 )> );
	}
	
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, BinaryPredicate pred)
	{
		while( first1 != last1 )
		{
			if( !pred( *first1,*first2) )
			{
				break;
			}
			++first1; ++first2;
		}
		return make_pair( first1, first2 );
	}

	template<class InputIterator1, class InputIterator2>
	bool equal( InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2 )
	{
		return equal( first1, last1, first2, std::not_equal_to<GSTL_ITER_VALUE_TYPE( InputIterator1 )> );
	}

	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool equal( InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, BinaryPredicate pred )
	{
		return mismatch( first1, last1, first2, pred ).first == last1;
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search( ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2 )
	{
		return search( first1, last1, first2, last2, std::equal_to<GSTL_ITER_VALUE_TYPE( ForwardIterator1 )> );
	}

	template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 search( ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred )
	{
		GSTL_DEBUG_RANGE( first1, last1 );
		GSTL_DEBUG_RANGE( first2, last2 );
		BOOST_CONCEPT_ASSERT(( boost::ForwardIterator<ForwardIterator1> ));
		BOOST_CONCEPT_ASSERT(( boost::ForwardIterator<ForwardIterator2> ));
		BOOST_CONCEPT_ASSERT(( boost::BinaryPredicate<BinaryPredicate,
			GSTL_ITER_VALUE_TYPE( ForwardIterator1 ),
			GSTL_ITER_VALUE_TYPE( ForwardIterator2 )> ));

		return detail::search( first1, last1, first2, last2, pred,
			GSTL_ITER_CAT( ForwardIterator1 ), GSTL_ITER_CAT( ForwardIterator2 ) );
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value)
	{
		return search_n( first, last, count, value, std::equal<T>() );
	}


	template
		<class ForwardIterator, class Size, class T, class BinaryPredicate>
		ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value,
		BinaryPredicate pred)
	{
		GSTL_DEBUG_RANGE( first, last );
		BOOST_CONCEPT_ASSERT(( boost::ForwardIterator<ForwardIterator> ));
		BOOST_CONCEPT_ASSERT(( boost::BinaryPredicate<BinaryPredicate,
			typename gstl::iterator_traits<ForwardIterator>::value_type, T ));

		ForwardIterator limit = first;
		advance( limit, distance( first, last ) - count );

		ForwardIterator it;
		Size i;
		while( first != limit )
		{
			it = first;
			i = 0;
			while( pred( *it, value ) )
			{ 
				++it;
				if( ++i == count )
				{
					return first;
				}
			}
			++first;
		}
		return last;
	}

	// 25.2, modifying sequence operations:
	// 25.2.1, copy:
	template<class InputIterator, class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last,
		OutputIterator dest)
	{
		for( ; first != last; ++first, ++dest )
		{
			*dest = *first;
		}
		return dest;
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 copy_backward( BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 dest)
	{
		for( ; first != last; ++first, ++dest )
		{
			*dest = *first;
		}
		return dest;
	}

	// 25.2.2, swap:
	template<class T> void swap(T& a, T& b);

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator2 swap_ranges( ForwardIterator1 first1,
		ForwardIterator1 last1, ForwardIterator2 first2 );

	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap( ForwardIterator1 a, ForwardIterator2 b );

	template<class InputIterator, class OutputIterator, class UnaryOperation>
	OutputIterator transform( InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation op );

	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class BinaryOperation>
		OutputIterator transform( InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, OutputIterator result,
		BinaryOperation binary_op );

	template<class ForwardIterator, class T>
	void replace( ForwardIterator first, ForwardIterator last,
		const T& old_value, const T& new_value );

	template<class ForwardIterator, class Predicate, class T>
	void replace_if( ForwardIterator first, ForwardIterator last,
		Predicate pred, const T& new_value );

	template<class InputIterator, class OutputIterator, class T>
	OutputIterator replace_copy( InputIterator first, InputIterator last,
		OutputIterator result, const T& old_value, const T& new_value );

	template<class Iterator, class OutputIterator, class Predicate, class T>
	OutputIterator replace_copy_if( Iterator first, Iterator last,
		OutputIterator result, Predicate pred, const T& new_value );

	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		while( first != last )  
		{
			*first = value;
			++first;
		}
	}

	template<class OutputIterator, class Size, class T>
	void fill_n(OutputIterator first, Size n, const T& value)
	{
		while( n-- )
		{
			*first = value;
			++first;
		}
	}

	template<class ForwardIterator, class Generator>
	void generate( ForwardIterator first, ForwardIterator last,
		Generator gen );
	template<class OutputIterator, class Size, class Generator>
	void generate_n( OutputIterator first, Size n, Generator gen );

		template<class ForwardIterator, class T>
	ForwardIterator remove( ForwardIterator first, ForwardIterator last,
		const T& value );

	template<class ForwardIterator, class Predicate>
	ForwardIterator remove_if( ForwardIterator first, ForwardIterator last,
		Predicate pred );

	template<class InputIterator, class OutputIterator, class T>
	OutputIterator remove_copy( InputIterator first, InputIterator last,
		OutputIterator result, const T& value );

	template<class InputIterator, class OutputIterator, class Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred);
	template<class ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last);
	template<class ForwardIterator, class BinaryPredicate>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last,
		BinaryPredicate pred);
	template<class InputIterator, class OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result);
	template<class InputIterator, class OutputIterator, class BinaryPredicate>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, BinaryPredicate pred);
	template<class BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last);
	template<class BidirectionalIterator, class OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first,
		BidirectionalIterator last,
		OutputIterator result);
	template<class ForwardIterator>
	void rotate(ForwardIterator first, ForwardIterator middle,
		ForwardIterator last);
	template<class ForwardIterator, class OutputIterator>
	OutputIterator rotate_copy
		(ForwardIterator first, ForwardIterator middle,
		ForwardIterator last, OutputIterator result);
	template<class RandomAccessIterator>
	void random_shuffle(RandomAccessIterator first,
		RandomAccessIterator last);
	template<class RandomAccessIterator, class RandomNumberGenerator>
	void random_shuffle(RandomAccessIterator first,
		RandomAccessIterator last,
		RandomNumberGenerator& rand);
	// 25.2.12, partitions:
	template<class BidirectionalIterator, class Predicate>
	BidirectionalIterator partition(BidirectionalIterator first,
		BidirectionalIterator last,
		Predicate pred);
	template<class BidirectionalIterator, class Predicate>
	BidirectionalIterator stable_partition(BidirectionalIterator first,
		BidirectionalIterator last,
		Predicate pred);

		// 25.3, sorting and related operations:
		// 25.3.1, sorting:
		template<class RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<class RandomAccessIterator>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<class RandomAccessIterator>
	void partial_sort(RandomAccessIterator first,
		RandomAccessIterator middle,
		RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void partial_sort(RandomAccessIterator first,
		RandomAccessIterator middle,
		RandomAccessIterator last, Compare comp);
	template<class InputIterator, class RandomAccessIterator>
	RandomAccessIterator
		partial_sort_copy(InputIterator first, InputIterator last,
		RandomAccessIterator result_first,
		RandomAccessIterator result_last);
	template<class InputIterator, class RandomAccessIterator, class Compare>
	RandomAccessIterator
		partial_sort_copy(InputIterator first, InputIterator last,
		RandomAccessIterator result_first,
		RandomAccessIterator result_last,
		Compare comp);
	template<class RandomAccessIterator>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, Compare comp);
	// 25.3.3, binary search:
	template<class ForwardIterator, class T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<class ForwardIterator, class T, class Compare>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);
	template<class ForwardIterator, class T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<class ForwardIterator, class T, class Compare>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);

		template<class ForwardIterator, class T>
	pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<class ForwardIterator, class T, class Compare>
	pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);
	template<class ForwardIterator, class T>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<class ForwardIterator, class T, class Compare>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);
	// 25.3.4, merge:
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class Compare>
		OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp);
	template<class BidirectionalIterator>
	void inplace_merge(BidirectionalIterator first,
		BidirectionalIterator middle,
		BidirectionalIterator last);
	template<class BidirectionalIterator, class Compare>
	void inplace_merge(BidirectionalIterator first,
		BidirectionalIterator middle,
		BidirectionalIterator last, Compare comp);
	// 25.3.5, set operations:
	template<class InputIterator1, class InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2);
	template<class InputIterator1, class InputIterator2, class Compare>
	bool includes
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compare comp);
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class Compare>
		OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp);

		template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_intersection
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class Compare>
		OutputIterator set_intersection
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp);
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_difference
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class Compare>
		OutputIterator set_difference
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp);
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator
		set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class Compare>
		OutputIterator
		set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result, Compare comp);
	// 25.3.6, heap operations:
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

		// 25.3.7, minimum and maximum:

#pragma push_macro("min")
#ifdef min
#	undef min
#endif
	template<class T>
	static inline const T min( const T& v1, const T& v2 )
	{
		return gstl::min( v1, v2, std::less<T>() );
	}

	template<class T, class Pred>
	static inline const T min( const T& v1, const T& v2, Pred pred )
	{
		return pred(v1, v2) ? v1 : v2;
	}

#pragma pop_macro("min")
#pragma push_macro("max")
#ifdef max
#	undef max
#endif
	template<class T>
	static inline const T max( const T& v1, const T& v2 )
	{
		return gstl::max( v1, v2, std::less<T>() );
	}

	template<class T, class Pred>
	static inline const T max( const T& v1, const T& v2, Pred pred )
	{
		return pred(v2, v1) ? v1 : v2;
	}

#pragma pop_macro("max")


	template<class ForwardIterator>
	ForwardIterator min_element( ForwardIterator first, ForwardIterator last );

	template<class ForwardIterator, class Compare>
	ForwardIterator min_element( ForwardIterator first, ForwardIterator last,
		Compare comp );
	
	template<class ForwardIterator>
	ForwardIterator max_element( ForwardIterator first, ForwardIterator last );
	
	template<class ForwardIterator, class Compare>
	ForwardIterator max_element( ForwardIterator first, ForwardIterator last,
		Compare comp );

	template<class InputIterator1, class InputIterator2>
	bool lexicographical_compare( InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2 );
	template<class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare( InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		Compare comp );

	// 25.3.9, permutations
	template<class BidirectionalIterator>
	bool next_permutation( BidirectionalIterator first,
		BidirectionalIterator last );
	template<class BidirectionalIterator, class Compare>
	bool next_permutation( BidirectionalIterator first,
		BidirectionalIterator last, Compare comp );
	template<class BidirectionalIterator>
	bool prev_permutation( BidirectionalIterator first,
		BidirectionalIterator last );
	template<class BidirectionalIterator, class Compare>
	bool prev_permutation( BidirectionalIterator first,
		BidirectionalIterator last, Compare comp );
}

//#pragma warning(pop)
#endif //GSTL_ALGORITHM_HEADER
