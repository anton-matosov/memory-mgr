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
#include "test_case.h"
#include "test_class.h"

struct Struct{};
union Union{};

class base_class
{};

class base_class2
{};

class derived_class: public base_class
{};

bool test_type_manip()
{
	TEST_START( L"types manipulation library" );

	memory_mgr::type_manip::int2type<1>::value;
	
	//////////////////////////////////////////////////////////////////////////
	// select<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing select<false, int, float>::result float_type" );
	typedef memory_mgr::type_manip::select<false, int, float>::result float_type;
	
	TEST_PRINT( L"Testing select<true, int, float>::result int_type" );
	typedef memory_mgr::type_manip::select<true, int, float>::result int_type;

	//////////////////////////////////////////////////////////////////////////
	// is_same_type<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing is_same_type<int, int_type>" );
	bool isame = memory_mgr::type_manip::is_same_type< int, int_type >::value;
	TEST_CHECK( isame );

	TEST_PRINT( L"Testing is_same_type<float, float_type>" );
	bool fsame = memory_mgr::type_manip::is_same_type< float, float_type >::value;
	TEST_CHECK( fsame );

	TEST_PRINT( L"Testing is_same_type<int_type, float_type>" );
	bool not_same = !memory_mgr::type_manip::is_same_type< int_type, float_type >::value;
	TEST_CHECK( not_same );

	//////////////////////////////////////////////////////////////////////////
	// is_class<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing is_class< test_class >" );
	bool test_class_isclass = memory_mgr::type_manip::is_class< test_class >::value;
	TEST_CHECK( test_class_isclass );

	TEST_PRINT( L"Testing is_class< Struct >" );
	bool struct_isclass = memory_mgr::type_manip::is_class< Struct >::value;
	TEST_CHECK( struct_isclass );

	TEST_PRINT( L"Testing is_class< int >" );
	bool int_isclass = memory_mgr::type_manip::is_class< int >::value;
	TEST_CHECK( !int_isclass );

	TEST_PRINT( L"Testing is_class< Union >" );
	bool union_isclass = memory_mgr::type_manip::is_class< Union >::value;
	TEST_CHECK( union_isclass );
	
	//////////////////////////////////////////////////////////////////////////
	// is_convertible<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing is_convertible<int, long>" );
	bool int_to_long = memory_mgr::type_manip::is_convertible<int, long>::exists;
	TEST_CHECK( int_to_long );

	TEST_PRINT( L"Testing is_convertible<long, int>" );
	bool long_to_int = memory_mgr::type_manip::is_convertible<long, int>::exists;
	TEST_CHECK( long_to_int );

	TEST_PRINT( L"Testing is_convertible<base_class, derived_class>" );
	bool base_to_der = memory_mgr::type_manip::is_convertible<base_class, derived_class>::exists;
	TEST_CHECK( !base_to_der );

	TEST_PRINT( L"Testing is_convertible<derived_class, base_class>" );
	bool der_to_base = memory_mgr::type_manip::is_convertible<derived_class, base_class>::exists;
	TEST_CHECK( der_to_base );

	//////////////////////////////////////////////////////////////////////////
	// super_subclass<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing super_subclass<base_class, derived_class>" );
	bool base_sub_der = memory_mgr::type_manip::super_subclass<base_class, derived_class>::value;
	TEST_CHECK( base_sub_der );

	TEST_PRINT( L"Testing super_subclass<derived_class, base_class>" );
	bool der_sub_base = memory_mgr::type_manip::super_subclass<derived_class, base_class>::value;
	TEST_CHECK( !der_sub_base );

	TEST_PRINT( L"Testing super_subclass<derived_class, base_class2>" );
	bool der_sub_base2 = memory_mgr::type_manip::super_subclass<derived_class, base_class2>::value;
	TEST_CHECK( !der_sub_base2 );
	
	TEST_PRINT( L"Testing super_subclass<base_class, base_class>" );
	bool base_sub_base = memory_mgr::type_manip::super_subclass<base_class, base_class>::value;
	TEST_CHECK( base_sub_base );

	TEST_PRINT( L"Testing super_subclass<base_class2, base_class2>" );
	bool base2_sub_base2 = memory_mgr::type_manip::super_subclass<base_class2, base_class2>::value;
	TEST_CHECK( base2_sub_base2 );

	TEST_PRINT( L"Testing super_subclass<base_class2, void*>" );
	bool base2_sub_void = memory_mgr::type_manip::super_subclass<base_class2, void>::value;
	TEST_CHECK( !base2_sub_void );
	
	//////////////////////////////////////////////////////////////////////////
	// super_subclass_strict<>
	//////////////////////////////////////////////////////////////////////////
	TEST_PRINT( L"Testing super_subclass_strict<base_class, derived_class>" );
	bool base_sub_der_strict = memory_mgr::type_manip::super_subclass_strict<base_class, derived_class>::value;
	TEST_CHECK( base_sub_der_strict );

	TEST_PRINT( L"Testing super_subclass_strict<derived_class, base_class>" );
	bool der_sub_base_strict = memory_mgr::type_manip::super_subclass_strict<derived_class, base_class>::value;
	TEST_CHECK( !der_sub_base_strict );

	TEST_PRINT( L"Testing super_subclass_strict<derived_class, base_class2>" );
	bool der_sub_base2_strict = memory_mgr::type_manip::super_subclass_strict<derived_class, base_class2>::value;
	TEST_CHECK( !der_sub_base2_strict );

	TEST_PRINT( L"Testing super_subclass_strict<base_class, base_class>" );
	bool base_sub_base_strict = memory_mgr::type_manip::super_subclass_strict<base_class, base_class>::value;
	TEST_CHECK( !base_sub_base_strict );

	TEST_PRINT( L"Testing super_subclass_strict<base_class2, base_class2>" );
	bool base2_sub_base2_strict = memory_mgr::type_manip::super_subclass_strict<base_class2, base_class2>::value;
	TEST_CHECK( !base2_sub_base2_strict );

	TEST_PRINT( L"Testing super_subclass_strict<base_class2, void*>" );
	bool base2_sub_void_strict = memory_mgr::type_manip::super_subclass_strict<base_class2, void>::value;
	TEST_CHECK( !base2_sub_void_strict );

	return true;						 
}
