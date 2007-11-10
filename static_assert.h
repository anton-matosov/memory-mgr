
#pragma once

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


