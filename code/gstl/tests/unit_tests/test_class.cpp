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


#include <utility>
#include <iostream>
#include "test_class.hpp"

test_class::test_class() :i_(0)
{}

test_class::test_class(int i) :i_(i)
{}

test_class::~test_class()
{
	i_ = 0;
}

void test_class::set( int i )
{
	i_ = i;
}

int test_class::get() const
{
	return i_;
}

test_class::operator int()
{
	return i_;
}

std::ostream& operator<<( std::ostream& out, const test_class& val )
{
	return out << val.get();
}

bool operator==( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() == rhs.get();
}

bool operator!=( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() != rhs.get();
}

bool operator<=( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() <= rhs.get();
}

bool operator>=( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() >= rhs.get();
}

bool operator<( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() < rhs.get();
}

bool operator>( const test_class& lhs, const test_class& rhs )
{
	return lhs.get() > rhs.get();
}
