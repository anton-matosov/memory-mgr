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
		//Type T is EqualityComparable
		for( ; first != last && !pred( *first ); ++first )
		{}
		return first;
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1
		find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2);

	template<class ForwardIterator1, class ForwardIterator2,
	class BinaryPredicate>
		ForwardIterator1
		find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred);

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1
		find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2);
	template<class ForwardIterator1, class ForwardIterator2,
	class BinaryPredicate>
		ForwardIterator1
		find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred);

		template<class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first,
		ForwardIterator last);
	template<class ForwardIterator, class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first,
		ForwardIterator last, BinaryPredicate pred);
	template<class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value);
	template<class InputIterator, class Predicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred);
	template<class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2);
	template
		<class InputIterator1, class InputIterator2, class BinaryPredicate>
		pair<InputIterator1, InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, BinaryPredicate pred);
	template<class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2);
	template
		<class InputIterator1, class InputIterator2, class BinaryPredicate>
		bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, BinaryPredicate pred);
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search
		(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2);
	template<class ForwardIterator1, class ForwardIterator2,
	class BinaryPredicate>
		ForwardIterator1 search
		(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred);
	template<class ForwardIterator, class Size, class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value);
	template
		<class ForwardIterator, class Size, class T, class BinaryPredicate>
		ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value,
		BinaryPredicate pred);

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
	BidirectionalIterator2
		copy_backward
		(BidirectionalIterator1 first, BidirectionalIterator1 last,
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
	ForwardIterator2 swap_ranges(ForwardIterator1 first1,
		ForwardIterator1 last1, ForwardIterator2 first2);
	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b);
	template<class InputIterator, class OutputIterator, class UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation op);
	template<class InputIterator1, class InputIterator2, class OutputIterator,
	class BinaryOperation>
		OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, OutputIterator result,
		BinaryOperation binary_op);
	template<class ForwardIterator, class T>
	void replace(ForwardIterator first, ForwardIterator last,
		const T& old_value, const T& new_value);
	template<class ForwardIterator, class Predicate, class T>
	void replace_if(ForwardIterator first, ForwardIterator last,
		Predicate pred, const T& new_value);
	template<class InputIterator, class OutputIterator, class T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
		OutputIterator result,
		const T& old_value, const T& new_value);
	template<class Iterator, class OutputIterator, class Predicate, class T>
	OutputIterator replace_copy_if(Iterator first, Iterator last,
		OutputIterator result,
		Predicate pred, const T& new_value);
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value);
	template<class OutputIterator, class Size, class T>
	void fill_n(OutputIterator first, Size n, const T& value);
	template<class ForwardIterator, class Generator>
	void generate(ForwardIterator first, ForwardIterator last,
		Generator gen);
	template<class OutputIterator, class Size, class Generator>
	void generate_n(OutputIterator first, Size n, Generator gen);

		template<class ForwardIterator, class T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<class ForwardIterator, class Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
		Predicate pred);
	template<class InputIterator, class OutputIterator, class T>
	OutputIterator remove_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& value);
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
	ForwardIterator min_element
		(ForwardIterator first, ForwardIterator last);
	template<class ForwardIterator, class Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
		Compare comp);
	template<class ForwardIterator>
	ForwardIterator max_element
		(ForwardIterator first, ForwardIterator last);
	template<class ForwardIterator, class Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
		Compare comp);
	template<class InputIterator1, class InputIterator2>
	bool lexicographical_compare
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2);
	template<class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare
		(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		Compare comp);
	// 25.3.9, permutations
	template<class BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first,
		BidirectionalIterator last);
	template<class BidirectionalIterator, class Compare>
	bool next_permutation(BidirectionalIterator first,
		BidirectionalIterator last, Compare comp);
	template<class BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first,
		BidirectionalIterator last);
	template<class BidirectionalIterator, class Compare>
	bool prev_permutation(BidirectionalIterator first,
		BidirectionalIterator last, Compare comp);
}

//#pragma warning(pop)
#endif //GSTL_ALGORITHM_HEADER