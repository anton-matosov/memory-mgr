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

#include <assert.h>
#include <stdexcept>

#ifndef GSTL_COMA
#	define GSTL_COMA ,
#endif

#ifndef GSTL_DEBUG_EXPRESSION
#	ifdef GSTL_DEBUG
#		define GSTL_DEBUG_EXPRESSION( expression ) expression
#	else
#		define GSTL_DEBUG_EXPRESSION( expression )
#	endif
#endif


#define GSTL_DO_STRINGIZE( __expr__ ) #__expr__
#define GSTL_STRINGIZE( __expr__ ) GSTL_DO_STRINGIZE( __expr__ )

#ifdef GSTL_ASSERT_EXCEPTION
namespace gstl
{
	class assert_exception
		:public std::logic_error
	{
	public:
		assert_exception( const char* message )
			:std::logic_error( message )
		{

		}
	};
}
#endif

#ifndef GSTL_ASSERT
#	if defined(GSTL_DEBUG) && !defined(GSTL_ASSERT_EXCEPTION)
#		define GSTL_ASSERT( expression ) GSTL_DEBUG_EXPRESSION( assert( expression ) )
#	elif defined( GSTL_ASSERT_EXCEPTION )
#		define GSTL_ASSERT( expression ) if( !(expression) )\
								{ throw gstl::assert_exception( GSTL_STRINGIZE(expression) ); }
#	else
#		define GSTL_ASSERT( expression )
#	endif
#endif

#define GSTL_NO_THROW

