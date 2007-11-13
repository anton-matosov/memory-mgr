/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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
namespace managers
{
	template 
	<
		class Mgr,
		class SyncObj = detail::sync::critical_section, 
		template <class> class ThreadingModel = detail::sync::class_level_lockable
	>
	class  singleton_manager : public singleton< Mgr, SyncObj, ThreadingModel >
	{
	public:
		typedef Mgr									mgr_t;
		typedef typename mgr_t::block_ptr_type		block_ptr_type;		
		typedef typename mgr_t::size_type			size_type;

		typedef typename mgr_t::ptr_t				ptr_t;
	};
}

#endif// MGR_SINGLETON_MANAGER_HEADER