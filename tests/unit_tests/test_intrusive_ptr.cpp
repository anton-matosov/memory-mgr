
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
#include "test_class.h"


#include <memory-mgr/smart_ptr/mgr_deleter.hpp>
#include <memory-mgr/smart_ptr/intrusive_ptr.hpp>
#include <memory-mgr/smart_ptr/intrusive_base.hpp>

#include "managers.h"
#include <memory-mgr/new.h>


// 
namespace
{
	typedef memory_mgr::named_objects<heap_sz_mgr> name_heap_mgr_type;
	typedef memory_mgr::singleton_manager<name_heap_mgr_type> sing_name_heap_mgr_type2;
}

MGR_WRAP_SINGLETON_MANAGER_CLASS( sing_mgr, sing_name_heap_mgr_type2 );

class TestInterusive
	:public memory_mgr::intrusive_base<TestInterusive, memory_mgr::mgr_deleter<sing_mgr> >
{

};

using memory_mgr::new_;

BOOST_AUTO_TEST_SUITE( test_intrusive_ptr )

BOOST_AUTO_TEST_CASE( test_create_delete )
{
	typedef memory_mgr::intrusive_ptr<TestInterusive> PtrType;
	PtrType ptr = new_<TestInterusive, sing_mgr>()();
	PtrType ptr2 = new_<TestInterusive, sing_mgr>()();

	PtrType ptr3 = ptr;
	PtrType ptr4 = ptr2;

	ptr = ptr2 = ptr3 = ptr4 = PtrType();
}


BOOST_AUTO_TEST_SUITE_END()