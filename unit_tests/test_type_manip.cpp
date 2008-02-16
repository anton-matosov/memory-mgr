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


#include "stdafx.h"
#include <detail/type_manip.h>
#include "test_class.h"

struct Struct{};
union Union{};

bool test_type_manip()
{
	memory_mgr::type_manip::int2type<1>::value;
	typedef memory_mgr::type_manip::select<false, int, float>::result float_type;
	float_type fvar = float_type();
	fvar;

	typedef memory_mgr::type_manip::select<true, int, float>::result int_type;
	int_type ivar = int_type();
	ivar;

	bool isame = memory_mgr::type_manip::is_same_type< int, int_type >::value;
	bool fsame = memory_mgr::type_manip::is_same_type< float, float_type >::value;

	bool not_same = !memory_mgr::type_manip::is_same_type< int_type, float_type >::value;


	bool isclass = memory_mgr::type_manip::is_class< test_class >::value;
	
	bool isclass2 = memory_mgr::type_manip::is_class< Struct >::value;
	bool isclass3 = memory_mgr::type_manip::is_class< int >::value;
	
	bool isclass4 = memory_mgr::type_manip::is_class< Union >::value;

	return true;						 
}
