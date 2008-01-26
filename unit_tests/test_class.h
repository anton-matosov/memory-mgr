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

#ifndef MGR_TEST_CLASS_UNIT_HEADER
#define MGR_TEST_CLASS_UNIT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "managed_base.h"
#include "heap_memory.h"

class TestClass: public memory_mgr::managed_base< memory_mgr::def_heap_mgr >
{
	int m_i;
public:
	typedef memory_mgr::managed_base< memory_mgr::def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	TestClass();
	~TestClass();

	void set( int i );
	int get();
};

#endif //MGR_TEST_CLASS_UNIT_HEADER
