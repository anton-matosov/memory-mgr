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

#ifndef MGR_SINGLETON_MANAGER_HEADER
#define MGR_SINGLETON_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/singleton.h"
#include "manager_traits.h"
#include "manager_category.h"

namespace memory_mgr
{	
	template
	< 
		class Mgr, 
		class SyncObj,
		template <class> class ThreadingModel
	>
	class  singleton_manager;

	template 
	<
		class Mgr,
		class SyncObj, 
		template <class> class ThreadingModel
	>
	struct manager_traits< singleton_manager< Mgr, SyncObj, ThreadingModel > >;

	template 
	<
		class Mgr,
		class SyncObj = detail::sync::critical_section, 
		template <class> class ThreadingModel = detail::sync::class_level_lockable
	>
	class  singleton_manager : public singleton< /*typename manager_traits<*/Mgr/*>::manager_type*/, Mgr, SyncObj, ThreadingModel >
	{
	private:
		singleton_manager();
		~singleton_manager();
		singleton_manager(const singleton_manager&);
		singleton_manager& operator=(const singleton_manager&);
	};

	template 
	<
		class MemMgr,
		class SyncObj, 
		template <class> class ThreadingModel
	>
	struct manager_traits< singleton_manager< MemMgr, SyncObj, ThreadingModel > > 
		: public manager_traits< MemMgr >
	{
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual singleton_manager_tag
		{};
	};
}

#endif// MGR_SINGLETON_MANAGER_HEADER
