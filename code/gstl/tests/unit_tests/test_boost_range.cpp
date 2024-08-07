/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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

#include <boost/test/unit_test.hpp>
#include "test_common.hpp"

#include <gstl/string>
#include <boost/range.hpp>

class boost_range_fixture
{
public:

};


BOOST_FIXTURE_TEST_SUITE( boost_range_test, boost_range_fixture )

BOOST_AUTO_TEST_CASE(test_make_range)
{
	gstl::string string = "hello range";
	boost::make_iterator_range(string);
	boost::make_iterator_range(string.begin(), string.end());

}

BOOST_AUTO_TEST_SUITE_END()
