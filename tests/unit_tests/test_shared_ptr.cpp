
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


#include "test_class.h"


#include "memory-mgr/smart_ptr/shared_ptr.hpp"
#include "memory-mgr/smart_ptr/smart_ptr/weak_ptr.hpp"
#include "memory-mgr/smart_ptr/scoped_ptr.hpp"
#include "memory-mgr/smart_ptr/scoped_array.hpp"


#include "memory-mgr/smart_ptr/mgr_deleter.hpp"
#include "memory-mgr/smart_ptr/make_shared.hpp"
#include "managers.h"
#include "memory-mgr/new.h"
//template class memory_mgr::scoped_ptr<int>;
//template class memory_mgr::scoped_array<int>;
template class memory_mgr::shared_ptr<int>;
template class memory_mgr::weak_ptr<int>;
// memory_mgr::scoped_ptr<int> scptr;
// memory_mgr::scoped_array<int> scarr;
// 
namespace
{
	typedef memory_mgr::named_objects<heap_sz_mgr> name_heap_mgr_type;

	typedef memory_mgr::singleton_manager<name_heap_mgr_type> sing_name_heap_mgr_type2;
}
MGR_DECLARE_MANAGER_CLASS( name_heap_mgr, name_heap_mgr_type );
MGR_WRAP_SINGLETON_MANAGER_CLASS( sing_mgr, sing_name_heap_mgr_type2 );


BOOST_AUTO_TEST_SUITE( test_shared_ptr )

BOOST_AUTO_TEST_CASE( test_create_delete )
{
 	memory_mgr::shared_ptr<int> int_pda( memory_mgr::new_<int, sing_mgr>()(),
 		memory_mgr::mgr_deleter<sing_mgr>(), memory_mgr::allocator<int, sing_mgr>() );
 	*int_pda = 10;
 
 	memory_mgr::weak_ptr<int> weak_int = int_pda;
 	memory_mgr::shared_ptr<int> int_pda2( weak_int.lock() );
}

BOOST_AUTO_TEST_CASE( test_make_shared_singleton )
{
	typedef sing_mgr mem_mgr;
	using memory_mgr::make_shared;
	using memory_mgr::shared_ptr;

	shared_ptr<int> int_p  = make_shared<int, mem_mgr>();
	shared_ptr<int> int_p1 = make_shared<int, mem_mgr>( 123 );
	shared_ptr<int> int_p2 = make_shared<int, mem_mgr>( 123, 123 );
	shared_ptr<int> int_p3 = make_shared<int, mem_mgr>( 123, 123, 123 );
	shared_ptr<int> int_p4 = make_shared<int, mem_mgr>( 123, 123, 123, 123 );
	shared_ptr<int> int_p5 = make_shared<int, mem_mgr>( 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p6 = make_shared<int, mem_mgr>( 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p7 = make_shared<int, mem_mgr>( 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p8 = make_shared<int, mem_mgr>( 123, 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p9 = make_shared<int, mem_mgr>( 123, 123, 123, 123, 123, 123, 123, 123, 123 );
}

BOOST_AUTO_TEST_CASE( test_make_shared )
{
	name_heap_mgr mem_mgr;
	using memory_mgr::make_shared;
	using memory_mgr::shared_ptr;

	shared_ptr<int> int_p  = make_shared<int>(mem_mgr);
	shared_ptr<int> int_p1 = make_shared<int>(mem_mgr, 123 );
	shared_ptr<int> int_p2 = make_shared<int>(mem_mgr, 123, 123 );
	shared_ptr<int> int_p3 = make_shared<int>(mem_mgr, 123, 123, 123 );
	shared_ptr<int> int_p4 = make_shared<int>(mem_mgr, 123, 123, 123, 123 );
	shared_ptr<int> int_p5 = make_shared<int>(mem_mgr, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p6 = make_shared<int>(mem_mgr, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p7 = make_shared<int>(mem_mgr, 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p8 = make_shared<int>(mem_mgr, 123, 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p9 = make_shared<int>(mem_mgr, 123, 123, 123, 123, 123, 123, 123, 123, 123 );
}

BOOST_AUTO_TEST_CASE( test_allocate_shared )
{
	using memory_mgr::allocate_shared;
	using memory_mgr::shared_ptr;
	memory_mgr::allocator<int, sing_mgr> alloc;

	shared_ptr<int> int_p  = allocate_shared<int>( alloc );
	shared_ptr<int> int_p1 = allocate_shared<int>( alloc, 123 );
	shared_ptr<int> int_p2 = allocate_shared<int>( alloc, 123, 123 );
	shared_ptr<int> int_p3 = allocate_shared<int>( alloc, 123, 123, 123 );
	shared_ptr<int> int_p4 = allocate_shared<int>( alloc, 123, 123, 123, 123 );
	shared_ptr<int> int_p5 = allocate_shared<int>( alloc, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p6 = allocate_shared<int>( alloc, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p7 = allocate_shared<int>( alloc, 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p8 = allocate_shared<int>( alloc, 123, 123, 123, 123, 123, 123, 123, 123 );
	shared_ptr<int> int_p9 = allocate_shared<int>( alloc, 123, 123, 123, 123, 123, 123, 123, 123, 123 );
}

BOOST_AUTO_TEST_SUITE_END()
