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


#include <iostream>
#include <string>

#define BOOST_ALL_NO_LIB
#ifdef max
#	undef max
#endif
#ifdef min
#	undef min
#endif
#include "test_common.hpp"

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

static const char m_test_str[];
static const char m_test_str2[];
static const size_t m_test_str_len;
static const size_t m_test_str_len2;