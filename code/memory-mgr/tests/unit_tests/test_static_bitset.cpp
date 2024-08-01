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

#include "memory-mgr/detail/static_bitset.h"
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

typedef memory_mgr::static_bitset<unsigned char, 32, memory_mgr::static_array> bitset_uchar_32_type;
typedef memory_mgr::static_bitset<unsigned short, 64, memory_mgr::static_array> bitset_ushort_64_type;
typedef memory_mgr::static_bitset<unsigned int, 128, memory_mgr::static_array> bitset_uint_128_type;
typedef memory_mgr::static_bitset<unsigned long, 128, memory_mgr::static_array> bitset_ulong_128_type;
typedef memory_mgr::static_bitset<unsigned long long, 256, memory_mgr::static_array> bitset_ulonglong_256_type;

typedef boost::mpl::list<bitset_uchar_32_type, bitset_ushort_64_type, bitset_uint_128_type, bitset_ulong_128_type, bitset_ulonglong_256_type> bitsets_list;

BOOST_AUTO_TEST_SUITE(test_static_bitset)

BOOST_AUTO_TEST_CASE_TEMPLATE(static_bitset_positive_cases, bitset_type, bitsets_list)
{
  bitset_type bitset;
  const typename bitset_type::size_type bits_count = bitset_type::num_bits;
  bitset.reset();

  BOOST_CHECK_EQUAL(bitset.size(), bits_count);
  BOOST_CHECK_EQUAL(bitset.test(0), false);

  bitset.set(0);
  BOOST_CHECK_EQUAL(bitset.test(0), true);

  bitset.set(1, 3);
  BOOST_CHECK_EQUAL(bitset.test(1, 3), true);

  bitset.set(6, 5);
  BOOST_CHECK_EQUAL(bitset.test(6, 5), true);

  typename bitset_type::bit_position_type pos = bitset.find_first();
  BOOST_CHECK_EQUAL(pos, 0);

  pos = bitset.find_next(pos);
  BOOST_CHECK_EQUAL(pos, 1);

  pos = bitset.find_n(5);
  BOOST_CHECK_EQUAL(pos, 6);

  bitset.flip(0);
  BOOST_CHECK_EQUAL(bitset.test(0), false);

  bitset.reset(1);
  BOOST_CHECK_EQUAL(bitset.test(1), false);

  bitset.reset(2, 3);
  BOOST_CHECK_EQUAL(bitset.test(2, 3), false);

  bitset.reset(pos, 5);
  BOOST_CHECK_EQUAL(bitset.test(pos, 5), false);

  bitset.set(0, bitset_type::num_bits);
  BOOST_CHECK_EQUAL(bitset.test(0, bitset_type::num_bits), true);

  constexpr auto bitset_npos = bitset_type::npos;  // Needed to keep npos inline as pass by reference will require a storage definition

  bitset.set(0, bitset_npos);
  BOOST_CHECK_EQUAL(bitset.test(0, bitset_npos), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_mask_calculation_on_bounds, bitset_type, bitsets_list)
{
  bitset_type bitset;
  constexpr auto bitset_npos = bitset_type::npos;

  bitset.reset(0, bitset.size());
  BOOST_CHECK_EQUAL(bitset.find_n(1), bitset_npos);
}

BOOST_AUTO_TEST_SUITE_END();
