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

#ifndef MGR_SINGLETON_HEADER
#define MGR_SINGLETON_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "locks.h"
#include "critical_section.h"

namespace managers
{
	template 
	<
		class T,
		class InstT = T,
		class SyncObj = detail::sync::critical_section, 
		template <class> class ThreadingModel = detail::sync::class_level_lockable
	>
	class  singleton : private ThreadingModel< SyncObj >
	{
	public:
		static T& instance()
		{
			if ( !m_instance )
			{
				lock l;
				if ( !m_instance )
				{
					m_instance = new InstT();
					atexit( destruct );
				}
			}
			return *m_instance;
		}


		static void destruct()
		{
			if ( m_instance )
			{
				lock l;
				if ( m_instance )
				{
					delete m_instance;
					m_instance = NULL;
				}
			}
		}

	protected:
		// shield the constructor and destructor to prevent outside sources
		// from creating or destroying a CSingleton instance.

		//! Default constructor.
		singleton(){}
		//! Destructor.
		~singleton(){}
	private:			
		static InstT* m_instance;

		
		singleton & operator=(const singleton &);
	};
	//------------------------------------------------------

	template 
	<
		class T,
		class InstT,
		class SyncObj,
		template <class> class ThreadingModel
	>
	typename InstT* singleton<T, InstT, SyncObj, ThreadingModel>::m_instance = NULL;

}


#endif// MGR_SINGLETON_HEADER