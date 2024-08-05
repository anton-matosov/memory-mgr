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

#pragma once

namespace memory_mgr
{
#define MGR_BIND_PARAM_NAME( name ) name##_binded_param

#define MGR_BIND_PARAM( name ) MGR_BIND_PARAM_NAME( name )<>

#define MGR_BINDED( T, name ) ::memory_mgr::params_binder< T, MGR_BIND_PARAM( name ) >

//This class should be a template class because otherwise it will be impossible
//to define static variable in the header file
#define MGR_DECLARE_BIND_PARAM( name, type, param_value )	\
	template<class T = type>								\
	struct MGR_BIND_PARAM_NAME( name ){ static T value; };	\
	template<class T>										\
	T MGR_BIND_PARAM_NAME( name )<T>::value = param_value;

	template<class T, class Par1>
	class params_binder
		:public T
	{
		typedef T base_type;
	public:
		params_binder()
			:base_type( Par1::value )
		{

		}
	};
}
