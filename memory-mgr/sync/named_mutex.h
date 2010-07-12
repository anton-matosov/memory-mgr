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

#ifndef MGR_NAMED_MUTEX_HEADER
#define MGR_NAMED_MUTEX_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/config/config.h>

namespace memory_mgr
{
	namespace sync
	{
		//-------------------------------------
		class named_mutex
		{
		public:
			inline named_mutex( const std::string& name )
			{ m_mutex = osapi::create_mutex( name ); }

			inline ~named_mutex()
			{ osapi::release_mutex( m_mutex ); }

			inline void Enter() const
			{ osapi::lock_mutex( m_mutex ); }

			inline void Leave() const
			{ osapi::release_mutex( m_mutex ); }
		private:
			mutable osapi::mutex_handle_t m_mutex;

			named_mutex(const named_mutex &);
			named_mutex & operator=(const named_mutex &);
		};

	}//sync


}//memory_mgr


#endif// MGR_CRITICAL_SECTION_HEADER
