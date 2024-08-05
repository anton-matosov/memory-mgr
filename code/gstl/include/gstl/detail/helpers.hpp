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

namespace gstl
{
	/**
	   @brief Helper classes
	*/
	namespace helpers 
	{
		template<class StringT>
		static inline StringT& add_leading_slash( StringT& str )
		{
			if(str[0] != '/')
			{
				str.insert( str.begin(), '/');
			}
			return str;
		}

		template<class StringT>
		static inline StringT& add_trailing_slash( StringT& str )
		{
			if( str.length() >= 1 )
			{
				const typename StringT::traits_type::char_type last = *(str.end() - 1);
				if(last != '/' || last != '\\')
				{
					str.insert( str.end(), '/');
				}
			}
			return str;
		}

		template<class StringT> static inline StringT get_parent_dir( const StringT& file_path )
		{
			size_t pos = file_path.find_last_of( "\\/" );
			return file_path.substr( 0, (pos == -1 ? 0 : pos) );
		}


		template <class T> 
		inline void unused_variable(T const&) {}
	}//helpers

}//gstl


