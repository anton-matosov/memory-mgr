
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

#include <memory-mgr/smart_ptr/shared_ptr.hpp>
#include <memory-mgr/smart_ptr/smart_ptr/weak_ptr.hpp>
#include <memory-mgr/smart_ptr/scoped_ptr.hpp>
#include <memory-mgr/smart_ptr/scoped_array.hpp>


#include <memory-mgr/offset_allocator.h>
#include "managers.h"
#include <memory-mgr/new.h>
// template class memory_mgr::scoped_ptr<int>;
// template class memory_mgr::scoped_array<int>;
template class memory_mgr::shared_ptr<int>;
template class memory_mgr::weak_ptr<int>;
// memory_mgr::scoped_ptr<int> scptr;
// memory_mgr::scoped_array<int> scarr;
// 
namespace
{
	typedef memory_mgr::singleton_manager
			< 
				memory_mgr::named_objects
				<
					heap_sz_mgr
				>
			>	sing_name_heap_mgr_type2;



}
MGR_DECLARE_MANAGER_CLASS( sing_name_heap_mgr2, sing_name_heap_mgr_type2 );

template<class MemMgr>
struct deleter_impl
{
	template<class T>
	void operator()( T* ptr )
	{
		(*ptr).~T();
		MemMgr::instance().deallocate( ptr );
	}
};

BOOST_AUTO_TEST_SUITE( test_shared_ptr )

BOOST_AUTO_TEST_CASE( test_create_delete )
{
	memory_mgr::shared_ptr<int> int_ptr( memory_mgr::new_<int, sing_name_heap_mgr2>()(),
		deleter_impl<sing_name_heap_mgr2>(), memory_mgr::offset_allocator<int, sing_name_heap_mgr2>() );
	*int_ptr = 10;

	memory_mgr::weak_ptr<int> weak_int = int_ptr;
	memory_mgr::shared_ptr<int> int_ptr2( weak_int.lock() );
}

BOOST_AUTO_TEST_SUITE_END()