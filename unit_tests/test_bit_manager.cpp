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


#include "stdafx.h"
#include <memory-mgr/detail/bit_manager.h>

typedef char block_type;
static const size_t bits_count = 32;
typedef memory_mgr::detail::bit_manager<block_type, bits_count> bit_manager;
typedef bit_manager::bit_position_type ptr_type;

BOOST_AUTO_TEST_SUITE( test_bit_manager )


	BOOST_AUTO_TEST_CASE( test_bit_manager )
	{	
		bit_manager bitmgr;

		ptr_type p0 = bitmgr.allocate(0);
		BOOST_CHECK_EQUAL( p0, bit_manager::npos );

		ptr_type p1 = bitmgr.allocate(1);
		BOOST_CHECK_EQUAL(  p1, 0U );

		ptr_type p2 = bitmgr.allocate(18);
		BOOST_CHECK_EQUAL(  p2, 1U );
		bitmgr.deallocate(p2, 18);

		p2 = bitmgr.allocate(1);
		BOOST_CHECK_EQUAL(  p2, 1U );
		bitmgr.deallocate(p2, 1);

		bitmgr.deallocate(p1, 1);
		p1 = bitmgr.allocate(1);
		BOOST_CHECK_EQUAL(  p1, 0U );
		bitmgr.deallocate(p1, 1);
	}

BOOST_AUTO_TEST_SUITE_END();

