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

#ifndef MGR_POINTER_TRATS_HEADER
#define MGR_POINTER_TRATS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/offset_pointer.h>

namespace memory_mgr
{
	//Offset pointer traits class
 	template<class T, class MemMgr>
 	struct pointer_traits
 	{
		typedef MemMgr								manager_type;
		typedef T									value_type;
		typedef pointer_traits< value_type, manager_type >		self_type;

		typedef				offset_pointer< value_type, manager_type >					pointer;
		typedef				const offset_pointer< value_type, manager_type >			const_pointer;
		typedef typename	offset_pointer< value_type, manager_type >::reference		reference;
		typedef	typename	offset_pointer< value_type, manager_type >::const_reference	const_reference;

		template<class Other>
		struct rebind
		{	// convert an pointer_traits<T> to an pointer_traits <Other>
			typedef typename pointer_traits< Other, manager_type > other;
		};

		static pointer null_ptr;

		static inline bool is_null( const_pointer ptr )
		{
			return ptr.is_null();
		}
 	};
 
 	template< class T, class MemMgr >
 	typename pointer_traits<T,MemMgr>::pointer pointer_traits<T,MemMgr>::null_ptr = typename pointer_traits< T, MemMgr >::pointer();

		
}


#endif// MGR_POINTER_TRATS_HEADER
