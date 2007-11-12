
#pragma once

#include "../singleton.h"
namespace managers
{
	template 
	<
		class Mgr,
		class SyncObj = ::sync::critical_section, 
		template <class> class ThreadingModel = ::sync::class_level_lockable
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