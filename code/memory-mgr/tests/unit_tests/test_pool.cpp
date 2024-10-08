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



#include "tests_shared/managers.h"
#include "memory-mgr/pool/pool.hpp"


#include <boost/test/unit_test.hpp>

template class memory_mgr::pool<>;

typedef memory_mgr::memory_mgr_pool_allocator<sing_shared_sz_mgr> memory_mgr_pool_allocator;

BOOST_AUTO_TEST_SUITE( test_pool )


BOOST_AUTO_TEST_CASE( test_alloc_dealloc )
{
	enum { numCalls = 1000 };
	memory_mgr::pool<memory_mgr_pool_allocator> p( 4, 4 );

	int* ppp[numCalls];
 	for (int i = 0; i < numCalls; ++i)
 	{
 		ppp[i] = (int*)p.allocate();
 		*ppp[i] = i;
 	}
 
 	for (int i = 0; i < numCalls; ++i)
 	{
		BOOST_CHECK_EQUAL( *ppp[i], i );
 		p.deallocate( ppp[i] );
 	}

//	BOOST_CHECK( p.release_memory() );
}

BOOST_AUTO_TEST_SUITE_END();
