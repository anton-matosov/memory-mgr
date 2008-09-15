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

#ifndef MGR_TEST_CLASS_UNIT_HEADER
#define MGR_TEST_CLASS_UNIT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/managed_base.h>
#include <memory-mgr/named_objects.h>
#include <memory-mgr/memory_manager.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/singleton_manager.h>
#include <memory-mgr/pointer_convert.h>


typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::named_objects
		< 
			memory_mgr::pointer_convert
			< 
				memory_mgr::memory_manager<size_t, 1024 * 1024, 4> 
			>
		>
	>
>	def_heap_mgr;

class test_class: public memory_mgr::managed_base< def_heap_mgr >
{
	int m_i,m_i1,m_i2,m_i3,m_i4;
public:
	typedef memory_mgr::managed_base< def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	test_class();
	~test_class();

	void set( int i );
	int get();
};

#endif //MGR_TEST_CLASS_UNIT_HEADER
