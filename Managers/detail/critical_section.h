
#pragma once

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