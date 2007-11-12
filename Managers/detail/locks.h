#pragma once

namespace managers
{
	namespace detail
	{
		namespace sync
		{
			//-------------------------------------
			// Object level blocking
			template<class SyncObjT>
			class object_level_lockable
			{  
			public:
				friend class LockT;

				object_level_lockable(){}

				// 		template< typename T >
				// 		object_level_lockable( T val )
				// 			:m_syncObj( val )
				// 		{}

				template<class LocableT>
				class LockT
				{
				public:
					LockT( const LocableT &c ) 
						: m_lackable(c)
					{ 
						m_lackable.m_syncObj.Enter();
					};

					~LockT()
					{ 
						m_lackable.m_syncObj.Leave();
					};

				private:
					LocableT const &m_lackable;

					LockT(const LockT &c);
					LockT& operator=(const LockT &c);
				};

				typedef LockT< object_level_lockable > Lock;
			private:
				mutable volatile SyncObjT m_syncObj;
				object_level_lockable( const object_level_lockable& );
				object_level_lockable& operator=( const object_level_lockable& );
			};
			//-------------------------------------

			template <class SyncObjT>
			class class_level_lockable
			{
				static SyncObjT sm_syncObj;

			public:
				//class Lock;
				friend class Lock;

				class Lock
				{
					Lock(const Lock&);
					Lock& operator=(const Lock&);
				public:
					Lock()
					{
						sm_syncObj.Enter();
					}

					// 			explicit Lock(const class_level_lockable&)
					// 			{
					// 				sm_syncObj.Enter();
					// 			}

					~Lock()
					{
						sm_syncObj.Leave();
					}
				};
			};

			template <class SyncObjT>
			SyncObjT class_level_lockable<SyncObjT>::sm_syncObj;
		
		}//sync

	}//detail

}//managers


