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

#ifndef MGR_STD_AFX_UNIT_HEADER
#define MGR_STD_AFX_UNIT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

//#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW


#include <boost/test/test_tools.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/mpl/list.hpp>


#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <memory-mgr/config/config.h>


static const size_t sz_null = 0;
static const size_t sz_one = 1;
static const size_t sz_two = 2;
static const size_t sz_three = 3;
static const size_t sz_four = 4;
static const size_t sz_five = 5;
static const size_t sz_six = 6;
static const size_t sz_seven = 7;
static const size_t sz_eight = 8;
static const size_t sz_nine = 9;
static const size_t sz_ten = 10;


#endif// MGR_STD_AFX_UNIT_HEADER

