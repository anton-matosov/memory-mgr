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

#ifndef MGR_BLOCK_ID_CONVERTER_HEADER
#define MGR_BLOCK_ID_CONVERTER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/manager_traits.h>


namespace memory_mgr
{	
// 	/**
// 	   @brief Converter class which should be used to convert objects of block_id_type
// 	*/
// 	template<class MemMgr> 
// 	class block_id_converter
// 	{
// 	public:
// 		typedef		MemMgr		mgr_type;
// 		typedef	typename manager_traits<mgr_type>::block_id_type	block_id_type;
// 
// 		block_id_type	to_block_id( block_id_type id, mgr_type& /*mgr*/ )
// 		{
// 			return id;
// 		}
// 	};

}

#endif //MGR_BLOCK_ID_CONVERTER_HEADER