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
#include "test_case.h"
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/pointer_convert.h>
#include "test_class.h"

namespace
{


	typedef unsigned char chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 256;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
	typedef memory_mgr::pointer_convert<memmgr_type> pconv_type;

	//template class memory_mgr::pointer_convert<memmgr_type>;



	bool test_alloc_dealloc()
	{
		SUBTEST_START( L"pointer_convert" );
		std::vector<chunk_type> memory( memory_size );
		pconv_type mgr( &*memory.begin() );
		const memmgr_type::size_type obj_size = 4;
		void* p1 = mgr.allocate( obj_size );
		void* p2 = mgr.allocate( obj_size );
		void* p3 = mgr.allocate( obj_size );
		void* p4 = mgr.allocate( obj_size );
		void* p5 = mgr.allocate( obj_size );


		mgr.deallocate( p2, obj_size );
		mgr.deallocate( p3, obj_size );
		mgr.deallocate( p1, obj_size );
		mgr.deallocate( p4, obj_size );
		mgr.deallocate( p5, obj_size );


		SUBTEST_END( mgr.is_free() );
	}

	bool test_null_ptr()
	{
		SUBTEST_START( L"deallocation of null ptr" );
		std::vector<chunk_type> memory( memory_size );
		pconv_type mgr( &*memory.begin() );

		mgr.deallocate( 0, 0 );

		SUBTEST_SUCCEDED;
	}
}

bool test_pointer_convert()
{
	TEST_START( L"pointer_convert decorator" );

	TEST_END( test_alloc_dealloc()
		&& test_null_ptr()
		);
}

