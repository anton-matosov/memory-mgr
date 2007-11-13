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

#ifndef MGR_LOCKS_HEADER
#define MGR_LOCKS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

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


#endif// MGR_LOCKS_HEADER