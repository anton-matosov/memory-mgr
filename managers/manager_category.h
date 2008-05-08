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

#ifndef MGR_MANAGER_CATEGORY_HEADER
#define MGR_MANAGER_CATEGORY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "manager_traits.h"
#include "detail/type_manip.h"

namespace memory_mgr
{
	//Tags to categorize memory managers capabilities
	struct memory_manager_tag{};
	struct singleton_manager_tag{};
	struct size_tracking_tag{};
	struct pointer_convertion_tag{};
	struct named_objects_manager_tag{};

	//Tags to categorize memory types
	struct unknown_memory_tag{};
	struct heap_memory_tag{};
	struct shared_memory_tag{};

	template< class MemMgr, class Category >
	struct is_category_supported
	{
		struct yes_type{};
		struct no_type{};

		enum {value = type_manip::super_subclass
			< 
			Category,
			typename manager_traits<MemMgr>::manager_category			 
			>::value};

		typedef typename type_manip::select< 
			value, 
			yes_type,
			no_type
		>::result result;
	};
}


#endif //MGR_MANAGER_CATEGORY_HEADER
