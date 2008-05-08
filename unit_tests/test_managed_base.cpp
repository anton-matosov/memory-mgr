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
#include "test_case.h"
#include "test_class.h"

bool test_managed_base()
{
	TEST_START( L"managed_base" );
	test_class* t1 = new test_class();
	test_class* t2 = new test_class();
	test_class* t3 = new test_class();
	test_class* t4 = new test_class();
	
	test_class* arr = new test_class[5];

	t1->set( 101 );
	t2->set( 102 );
	t3->set( 103 );
	t4->set( 104 );

	arr[0].set(100);
	arr[1] = *t1;
	arr[2] = *t2;
	arr[3] = *t3;
	arr[4] = *t4;

	delete t2;
	delete t4;
	delete t1;
	delete t3;
	delete[] arr;

	TEST_END( test_class::mem_mgr::instance().free() );
}


