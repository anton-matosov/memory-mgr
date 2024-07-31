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



#include "memory-mgr/detail/type_manip.h"
#include "test_class.h"

struct Struct{};
union Union{};

class base_class
{};

class base_class2
{};

class derived_class: public base_class
{};

BOOST_AUTO_TEST_SUITE( test_type_manip )

	BOOST_AUTO_TEST_CASE( test_type_manip )
	{	
		memory_mgr::type_manip::int2type<1>::value;

		//////////////////////////////////////////////////////////////////////////
		// select<>
		//////////////////////////////////////////////////////////////////////////
		
		typedef memory_mgr::type_manip::select<false, int, float>::result float_type;		
		typedef memory_mgr::type_manip::select<true, int, float>::result int_type;

		//////////////////////////////////////////////////////////////////////////
		// is_same_type<>
		//////////////////////////////////////////////////////////////////////////
		
		bool isame = memory_mgr::type_manip::is_same_type< int, int_type >::value;
		BOOST_CHECK( isame );

		
		bool fsame = memory_mgr::type_manip::is_same_type< float, float_type >::value;
		BOOST_CHECK( fsame );

		
		bool not_same = !memory_mgr::type_manip::is_same_type< int_type, float_type >::value;
		BOOST_CHECK( not_same );

		//////////////////////////////////////////////////////////////////////////
		// is_class<>
		//////////////////////////////////////////////////////////////////////////
		
		bool test_class_isclass = memory_mgr::type_manip::is_class< test_class >::value;
		BOOST_CHECK( test_class_isclass );

		
		bool struct_isclass = memory_mgr::type_manip::is_class< Struct >::value;
		BOOST_CHECK( struct_isclass );

		
		bool int_isclass = memory_mgr::type_manip::is_class< int >::value;
		BOOST_CHECK( !int_isclass );

		
		bool union_isclass = memory_mgr::type_manip::is_class< Union >::value;
		BOOST_CHECK( union_isclass );

		//////////////////////////////////////////////////////////////////////////
		// is_convertible<>
		//////////////////////////////////////////////////////////////////////////
		
		bool int_to_long = memory_mgr::type_manip::is_convertible<int, long>::exists;
		BOOST_CHECK( int_to_long );
		
		bool long_to_int = memory_mgr::type_manip::is_convertible<long, int>::exists;
		BOOST_CHECK( long_to_int );
		
		bool base_to_der = memory_mgr::type_manip::is_convertible<base_class, derived_class>::exists;
		BOOST_CHECK( !base_to_der );
		
		bool der_to_base = memory_mgr::type_manip::is_convertible<derived_class, base_class>::exists;
		BOOST_CHECK( der_to_base );

		//////////////////////////////////////////////////////////////////////////
		// super_subclass<>
		//////////////////////////////////////////////////////////////////////////
		
		bool base_sub_der = memory_mgr::type_manip::super_subclass<base_class, derived_class>::value;
		BOOST_CHECK( base_sub_der );
		
		bool der_sub_base = memory_mgr::type_manip::super_subclass<derived_class, base_class>::value;
		BOOST_CHECK( !der_sub_base );
		
		bool der_sub_base2 = memory_mgr::type_manip::super_subclass<derived_class, base_class2>::value;
		BOOST_CHECK( !der_sub_base2 );
		
		bool base_sub_base = memory_mgr::type_manip::super_subclass<base_class, base_class>::value;
		BOOST_CHECK( base_sub_base );

		bool base2_sub_base2 = memory_mgr::type_manip::super_subclass<base_class2, base_class2>::value;
		BOOST_CHECK( base2_sub_base2 );

		bool base2_sub_void = memory_mgr::type_manip::super_subclass<base_class2, void>::value;
		BOOST_CHECK( !base2_sub_void );

		//////////////////////////////////////////////////////////////////////////
		// super_subclass_strict<>
		//////////////////////////////////////////////////////////////////////////
		bool base_sub_der_strict = memory_mgr::type_manip::super_subclass_strict<base_class, derived_class>::value;
		BOOST_CHECK( base_sub_der_strict );

		bool der_sub_base_strict = memory_mgr::type_manip::super_subclass_strict<derived_class, base_class>::value;
		BOOST_CHECK( !der_sub_base_strict );

		bool der_sub_base2_strict = memory_mgr::type_manip::super_subclass_strict<derived_class, base_class2>::value;
		BOOST_CHECK( !der_sub_base2_strict );

		bool base_sub_base_strict = memory_mgr::type_manip::super_subclass_strict<base_class, base_class>::value;
		BOOST_CHECK( !base_sub_base_strict );
		
		bool base2_sub_base2_strict = memory_mgr::type_manip::super_subclass_strict<base_class2, base_class2>::value;
		BOOST_CHECK( !base2_sub_base2_strict );
		
		bool base2_sub_void_strict = memory_mgr::type_manip::super_subclass_strict<base_class2, void>::value;
		BOOST_CHECK( !base2_sub_void_strict );
	}

BOOST_AUTO_TEST_SUITE_END();

