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

#ifndef MGR_PTR_HELPERS_HEADER
#define MGR_PTR_HELPERS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <assert.h>
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

		template<class T, class MemMgr>
		static inline typename boost::remove_pointer< typename boost::remove_const<T>::type >::type*
			to_pointer( typename manager_traits<MemMgr>::block_offset_type offset, MemMgr& mgr )
		{
			return static_cast<typename boost::remove_pointer< typename boost::remove_const<T>::type >::type*>(
				mgr.offset_to_pointer( offset ) );
		}

		template<class T, class MemMgr>
		static inline typename boost::remove_pointer< typename boost::remove_const<T>::type >::type*
			to_pointer( typename boost::remove_pointer< typename boost::remove_const<T>::type >::type* ptr, MemMgr& /*mgr*/ )
		{
			return ptr;
		}

		template<class T, class MemMgr>
		static inline const typename boost::remove_pointer< typename boost::remove_const<T>::type >::type*
			to_pointer( const typename boost::remove_pointer< typename boost::remove_const<T>::type >::type* ptr, MemMgr& /*mgr*/ )
		{
			return ptr;
		}

		template<class MemMgr>
		static inline typename manager_traits<MemMgr>::block_offset_type pointer_to_offset( const void* ptr, MemMgr& mgr )
		{
			return mgr.pointer_to_offset( ptr );
		}

		template<class MemMgr>
		static inline typename manager_traits<MemMgr>::block_offset_type to_offset( const void* ptr, MemMgr& mgr )
		{
			return mgr.pointer_to_offset( ptr );
		}

		template<class MemMgr>
		static inline typename manager_traits<MemMgr>::block_offset_type to_offset( typename manager_traits<MemMgr>::block_offset_type ptr,
			MemMgr& /*mgr*/ )
		{
			return ptr;
		}

	}// namespace detail

}// namespace memory_mgr

#endif //MGR_PTR_HELPERS_HEADER
