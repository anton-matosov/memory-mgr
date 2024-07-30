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


#include "memory-mgr/detail/math.h"
#include <boost/type_traits/make_unsigned.hpp>

BOOST_AUTO_TEST_SUITE( test_math_module )


	typedef boost::mpl::list< char, unsigned char, short, unsigned short, int, 
		unsigned int, long, unsigned long, long long, unsigned long long, size_t> test_types_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_lowest_bit, test_type, test_types_list )
	{
		enum
		{
			numBytes = sizeof( test_type ),
			bitsInByte = 8,
			numBits = numBytes * bitsInByte
		};

		for( int i = 0; i < numBits; ++i )
		{
			test_type var = test_type(1) << i;
			BOOST_CHECK_EQUAL( memory_mgr::math::lowest_bit<boost::make_unsigned<test_type>::type >( var ), i );
			BOOST_REQUIRE_EQUAL( memory_mgr::math::lowest_bit2( var ), i );
		}
	}

BOOST_AUTO_TEST_SUITE_END();
