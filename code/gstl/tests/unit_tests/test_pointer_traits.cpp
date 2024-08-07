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

#include <boost/test/unit_test.hpp>
#include "test_common.hpp"

#include <memory-mgr/offset_ptr.h>
#include <gstl/detail/pointer_traits.hpp>
#include <boost/type_traits.hpp>

namespace
{
	using memory_mgr::offset_ptr;
	typedef boost::mpl::list< int, const int, volatile int,
		int*, const int*, const int* const,
		volatile int*, int* volatile, volatile int* volatile,
		const int* volatile, volatile int* const,
		offset_ptr<int>, offset_ptr<const int>, offset_ptr<volatile int> >  pointer_types_list;
}

BOOST_AUTO_TEST_SUITE( test_pointer_traits )


BOOST_AUTO_TEST_CASE_TEMPLATE( test_ptr_traits, ptr_type, pointer_types_list )
{
	typedef gstl::pointer_traits<ptr_type> ptr_traits;

	BOOST_STATIC_ASSERT( boost::is_pointer<typename ptr_traits::pointer>::value );
	BOOST_STATIC_ASSERT( boost::is_pointer<typename ptr_traits::const_pointer>::value );
	BOOST_STATIC_ASSERT( boost::is_reference<typename ptr_traits::reference>::value );
	BOOST_STATIC_ASSERT( boost::is_reference<typename ptr_traits::const_reference>::value );

	BOOST_STATIC_ASSERT( ! boost::is_const<typename ptr_traits::value_type>::value );
	BOOST_STATIC_ASSERT( ! boost::is_volatile<typename ptr_traits::value_type>::value );
	//BOOST_STATIC_ASSERT( boost::is_const<ptr_traits::const_reference>::value );
}

BOOST_AUTO_TEST_SUITE_END()
