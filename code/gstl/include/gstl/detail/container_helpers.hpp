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

#include "gstl/detail/iterator.hpp"

namespace gstl
{
	namespace detail
	{
		/**
		   @brief 23 Containers library
		   @details page 470 (real pages 495, 497)
		*/
		namespace container
		{
			template<class Container, class T>
			void resize( Container* cont, typename Container::size_type newSize, T fillWith )
			{
				if( newSize > cont->size() )
				{
					cont->insert( cont->end(), newSize - cont->size(), fillWith );
				}
				else if( newSize < cont->size() )
				{
					typename Container::iterator it = cont->begin();
					gstl::advance( it, newSize );
					cont->erase( it, cont->end() );
				}
			}

			template<class Container>
			bool empty( Container* cont )
			{
				return !cont->size();
			}

			template<class Container>
			void clear( Container* cont )
			{
				cont->erase( cont->begin(), cont->end() );
			}

			template<class Container, class T>
			void push_front( Container* cont, const T& x )
			{
				cont->insert( cont->begin(), x );
			}

			template<class Container, class T>
			void push_back( Container* cont, const T& x )
			{
				cont->insert( cont->end(), x );
			}

			template<class Container>
			void pop_front( Container* cont )
			{
				cont->erase( cont->begin() );
			}

			template<class Container>
			void pop_back( Container* cont )
			{
				cont->erase( --cont->end() );
			}

			template<class Container>
			typename Container::reference front( Container* cont )
			{
				return *cont->begin();
			}

			template<class Container>
			typename Container::const_reference front( const Container* cont )
			{
				return *cont->begin();
			}

			template<class Container>
			typename Container::reference back( Container* cont )
			{
				return *--(cont->end());
			}

			template<class Container>
			typename Container::const_reference back( const Container* cont )
			{
				return *--(cont->end());
			}
		}
	}
}








