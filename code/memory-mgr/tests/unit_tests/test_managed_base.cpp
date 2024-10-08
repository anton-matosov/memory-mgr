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


#include <boost/test/unit_test.hpp>

class managed_base_fixture
{
public:
	static void verify_array_data( const test_class* arr )
	{
		BOOST_CHECK_MESSAGE( (arr[0].get() == 100) &&
			(arr[1].get() == 101) && 
			(arr[2].get() == 102) && 
			(arr[3].get() == 103) && 
			(arr[4].get() == 104) &&
			(arr[5].get() == 105) , "allocated memory data is corrupted" );
	}
};

BOOST_FIXTURE_TEST_SUITE(test_managed_base, managed_base_fixture);

	BOOST_AUTO_TEST_CASE( test_null_ptr )
	{
		test_class* t1 = 0;

		delete t1;
		delete[] t1;
	}

	BOOST_AUTO_TEST_CASE( test_managed_base )
	{
		test_class* t1 = new test_class();
		t1->set( 101 );

		const test_class* t2 = new const test_class( 102 );

		test_class* arr = new test_class[6];
		arr[0].set(100);
		arr[1] = *t1;
		arr[2] = *t2;
		arr[5].set(105);

		test_class* t3 = new test_class();
		t3->set( 103 );
		arr[3] = *t3;

		//test_class* t4 = new test_class();
		test_class* t4 = new test_class();
		t4->set( 104 );
		
		test_class* t5 = new test_class();
		t5->set( 104 );

 		arr[4] = *t4;
		
		BOOST_CHECK_MESSAGE( (t1->get() == 101)
			&& (t2->get() == 102)
			&& (t3->get() == 103)
			&& (t4->get() == 104), "allocated memory data is corrupted" );

		verify_array_data( arr );

		const test_class* const arr2 = new const test_class[4];

		BOOST_CHECK_MESSAGE( (t1->get() == 101)
			&& (t2->get() == 102)
			&& (t3->get() == 103)
			&& (t4->get() == 104), "allocated memory data is corrupted" );

		verify_array_data( arr );

		delete t2;
		verify_array_data( arr );

		delete t4;
		verify_array_data( arr );

		delete[] arr2;
		BOOST_CHECK_MESSAGE( (t1->get() == 101) && (t3->get() == 103),
			"allocated memory data is corrupted" );
		verify_array_data( arr );

		delete t1;
		verify_array_data( arr );

		delete t3;
		verify_array_data( arr );

		delete[] arr;

		//test_class::mem_mgr::instance().clear();
		//BOOST_CHECK( test_class::mem_mgr::instance().is_free() );
	}

BOOST_AUTO_TEST_SUITE_END();


