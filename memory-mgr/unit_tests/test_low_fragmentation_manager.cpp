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
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/low_fragmentation_manager.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/allocator.h>
#include <memory-mgr/offset_allocator.h>
#include "common_manager_tests.h"
#include "managers.h"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>


BOOST_AUTO_TEST_SUITE( test_low_fragmentation_manager )

	BOOST_AUTO_TEST_CASE( calculate_pools_ids )
	{
		ULONGLONG prevSize = 0;
		size_t poolId = 0;

		for( size_t i = 0; i < 16384; i += 1 )
		{
			size_t segment_allocation_size = 1;
			size_t resulting_size = memory_mgr::detail::get_allocation_size( i, segment_allocation_size );

			if( resulting_size != prevSize )
			{
				if( prevSize != 0 )
				{
					++poolId;
				}
				prevSize = resulting_size;
			}

			BOOST_CHECK_EQUAL( memory_mgr::detail::get_pool_id(i), poolId );

		}
	}

	BOOST_AUTO_TEST_CASE( test_data_validness )
	{
		sz_lfm_heap_sz_mgr mgr;
		test::test_data_validness(mgr);
	}

BOOST_AUTO_TEST_SUITE_END()



