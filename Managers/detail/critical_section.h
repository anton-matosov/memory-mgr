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

#ifndef MGR_CRITICAL_SECTION_HEADER
#define MGR_CRITICAL_SECTION_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "../config/config.h"

#if defined(MGR_WINDOWS_PLATFORM)

namespace managers
{
	namespace detail
	{
		namespace sync
		{
			//-------------------------------------
			class critical_section{
			public:
				critical_section()	{ InitializeCriticalSection	(&m_cs); }
				~critical_section()	{ DeleteCriticalSection		(&m_cs); }
				void Enter() const	{ EnterCriticalSection		(&m_cs); }
				void Leave() const	{ LeaveCriticalSection		(&m_cs); }
			private:
				mutable CRITICAL_SECTION m_cs;

				critical_section(const critical_section &);
				critical_section & operator=(const critical_section &);
			};

		}//sync

	}//detail

}//managers
#endif

#endif// MGR_CRITICAL_SECTION_HEADER