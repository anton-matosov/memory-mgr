#pragma once

#include "locks.h"
#include "critical_section.h"

namespace managers
{
	template 
	<
		class T,
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
				Lock lock;
				if ( !m_instance )
				{
					m_instance = new T();
					atexit( destruct );
				}
			}
			return *m_instance;
		}


		static void destruct()
		{
			if ( m_instance )
			{
				Lock lock;
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
		static T* m_instance;

		
		singleton & operator=(const singleton &);
	};
	//------------------------------------------------------

	template 
	<
		class T,
		class SyncObj,
		template <class> class ThreadingModel
	>
	typename T* singleton<T, SyncObj, ThreadingModel>::m_instance = NULL;

}
