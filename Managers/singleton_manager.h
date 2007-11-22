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
// 	template< class MgrT >
// 	class singleton_ptr_t: public detail::off_ptr_t< MgrT >
// 	{		
// 		typedef MgrT mgr_t;
// 	public:			
// 		typedef typename detail::off_ptr_t< mgr_t >	base_t;
// 		typedef typename base_t::size_type			size_type;
// 
// 		singleton_ptr_t( const base_t& ptr )
// 			:base_t( ptr.get_off() )
// 		{}
// 
// 		//Construct pointer from offset
// 		explicit singleton_ptr_t( const size_type offset )
// 			:base_t( offset )
// 		{}
// 
// 		//Construct pointer from memory address
// 		singleton_ptr_t( const mgr_t& mgr, const void* ptr )			
// 			:base_t( mgr, ptr )
// 		{}
// 
// 		void* get_ptr()
// 		{
// 			return base_t::get_ptr( mgr_t::instance() );
// 		}
// 
// 		const void* get_ptr() const
// 		{
// 			return base_t::get_ptr( mgr_t::instance() );
// 		}
// 
// 		//Call this method to get real memory address
// 		const void* get_ptr( const mgr_t& mgr ) const
// 		{
// 			return base_t::get_ptr( mgr );
// 		}
// 	};
	

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
		typedef singleton_manager<mgr_t, SyncObj, ThreadingModel> self_type;
		typedef typename mgr_t::ptr_t				ptr_t;
		//typedef singleton_ptr_t<self_type>				ptr_t;
	
		enum
		{
			chunk_size = mgr_t::chunk_size,
			memory_size =  mgr_t::memory_size,
			num_chunks =  mgr_t::num_chunks
		};

		static const ptr_t null_ptr;
	private:
		singleton_manager();
		~singleton_manager();
		singleton_manager(const singleton_manager&);
		singleton_manager& operator=(const singleton_manager&);
	};

	template< class MemMgr, class SyncObj, template <class> class ThreadingModel >
	typename const singleton_manager<MemMgr, SyncObj, ThreadingModel>::ptr_t singleton_manager<MemMgr, SyncObj, ThreadingModel>::null_ptr( singleton_manager<MemMgr, SyncObj, ThreadingModel>::mgr_t::null_ptr );
}

#endif// MGR_SINGLETON_MANAGER_HEADER
