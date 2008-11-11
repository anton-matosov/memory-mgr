/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#include "stdafx.h"

#include <gstl/vector>
#include <vector>



class vector_fixture
{
public:
	typedef gstl::vector<int> vector_type;
};

BOOST_FIXTURE_TEST_SUITE( vector_test, vector_fixture )

BOOST_AUTO_TEST_CASE( test_constructors )
{
	//Default constructor
	vector_type vec1;

	int arr[] = { 1, 2, 3 };

	//Iterators constructor
	vector_type vec2( arr, GSTL_ARRAY_END( arr ) );
}

BOOST_AUTO_TEST_SUITE_END()


