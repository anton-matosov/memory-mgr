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

#ifndef MGR_NEW_HEADER
#define MGR_NEW_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/config/config.h>
#include <memory-mgr/sync/locks.h>
#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/detail/new_helpers.h>

#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

namespace memory_mgr
{
	namespace detail
	{
		template<class T, class MemMgr>
		class allocate_base
		{
		public:
			typedef MemMgr			mgr_type;

			typedef T				object_type;
			typedef object_type*	object_pointer_type;
			typedef typename memory_mgr::detail::mem_mgr_wrapper<mgr_type>::new_helper_type helper_type;
			typedef typename manager_traits<mgr_type>::size_type	size_type;
			typedef typename ::memory_mgr::sync::lockable lockable_type;

			allocate_base( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr )
				: m_mgr( &mgr.get() )
			{

			}

			virtual ~allocate_base() = 0
			{

			}

			virtual bool construction_needed()
			{
				return construction_needed_impl();
			}

			virtual void* allocate( size_t num_items )
			{
				size_type required_size = sizeof( object_type ) * num_items;
				return allocate_impl( required_size, *m_mgr );
			}

			virtual lockable_type& get_lockable() = 0;
		
		protected:
			virtual void* allocate_impl( size_t size, mgr_type& mgr ) = 0;
			virtual bool construction_needed_impl() = 0;

			mgr_type*	m_mgr;
		};

		template<class T, class MemMgr>
		class allocate_unnamed_impl
			:public allocate_base<T, MemMgr>
		{
			typedef allocate_base<T, MemMgr> base_type;
		public:

			allocate_unnamed_impl( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr )
				: base_type( mgr )
			{

			}

			virtual lockable_type& get_lockable()
			{
				return m_fake_lockable;
			}
		protected:
			virtual void* allocate_impl( size_t size, mgr_type& mgr )
			{
				return helper_type::allocate( size, mgr );
			}

			virtual bool construction_needed_impl()
			{
				return true;
			}

		private:
			::memory_mgr::sync::fake_lockable m_fake_lockable;
		};

		template<class T, class MemMgr>
		class allocate_named_impl
			:public allocate_unnamed_impl<T, MemMgr>
		{
			typedef allocate_unnamed_impl<T, MemMgr> base_type;

			std::string m_object_name;
			bool m_construction_needed;
		public:

			allocate_named_impl( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr,
				const char* object_name  )
				: base_type( mgr ),
				m_construction_needed( true ),
				m_object_name( object_name == NULL ? "" : object_name )
			{

			}

			virtual lockable_type& get_lockable()
			{
				return m_mgr->get_lockable();
			}
		protected:
			virtual void* allocate_impl( size_t size, mgr_type& mgr )
			{
				if( m_object_name.empty() )
				{
					return base_type::allocate_impl( size, mgr );
				}
				else
				{
					m_construction_needed = ! mgr.is_exists( m_object_name.c_str() );
					return helper_type::allocate( size, mgr, m_object_name.c_str() );
				}
			}

			virtual bool construction_needed_impl()
			{
				return m_construction_needed;
			}
		};


		template<class T, class MemMgr>
		class new_proxy
		{
			typedef MemMgr			mgr_type;
			typedef T				object_type;
			typedef object_type*	object_pointer_type;
			typedef allocate_base<T, MemMgr> allocate_base_type;
			typedef typename manager_traits<mgr_type>::lock_type lock_type;

			object_pointer_type m_object;
			size_t m_num_items;
			boost::shared_ptr<allocate_base_type> m_alloc;

		public:
			new_proxy( const new_proxy& old_proxy, size_t num_items )
				:m_object( old_proxy.m_object ),
				m_alloc( old_proxy.m_alloc ),
				m_num_items( num_items )
			{
			}

			new_proxy( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr, const char* object_name  )
				:m_object( 0 ),
				m_alloc( new allocate_named_impl<T, mgr_type>( mgr, object_name ) ),
				m_num_items( 1 )
			{

			}

			new_proxy( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr )
				:m_object( 0 ),
				m_alloc( new allocate_unnamed_impl<T, mgr_type>( mgr ) ),
				m_num_items( 1 )
			{

			}

			new_proxy operator[]( size_t num_items )
			{			
				return new_proxy( *this, num_items );
			}

			void allocate()
			{
				if( ! m_object )
				{
					m_object = static_cast<object_pointer_type>( m_alloc->allocate( m_num_items ) );
				}
			}

#include <memory-mgr/detail/new_proxy_operator_braces.h>
		};

	}

 	template<class T, class MemMgr>
	detail::new_proxy<T, MemMgr> new_( MemMgr& mgr )
 	{
 		return detail::new_proxy<T, MemMgr>( mem_mgr( mgr ) );
 	}

	template<class T, class MemMgr>
	detail::new_proxy<T, MemMgr> new_( MemMgr& mgr, const char* object_name )
	{
		return detail::new_proxy<T, MemMgr>( mem_mgr( mgr ), object_name );
	}

 	template<class T, class SingMemMgr>
 	detail::new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type> new_()
 	{
 		return detail::new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type>
			( mem_mgr<SingMemMgr>() );
 	}

	template<class T, class SingMemMgr>
	detail::new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type> new_( const char* object_name )
	{
		return detail::new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type>
			( mem_mgr<SingMemMgr>(), object_name );
	}

// 	template<class T, class SingMemMgr, class T1>
// 	T* new_( T1 v1 )
// 	{
// 		return new_allocator<T, typename manager_traits< SingMemMgr >::base_manager_type>( mem_mgr<SingMemMgr>() )( v1 );
// 	}
}


/**
   @brief Sort of overloaded operator delete, deallocates memory block in memory managed by mem_mgr
   @param p pointer to memory block
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception newer throws
*/
template<class T, class MemMgr>
inline void delete_( T* p, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_wrapper<mgr_type>::new_helper_type helper_type;

		helper_type::destroy_and_deallocate( p, mgr.get() );
	}
}

template<class T, class MemMgr>
inline void delete_( T* p, MemMgr& mgr )
{
	delete_( p, memory_mgr::mem_mgr(mgr) );
}

template<class MemMgr, class T>
inline void delete_( T* p )
{
	delete_( p, memory_mgr::mem_mgr<MemMgr>() );
}

/**
   @brief Sort of overloaded operator delete[], deallocates memory block in memory managed by mem_mgr
   @param p pointer to memory block
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception newer throws
   */
template<class T, class MemMgr>
inline void delete_array( T* p, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_wrapper<mgr_type>::new_helper_type helper_type;
		
//#error This deletion is incorrect if object is class and was constructed with new_()
		helper_type::destroy_and_deallocate_array( p, mgr.get() );
	}
}

template<class T, class MemMgr>
inline void delete_array( const T* p, MemMgr& mgr )
{
	::delete_array( p, memory_mgr::mem_mgr( mgr ) );
}

template<class MemMgr, class T>
inline void delete_array( const T* p )
{
	::delete_array( p, memory_mgr::mem_mgr<MemMgr>() );
}

#endif //MGR_NEW_HEADER
