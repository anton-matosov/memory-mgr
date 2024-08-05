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


#pragma once

class test_class
{
	int i_;
public:

	test_class();
	test_class(int i);
	~test_class();

	void set( int i );
	int get() const;

	operator int();
};

std::ostream& operator<<( std::ostream& out, const test_class& val );

bool operator==( const test_class& lhs, const test_class& rhs );
bool operator!=( const test_class& lhs, const test_class& rhs );
bool operator<=( const test_class& lhs, const test_class& rhs );
bool operator>=( const test_class& lhs, const test_class& rhs );

bool operator<( const test_class& lhs, const test_class& rhs );
bool operator>( const test_class& lhs, const test_class& rhs );


