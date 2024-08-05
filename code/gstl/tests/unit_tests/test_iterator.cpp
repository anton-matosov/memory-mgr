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



#include <deque>
#include <boost/foreach.hpp>
#include <gstl/iterator>
#include <gstl/algorithm>
#include <gstl/detail/helpers.hpp>
#include <boost/implicit_cast.hpp>

class iterator_test_fixture
{
public:
};


//Instantiate iterator base templates
template struct gstl::iterator<gstl::input_iterator_tag, int>;
template struct gstl::iterator<gstl::input_iterator_tag, int*>;
template struct gstl::iterator<gstl::input_iterator_tag, const int*>;
template struct gstl::iterator<gstl::input_iterator_tag, int&>;
template struct gstl::iterator<gstl::input_iterator_tag, const int&>;


typedef gstl::iterator<gstl::input_iterator_tag, int>			int_iter_t;
//typedef gstl::iterator<gstl::input_iterator_tag, int*>			int_ptr_iter_t;
//typedef gstl::iterator<gstl::input_iterator_tag, const int*>	const_int_ptr_iter_t;

template struct gstl::iterator_traits<int_iter_t>;
template struct gstl::iterator_traits<int*>;
template struct gstl::iterator_traits<const int*>;


BOOST_FIXTURE_TEST_SUITE( iterator_test, iterator_test_fixture )

typedef boost::mpl::list< int, float, double > t_list;

BOOST_AUTO_TEST_CASE( test_iterator_tags )
{
	gstl::input_iterator_tag i_tag;
	gstl::helpers::unused_variable( i_tag );

	gstl::output_iterator_tag o_tag;
	gstl::helpers::unused_variable( o_tag );

	gstl::forward_iterator_tag f_tag;
	gstl::helpers::unused_variable( f_tag );

	gstl::bidirectional_iterator_tag b_tag;
	gstl::helpers::unused_variable( b_tag );

	gstl::random_access_iterator_tag r_tag;
	gstl::helpers::unused_variable( r_tag );

	BOOST_CONCEPT_ASSERT( (boost::Convertible<gstl::forward_iterator_tag, gstl::input_iterator_tag>) );
	BOOST_CONCEPT_ASSERT( (boost::Convertible<gstl::bidirectional_iterator_tag, gstl::forward_iterator_tag>) );
	BOOST_CONCEPT_ASSERT( (boost::Convertible<gstl::random_access_iterator_tag, gstl::bidirectional_iterator_tag>) );
}

//////////////////////////////////////////////////////////////////////////
// These tests were took from boost\pending\iterator_tests.hpp and adopted to
// boost::test framework
//
// Tests whether type Iterator satisfies the requirements for a
// TrivialIterator.
// Preconditions: i, j, *i, val
template <class Iterator, class T>
void trivial_iterator_test(const Iterator i, const Iterator j, T val)
{
	Iterator k;
	BOOST_CHECK_EQUAL(i, i);
	BOOST_CHECK_EQUAL(j, j);
	BOOST_CHECK_NE(i, j);
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
	T v = *i;
#else
	typename gstl::iterator_traits<Iterator>::value_type v = *i;
#endif
	BOOST_CHECK_EQUAL(v, val);
#if 0
	// hmm, this will give a warning for transform_iterator...  perhaps
	// this should be separated out into a stand-alone test since there
	// are several situations where it can't be used, like for
	// integer_range::iterator.
	BOOST_CHECK_EQUAL(v, i->foo());
#endif
	k = i;
	BOOST_CHECK_EQUAL(k, k);
	BOOST_CHECK_EQUAL(k, i);
	BOOST_CHECK_NE(k, j);
	BOOST_CHECK_EQUAL(*k, val);
}


// Preconditions: i, j
template <class Iterator, class T>
void mutable_trivial_iterator_test(const Iterator i, const Iterator j, T val)
{
	*i = val;
	trivial_iterator_test(i, j, val);
}


// Preconditions: *i, v1, *++i, v2
template <class Iterator, class T>
void input_iterator_test(Iterator i, T v1, T v2)
{
	Iterator i1(i);

	BOOST_CHECK_EQUAL(i, i1);
	BOOST_CHECK(!(i != i1));

	// I can see no generic way to create an input iterator
	// that is in the domain of== of i and != i.
	// The following works for istream_iterator but is not
	// guaranteed to work for arbitrary input iterators.
	//
	//   Iterator i2;
	//
	//   assert(i != i2);
	//   assert(!(i == i2));

	BOOST_CHECK_EQUAL(*i1, v1);
	BOOST_CHECK_EQUAL(*i , v1);

	// we cannot test for equivalence of (void)++i & (void)i++
	// as i is only guaranteed to be single pass.
	BOOST_CHECK_EQUAL(*i++, v1);

	i1 = i;

	BOOST_CHECK_EQUAL(i, i1);
	BOOST_CHECK(!(i != i1));

	BOOST_CHECK_EQUAL(*i1, v2);
	BOOST_CHECK_EQUAL(*i , v2);

	// i is dereferencable, so it must be incrementable.
	++i;

	// how to test for operator-> ?
}

// how to test output iterator?


template <bool is_pointer> struct lvalue_test
{
	template <class Iterator> static void check(Iterator)
	{
# ifndef BOOST_NO_STD_ITERATOR_TRAITS
		typedef typename gstl::iterator_traits<Iterator>::reference reference;
		typedef typename gstl::iterator_traits<Iterator>::value_type value_type;
# else
		typedef typename Iterator::reference reference;
		typedef typename Iterator::value_type value_type;
# endif
		BOOST_STATIC_ASSERT(boost::is_reference<reference>::value);
		BOOST_STATIC_ASSERT((boost::is_same<reference,value_type&>::value
			|| boost::is_same<reference,const value_type&>::value
			));
	}
};

# ifdef BOOST_NO_STD_ITERATOR_TRAITS
template <> struct lvalue_test<true> {
	template <class T> static void check(T) {}
};
#endif

template <class Iterator, class T>
void forward_iterator_test(Iterator i, T v1, T v2)
{
	input_iterator_test(i, v1, v2);

	Iterator i1 = i, i2 = i;

	BOOST_CHECK_EQUAL(i, i1++);
	BOOST_CHECK_NE(i, ++i2);

	trivial_iterator_test(i, i1, v1);
	trivial_iterator_test(i, i2, v1);

	++i;
	BOOST_CHECK_EQUAL(i, i1);
	BOOST_CHECK_EQUAL(i, i2);
	++i1;
	++i2;

	trivial_iterator_test(i, i1, v2);
	trivial_iterator_test(i, i2, v2);

	// borland doesn't allow non-type template parameters
# if !defined(__BORLANDC__) || (__BORLANDC__ > 0x551)
	lvalue_test<(boost::is_pointer<Iterator>::value)>::check(i);
#endif
}

// Preconditions: *i, v1, *++i, v2
template <class Iterator, class T>
void bidirectional_iterator_test(Iterator i, T v1, T v2)
{
	forward_iterator_test(i, v1, v2);
	++i;

	Iterator i1 = i, i2 = i;

	BOOST_CHECK_EQUAL(i, i1--);
	BOOST_CHECK_NE(i, --i2);

	trivial_iterator_test(i, i1, v2);
	trivial_iterator_test(i, i2, v2);

	--i;
	BOOST_CHECK_EQUAL(i, i1);
	BOOST_CHECK_EQUAL(i, i2);
	++i1;
	++i2;

	trivial_iterator_test(i, i1, v1);
	trivial_iterator_test(i, i2, v1);
}

// mutable_bidirectional_iterator_test

template <class U> struct undefined;

// Preconditions: [i,i+N) is a valid range
template <class Iterator, class TrueVals>
void random_access_iterator_test(Iterator i, int N, TrueVals vals)
{
	bidirectional_iterator_test(i, vals[0], vals[1]);
	const Iterator j = i;
	int c;

	typedef typename gstl::iterator_traits<Iterator>::value_type value_type;

	for (c = 0; c < N-1; ++c)
	{
		BOOST_CHECK_EQUAL(i, j + c);
		BOOST_CHECK_EQUAL(*i, vals[c]);
		BOOST_CHECK_EQUAL(*i, boost::implicit_cast<value_type>(j[c]));
		BOOST_CHECK_EQUAL(*i, *(j + c));
		BOOST_CHECK_EQUAL(*i, *(c + j));
		++i;
		BOOST_CHECK_GT(i, j);
		BOOST_CHECK_GE(i, j);
		BOOST_CHECK_LE(j, i);
		BOOST_CHECK_LT(j, i);
	}

	Iterator k = j + N - 1;
	for (c = 0; c < N-1; ++c)
	{
		BOOST_CHECK_EQUAL(i, k - c);
		BOOST_CHECK_EQUAL(*i, vals[N - 1 - c]);
		BOOST_CHECK_EQUAL(*i, boost::implicit_cast<value_type>(j[N - 1 - c]));
		Iterator q = k - c;
		BOOST_CHECK_EQUAL(*i, *q);
		BOOST_CHECK_GT(i, j);
		BOOST_CHECK_GE(i, j);
		BOOST_CHECK_LE(j, i);
		BOOST_CHECK_LT(j, i);
		--i;
	}
}

// Precondition: i, j
template <class Iterator, class ConstIterator>
void const_nonconst_iterator_test(Iterator i, ConstIterator j)
{
	BOOST_CHECK_NE(i, j);
	BOOST_CHECK_NE(j, i);

	ConstIterator k(i);
	BOOST_CHECK_EQUAL(k, i);
	BOOST_CHECK_EQUAL(i, k);

	k = i;
	BOOST_CHECK_EQUAL(k, i);
	BOOST_CHECK_EQUAL(i, k);
}

BOOST_AUTO_TEST_CASE( test_advance )
{
	typedef std::vector<int> int_vec_type;
	int_vec_type int_vec( 10 );
	int_vec_type::iterator iv = int_vec.begin();
	int_vec_type::iterator iv2 = iv;
	int_vec_type::iterator iv3 = iv;

	gstl::advance( iv, 5 );
	BOOST_CHECK_EQUAL( *iv, int_vec[5] );

	gstl::advance( iv2, 2 );
	BOOST_CHECK_EQUAL( *iv2, int_vec[2] );

	gstl::advance( iv3, 0 );
	BOOST_CHECK_EQUAL( *iv3, int_vec[0] );


	gstl::advance( iv2, -2 );
	BOOST_CHECK_EQUAL( *iv2, int_vec[0] );
	BOOST_CHECK_EQUAL( *iv2, *iv3 );


	typedef std::list<int> int_list_type;
	int_list_type int_list( int_vec.begin(), int_vec.end() );

	int_list_type::iterator il = int_list.begin();
	int_list_type::iterator il2 = il;
	int_list_type::iterator il2_s = il;

	gstl::advance( il2, 5 );
	std::advance( il2_s, 5 );
	BOOST_CHECK( il2 == il2_s );
	BOOST_CHECK_EQUAL( *il2, *il2_s );

	gstl::advance( il2, -2 );
	std::advance( il2_s, -2 );
	BOOST_CHECK( il2 == il2_s );
	BOOST_CHECK_EQUAL( *il2, *il2_s );
}

BOOST_AUTO_TEST_CASE( test_distance )
{
	typedef std::vector<int> int_vec_type;
	int_vec_type int_vec( 10 );
	int i = 0;
	BOOST_FOREACH( int& vi, int_vec )
	{
		vi = ++i;
	}

	int_vec_type::iterator it = int_vec.begin();
	int_vec_type::iterator it0 = it;
	int_vec_type::iterator it3 = it + 3;
	int_vec_type::iterator it5 = it + 5;

	BOOST_CHECK_EQUAL( gstl::distance( it, it0 ), 0 );
	BOOST_CHECK_EQUAL( gstl::distance( it, it3 ), 3 );
	BOOST_CHECK_EQUAL( gstl::distance( it, it5 ), 5 );
	BOOST_CHECK_EQUAL( gstl::distance( it3, it5 ), 2 );

	typedef std::list<int> int_list_type;
	int_list_type int_list( int_vec.begin(), int_vec.end() );

	int_list_type::iterator il = int_list.begin();
	int_list_type::iterator il0 = il;
	int_list_type::iterator il3 = il;
	int_list_type::iterator il5= il;
	std::advance( il3, 3 );
	std::advance( il5, 5 );

	BOOST_CHECK_EQUAL( gstl::distance( il, il0 ), 0 );
	BOOST_CHECK_EQUAL( gstl::distance( il, il3 ), 3 );
	BOOST_CHECK_EQUAL( gstl::distance( il, il5 ), 5 );
	BOOST_CHECK_EQUAL( gstl::distance( il3, il5 ), 2 );
}

BOOST_AUTO_TEST_CASE( test_reverse_iterator )
{
	char array[] = "only test array";
	const size_t arr_len = GSTL_ARRAY_LEN( array );

	typedef char* iter_t;
	typedef const char* const_iter_t;
	typedef gstl::reverse_iterator<iter_t> riter_t;
	typedef gstl::reverse_iterator<const_iter_t> const_riter_t;

	iter_t begin = array;
	iter_t end= array + arr_len;
	riter_t rbegin( end );
	riter_t rend( begin );

	char reversed[arr_len];
	iter_t rev_begin = reversed;
	iter_t rev_end	= reversed + arr_len;
	const_iter_t const_rev_end	= reversed + arr_len;

	gstl::copy(begin, end, reversed);
	std::reverse( rev_begin, rev_end );

	riter_t i(rev_end);
	random_access_iterator_test(i, arr_len, array);

	random_access_iterator_test(boost::make_reverse_iterator(rev_end), arr_len, array);

	const_riter_t j(rev_end);
	random_access_iterator_test(j, arr_len, array);

	random_access_iterator_test(boost::make_reverse_iterator(const_rev_end), arr_len, array);

	const_nonconst_iterator_test(i, ++j);
}

	typedef std::deque<int> int_deque_type;
	template class gstl::back_insert_iterator<int_deque_type>;
	template class gstl::front_insert_iterator<int_deque_type>;
	template class gstl::insert_iterator<int_deque_type>;

	template class gstl::istream_iterator<int>;
	template class gstl::ostream_iterator<int>;
	template class gstl::istreambuf_iterator<char>;
	template class gstl::ostreambuf_iterator<char>;

BOOST_AUTO_TEST_CASE( test_insert_iterators )
{
	typedef gstl::back_insert_iterator<int_deque_type>	back_insert_iterator;
	typedef gstl::front_insert_iterator<int_deque_type>	front_insert_iterator;
	typedef gstl::insert_iterator<int_deque_type>		insert_iterator;

	int_deque_type int_deque;
	back_insert_iterator back_iter = gstl::back_inserter( int_deque );
	front_insert_iterator front_iter = gstl::front_inserter( int_deque );
	insert_iterator insert_iter = gstl::inserter( int_deque, int_deque.begin() );
}

BOOST_AUTO_TEST_CASE( test_stream_iterators )
{
	//////////////////////////////////////////////////////////////////////////
	//This instantiations produce CRT SECURE warning messages (Tested on VC 9.0)
	std::basic_istringstream<char, gstl::char_traits<char> > istr;
	//////////////////////////////////////////////////////////////////////////
	std::basic_ostringstream<char, gstl::char_traits<char> > ostr;


	typedef gstl::char_traits<char> traits;
	gstl::istream_iterator<int, char, traits>	istream_iter( istr );
	gstl::istreambuf_iterator<char, traits>		istreambuf_iter( istr );
	gstl::istreambuf_iterator<char, traits>		istreambuf_iter2( istr.rdbuf() );
	gstl::ostream_iterator<int, char, traits>	ostream_iter( ostr );
	gstl::ostreambuf_iterator<char, traits>		ostreambuf_iter( ostr );
	gstl::ostreambuf_iterator<char, traits>		ostreambuf_iter2( ostr.rdbuf() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_iterator2, type, t_list )
{

}

BOOST_AUTO_TEST_SUITE_END();




