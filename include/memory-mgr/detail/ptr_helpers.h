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

#include <memory-mgr/detail/assert.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/config/config.h>

namespace memory_mgr
{
	namespace detail
	{
		template<class MemMgr>
		static inline void* offset_to_pointer( typename manager_traits<MemMgr>::block_offset_type offset, MemMgr& mgr )
		{
			return mgr.offset_to_pointer( offset );
		}


		template<class MemMgr>
		static inline typename manager_traits<MemMgr>::block_offset_type pointer_to_offset( const void* ptr, MemMgr& mgr )
		{
			return mgr.pointer_to_offset( ptr );
		}
	}
}
