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

#pragma once

#include "memory-mgr/managed_base.h"
#include "memory-mgr/named_objects.h"
#include "memory-mgr/memory_manager.h"
#include "memory-mgr/heap_segment.h"
#include "memory-mgr/singleton_manager.h"
#include "memory-mgr/size_tracking.h"
#include "memory-mgr/params_binder.h"
#include "memory-mgr/sync/named_mutex.h"

MGR_DECLARE_BIND_PARAM( MutexName, const char*, "Default memory sync" );
typedef MGR_BINDED( memory_mgr::sync::named_mutex, MutexName ) def_named_mutex;


struct my_mutex
	:public memory_mgr::sync::named_mutex
{
	my_mutex()
		:memory_mgr::sync::named_mutex( "mutex name" )
	{

	}
};

typedef memory_mgr::singleton_manager
< 
	memory_mgr::named_objects
	< 
		memory_mgr::heap_segment
		< 
			memory_mgr::size_tracking
			< 
				memory_mgr::memory_manager<size_t, 1024 * 1024, 4, def_named_mutex> 
			>
		>
	>
>	def_heap_mgr;

class test_class
	: public memory_mgr::managed_base< def_heap_mgr >
{
	int m_i,
		m_i1,
		m_i2,
		m_i3,
		m_i4;
public:
	typedef memory_mgr::managed_base< def_heap_mgr > base_t;
	typedef base_t::mem_mgr mem_mgr;

	test_class( int i = 0 );
	test_class( int i, int j );
	test_class( int i, int j, int& ref );
	~test_class();

	void set( int i );
	int get() const;
};

typedef int builtin_type;

class base_test_class 
	: public memory_mgr::managed_base<def_heap_mgr>
{
	int i_;
public:
	base_test_class( int i = 0 )
		:i_(i)
	{}

	virtual ~base_test_class()
	{
		i_ = 0xB;
	}

	int Get() const { return i_; }
	void Set( int i ){ i_ = i; }
};

class derived_test_class 
	: public base_test_class
{
	int i2_;
	int i3_;
	int i4_;
	int i5_;
public:
	derived_test_class( int i = 0 )
		:base_test_class(i),
		i2_(i + 1)
	{}

	~derived_test_class()
	{
		Set( 0xD );
		i2_ = 0xD;
		i3_ = 0xD;
		i4_ = 0xD;
		i5_ = 0xD;
	}
};
