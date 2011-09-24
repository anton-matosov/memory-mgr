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
#include "managers.h"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>

typedef memory_mgr::low_fragmentation_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::memory_manager<size_t, 1234 * 321, 4> 
	> 
> lfm_mgr;

MGR_DECLARE_MANAGER_CLASS( low_fragmentation_mgr, lfm_mgr );

static size_t get_allocation_size( size_t requested_size, size_t& poolId )
{
	const size_t allocation_segments[] = { 256, 512, 1024, 2048, 4096, 8196, 16384 };
	size_t resulting_size = requested_size;
	poolId = 0;

	for( int segment = 0; segment < _countof( allocation_segments ); ++segment )
	{
		if( requested_size <= allocation_segments[segment] )
		{
			const int multiple = allocation_segments[segment] / 32;

			size_t sizeLeft = (requested_size + multiple - 1);
			int negativeMultiple = (-multiple);
			resulting_size = sizeLeft & negativeMultiple;
			poolId = resulting_size / multiple;

 			std::cout << std::hex
 				<< requested_size
 				<< ' ' << allocation_segments[segment]
 				<< ' ' << multiple
 				<< ' ' << sizeLeft
 				<< ' ' << negativeMultiple 
 				<< ' ' << resulting_size 
 				<< ' ' << poolId 
				<< ' ' << sizeLeft / multiple
				<< ' ' << (sizeLeft / multiple) * (segment + 1)
 				<< '\n';

			assert( poolId == (sizeLeft / multiple) );
			break;
		}
	}
	return resulting_size;
}

struct SegmentInfo
{
	size_t maxBlockSize;
	size_t multiple;
	size_t numPools;
	size_t firstPoolId;
};

size_t *poolIds;

BOOST_AUTO_TEST_SUITE( test_low_fragmentation_manager )

	BOOST_AUTO_TEST_CASE( calculate_pools_sizes_and_count )
	{
		const size_t allocation_segments[] = { 256, 512, 1024, 2048, 4096, 8196, 16384 };

		std::cout << "\n";

		std::ofstream pool_by_size( "E:\\projects\\memory-mgr\\pool_by_size" );

		size_t itemsOnRow = 20;
		size_t itemsPrinted = 0;
		{
			size_t prevSize = 0;
			size_t poolId = 0;
			poolIds = new size_t[16384];

			//for index 0
			for( size_t i = 0; i < 16384; i += 1 )
			{
				for( int segment = 0; segment < _countof( allocation_segments ); ++segment )
				{
					if( i <= allocation_segments[segment] )
					{
						const int multiple = allocation_segments[segment] / 32;
						size_t resulting_size = memory_mgr::detail::make_multiple_of( i, multiple );
						if( resulting_size != prevSize )
						{
							if( prevSize != 0 )
							{
								++poolId;
							}
							prevSize = resulting_size;
						}
						break;
					}
				}

				pool_by_size << poolId << ", ";
				poolIds[i] = poolId;

				if( ++itemsPrinted == itemsOnRow )
				{
					pool_by_size << '\n';
					itemsPrinted = 0;
				}
			}
		}

		size_t totalPoolsRequired = 0;
		size_t prevSegmentSize = 0;
		std::cout << "\ninSegmentSize\tMultiple\tNum pools required\n";
		for( int segment = 0; segment < _countof( allocation_segments ); ++segment )
		{
			size_t segmentSize = allocation_segments[segment];
			const int multiple = segmentSize / 32;
			
			size_t inSegmentSize = segmentSize - prevSegmentSize;

			size_t poolsRequired = inSegmentSize / multiple;
			std::cout << inSegmentSize << '\t' << multiple << '\t'<< poolsRequired << '\n';

			prevSegmentSize = segmentSize;
			totalPoolsRequired += poolsRequired;
		}

		std::cout << "Total pools required: " << totalPoolsRequired << '\n';

		size_t prevSize = 0;
		for( int i = 0; i < 16384; i += 1 )
		{
			size_t poolId = 0;
			size_t requested_size = i;
			const size_t allocation_segments[] = { 256, 512, 1024, 2048, 4096, 8196, 16384 };
			size_t resulting_size = requested_size;

			for( int segment = 0; segment < _countof( allocation_segments ); ++segment )
			{
				if( requested_size <= allocation_segments[segment] )
				{
					const int multiple = allocation_segments[segment] / 32;

					size_t sizeLeft = (requested_size + multiple - 1);
					int negativeMultiple = (-multiple);
					resulting_size = sizeLeft & negativeMultiple;
					poolId = resulting_size / multiple;

					//if( resulting_size != prevSize )
					{
					//	std::cout //<< std::hex
//							<< requested_size
// 							<< ' ' << allocation_segments[segment]
// 							<< ' ' << multiple
// 							<< ' ' << sizeLeft
// 							<< ' ' << negativeMultiple 
// 							<< ' ' << resulting_size 
// 							<< ' ' << poolId 
// 							<< ' ' << sizeLeft / multiple
					//		<< ' ' << (sizeLeft / multiple) * (segment + 1)
					//		<< '\n';

					}

					assert( poolId == (sizeLeft / multiple) );

					prevSize = resulting_size;
					break;
				}
			}




			//size_t currSize = get_allocation_size(i, poolId);

//			std::cout << i << '\t' << poolId << '\n';
// 			if( currSize != prevSize )
// 			{
// 				std::cout << currSize << '\t' << poolId << '\n';
// 				prevSize = currSize;
// 			}
		}
	}

BOOST_AUTO_TEST_SUITE_END()



