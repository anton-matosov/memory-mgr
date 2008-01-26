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
#include "heap_memory.h"
#include "managed_base.h"

typedef unsigned char chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 256;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;
template memmgr_type;
template class memory_mgr::size_tracking< memmgr_type >;

typedef memmgr_type::ptr_type ptr_type;


class test_class: public memory_mgr::managed_base< memory_mgr::def_heap_mgr >
{
	int m_i;
public:
	typedef memory_mgr::managed_base< memory_mgr::def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	test_class()
		:m_i(0)
	{}
	
	~test_class()
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