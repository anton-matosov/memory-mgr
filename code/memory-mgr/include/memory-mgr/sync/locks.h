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

#pragma once

namespace memory_mgr
{
	/**
	   @brief Synchronization principles
	*/
	namespace sync
	{
		class lockable
		{  
		public:
			virtual void enter() const = 0;
			virtual void leave() const = 0;
		};

		class scoped_lock
		{
		public:
			scoped_lock( const lockable &lockable ) 
				: m_lockable(lockable)
			{ 
				m_lockable.enter();
			};

			~scoped_lock()
			{ 
				m_lockable.leave();
			};

		private:
			lockable const &m_lockable;

			//Non copyable
			scoped_lock(const scoped_lock &);
			scoped_lock& operator=(const scoped_lock &);
		};

		class fake_lockable
			:public lockable
		{
		public:
			virtual void enter() const
			{
			}

			virtual void leave() const
			{
			}
		};

		// Locks do nothing
		template<class SyncObjT>
		class pseudo_lockable
			:public fake_lockable
		{  
		public:
			pseudo_lockable()
			{}

			template< typename T >
			pseudo_lockable( T )
			{}

			typedef scoped_lock lock_type;
		private:
			pseudo_lockable( const pseudo_lockable& );
			pseudo_lockable& operator=( const pseudo_lockable& );
		};

		//-------------------------------------
		// Object level blocking
		template<class SyncObjT>
		class object_level_lockable
			:public lockable
		{  
		public:
			object_level_lockable()
			{}

			template< typename T >
			object_level_lockable( T val )
				:m_sync_object( val )
			{}

			virtual void enter() const
			{
				m_sync_object.enter();
			}

			virtual void leave() const
			{
				m_sync_object.leave();
			}
			
			typedef scoped_lock lock_type;
		private:
			mutable SyncObjT m_sync_object;

			object_level_lockable( const object_level_lockable& );
			object_level_lockable& operator=( const object_level_lockable& );
		};
		//-------------------------------------

		template <class SyncObjT>
		class class_level_lockable
		{
			static SyncObjT sm_sync_object;

		public:
			//class lock;
			friend class lock;

			class static_lock
			{
				static_lock(const static_lock&);
				static_lock& operator=(const static_lock&);
			public:
				static_lock()
				{
					sm_sync_object.enter();
				}

				~static_lock()
				{
					sm_sync_object.leave();
				}
			};
			typedef static_lock lock_type;
		};

		template <class SyncObjT>
		SyncObjT class_level_lockable<SyncObjT>::sm_sync_object;

	}//sync

}//memory_mgr
