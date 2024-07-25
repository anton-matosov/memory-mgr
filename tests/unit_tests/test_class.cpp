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

#include "stdafx.h"
#include "test_class.h"

test_class::test_class( int i/* = 0*/ )
 :m_i(i)
{

}
test_class::test_class( int i, int j )
:m_i(i),
m_i1( j )
{

}
test_class::test_class( int i, int j, int& ref )
:m_i(i),
m_i1( j ),
m_i2( ref )
{

}

test_class::~test_class()
{
	m_i = 0;
}

void test_class::set( int i )
{
	m_i = i;
}

int test_class::get() const
{
	return m_i;
}
