/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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


#include <vector>
#include "memory_manager.h"
#include "heap_memory_manager.h"
#include "managed_base.h"

typedef unsigned char chunk_t;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef managers::memory_manager<chunk_t, memory_size, chunk_size > memmgr_t;
template memmgr_t;
template class managers::size_tracking< memmgr_t >;

typedef memmgr_t::ptr_t ptr_t;


class TestClass: public managers::managed_base< managers::def_heap_mgr >
{
	int m_i;
public:
	typedef managers::managed_base< managers::def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	TestClass()
		:m_i(0)
	{}
	
	~TestClass()
	{
		m_i = 0;
	}

	void set( int i )
	{
		m_i = i;
	}

	int get()
	{
		return m_i;
	}
};

bool test_alloc_dealloc()
{

	return true;
}

bool test_size_tracking()
{
	return true;
}

bool test_out_of_memory()
{
	return true;
}

bool test_managed_base()
{
	return true;
}

bool test_memory_manager()
{
 	return test_alloc_dealloc() &&
	test_size_tracking() &&
	test_out_of_memory() &&
	test_managed_base()
	;
}