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

#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/detail/new_helpers.h>
#include <memory-mgr/singleton_manager.h>

namespace memory_mgr
{
	template<class T>
	class new_constructor
	{
		typedef T				object_type;
		typedef object_type*	object_pointer_type;

		object_pointer_type m_object;
	public:
		new_constructor( object_pointer_type object )
			:m_object( object )
		{

		}

		operator object_pointer_type()
		{
			return operator()();
		}

		object_pointer_type operator()()
		{
			if( m_object )
			{
				new( m_object ) object_type();
			}
			return m_object;
		}

		template<class T1>
		object_pointer_type operator()( T1 v1 )
		{
			if( m_object )
			{
				new( m_object ) object_type( v1 );
			}
			return m_object;
		}


		template<class T1, class T2>
		object_pointer_type operator()( T1 v1, T2 v2 )
		{
			if( m_object )
			{
				new( m_object ) object_type( v1, v2 );
			}
			return m_object;
		}
	};


	template<class T, class MemMgr>
	class new_proxy
	{
		typedef MemMgr			mgr_type;
		//typedef typename manager_traits< MemMgr >::base_manager_type mgr_type;
		typedef T				object_type;
		typedef object_type*	object_pointer_type;
		typedef typename memory_mgr::detail::mem_mgr_wrapper<mgr_type>::new_helper_type helper_type;
		typedef typename manager_traits<mgr_type>::size_type	size_type;

		typedef new_constructor<object_type> new_constructor;

		object_pointer_type m_object;
		mgr_type*			m_mgr;
		size_t m_num_items;
		std::string m_object_name;
	public:
		new_proxy( const new_proxy& old_proxy, size_t num_items )
			:m_object( old_proxy.m_object ),
			m_num_items( num_items ),
			m_mgr( old_proxy.m_mgr ),
			m_object_name( old_proxy.m_object_name )
		{
		}

		new_proxy( const char* object_name )
		   :m_object( 0 ),
		   m_mgr( &mgr_type::instance() ),
		   m_num_items( 1 ),
		   m_object_name( object_name == NULL ? "" : object_name )
		{

		}

 		new_proxy( const memory_mgr::detail::mem_mgr_wrapper<mgr_type>& mgr, const char* object_name  )
 			:m_object( 0 ),
 			m_mgr( &mgr.get() ),
			m_num_items( 1 ),
			m_object_name( object_name == NULL ? "" : object_name )
 		{
 
 		}

		object_pointer_type operator()()
		{
			allocate();
			object_pointer_type object = m_object;

			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type();
			}

			return m_object;
		}

		template<class T1>
		object_pointer_type operator()( T1 v1 )
		{
			allocate();
			object_pointer_type object = m_object;

			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type( v1 );
			}

			return m_object;
		}


		template<class T1, class T2>
		object_pointer_type operator()( T1 v1, T2 v2 )
		{
			allocate();
			object_pointer_type object = m_object;

			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type( v1, v2 );
			}

			return m_object;
		}

		template<class T1, class T2>
		object_pointer_type operator()( T1 v1, T2 v2, T2 v3 )
		{
			allocate();
			object_pointer_type object = m_object;

			for( size_t i = 0; i < m_num_items; ++i )
			{
				::new( object + i ) object_type( v1, v2, v3 );
			}

			return m_object;
		}

		new_proxy operator[]( size_t num_items )
		{			
			return new_proxy( *this, num_items );
		}

		void allocate()
		{
			if( ! m_object )
			{
				m_object = static_cast<object_pointer_type>( allocate_named_or_unnamed() );
			}
		}

		void* allocate_named_or_unnamed()
		{
			size_type required_size = sizeof( object_type ) * m_num_items;
			if( m_object_name.empty() )
			{
				return helper_type::allocate( required_size, *m_mgr );
			}
			else
			{
				return helper_type::allocate( required_size, *m_mgr, m_object_name.c_str() );
			}
		}
	};

 	template<class T, class MemMgr>
 	new_proxy<T, MemMgr> new_( MemMgr& mgr, const char* object_name = 0 )
 	{
 		return new_proxy<T, MemMgr>( mem_mgr( mgr ), object_name );
 	}
 
 	template<class T, class SingMemMgr>
 	new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type> new_( const char* object_name = 0 )
 	{
 		return new_proxy<T, typename manager_traits< SingMemMgr >::base_manager_type>
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
