/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
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


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#include "StdAfx.h"
#include <memory-mgr/detail/static_bitset.h>

typedef memory_mgr::static_bitset<unsigned char, 32, memory_mgr::static_array> bitset_uchar_32_type;

typedef boost::mpl::list< bitset_uchar_32_type > bitsets_list;

BOOST_AUTO_TEST_SUITE( test_static_bitset )

	BOOST_AUTO_TEST_CASE_TEMPLATE( static_bitset, bitset_type, bitsets_list )
	{
		bitset_type bitset;
		const typename bitset_type::size_type bits_count = bitset_type::num_bits;
		bitset.reset();

		BOOST_CHECK_EQUAL( bitset.size(), bits_count );
		BOOST_CHECK_EQUAL( bitset.test(0), false );

		bitset.set(0);
		BOOST_CHECK_EQUAL(  bitset.test(0), true );

		bitset.set(1,3);
		BOOST_CHECK_EQUAL(  bitset.test(1,3), true );

		bitset.set(6,5);
		BOOST_CHECK_EQUAL(  bitset.test(6,5), true );

		typename bitset_type::size_type pos = bitset.find_first();
		BOOST_CHECK_EQUAL(  pos, sz_null );

		pos = bitset.find_next(pos);
		BOOST_CHECK_EQUAL(  pos, sz_one );

		pos = bitset.find_n(5);
		BOOST_CHECK_EQUAL(  pos, sz_six );

		bitset.flip(0);
		BOOST_CHECK_EQUAL(  bitset.test(0), false );

		bitset.reset(1);
		BOOST_CHECK_EQUAL(  bitset.test(1), false );

		bitset.reset(2,3);
		BOOST_CHECK_EQUAL( bitset.test(2,3), false );


		bitset.reset( pos, 5 );
		BOOST_CHECK_EQUAL(  bitset.test( pos, 5 ), false );

		bitset.set( 0, bitset_type::num_bits );
		BOOST_CHECK_EQUAL(  bitset.test(0, bitset_type::num_bits ), true );
	}

BOOST_AUTO_TEST_SUITE_END();


