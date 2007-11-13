/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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

#ifndef MGR_STATIC_ASSERT_HEADER
#define MGR_STATIC_ASSERT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace detail
{
	//Helper structures for the STATIC_ASSERT
	template<int> struct CompileTimeError;
    template<> struct CompileTimeError<true> {};
}

////////////////////////////////////////////////////////////////////////////////
// macro STATIC_ASSERT
// expr - a compile-time integral or pointer expression
// msg	- a C++ identifier that does not need to be defined
// If expr is zero, msg will appear in a compile-time error message.
////////////////////////////////////////////////////////////////////////////////

#define STATIC_ASSERT(expr, msg)\
	::detail::CompileTimeError<((expr) != 0)> ERROR_##msg;/* (void)ERROR_##msg;*/


#endif// MGR_STATIC_ASSERT_HEADER