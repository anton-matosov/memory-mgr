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
	template<class T, class MemMgr>
	class new_c
	{
		typedef MemMgr			mgr_type;
		typedef T				object_type;
		typedef object_type*	object_pointer_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;
		typedef typename manager_traits<mgr_type>::size_type	size_type;

		object_pointer_type m_object;
		mgr_type*			m_mgr;
	public:
		new_c( const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr  )
			:m_object( 0 ),
			m_mgr( &mgr.m_mgr )
		{

		}

		void allocate( size_type n = 1 )
		{
			m_object = static_cast<object_pointer_type>( 
				helper_type::new_impl( sizeof( object_type ) * n, *m_mgr ) );
		}

		object_pointer_type operator()()
		{
			if( !m_object )
			{
				allocate();
				new( m_object ) object_type();
			}
			return m_object;
		}

		template<class T1>
		object_pointer_type operator()( T1 v1 )
		{
			if( !m_object )
			{
				allocate();
				new( m_object ) object_type( v1 );
			}
			return m_object;
		}


		template<class T1, class T2>
		object_pointer_type operator()( T1 v1, T2 v2 )
		{
			if( !m_object )
			{
				allocate();
				new( m_object ) object_type( v1, v2 );
			}
			return m_object;
		}

		object_pointer_type operator[]( size_t n )
		{
			if( !m_object )
			{
				allocate( n );
				//new( m_object ) object_type[ n ];
				helper_type::construct( m_object, n );
			}
			return m_object;
		}
	};

	template<class T, class MemMgr>
	new_c<T, MemMgr> new_( MemMgr& mgr )
	{
		return new_c<T, MemMgr>( mem_mgr( mgr ) );
	}

	template<class T, class SingMemMgr>
	new_c<T, typename manager_traits< SingMemMgr >::base_manager_type> new_()
	{
		return new_c<T, typename manager_traits< SingMemMgr >::base_manager_type>( mem_mgr<SingMemMgr>() );
	}

	template<class T, class SingMemMgr, class T1>
	T* new_( T1 v1 )
	{
		return new_c<T, typename manager_traits< SingMemMgr >::base_manager_type>( mem_mgr<SingMemMgr>() )( v1 );
	}
}

/**
   @brief Overloaded operator new, allocates memory block in memory managed passed as second parameter
   @param size size of memory block required to store object,
				this parameter is passed by compiler automatically
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception bad_alloc if manager went out of memory  
   @return pointer to allocated memory block
*/
template<class MemMgr>
inline void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;
	
	return helper_type::new_impl( size, mgr.m_mgr );
}

template<class MemMgr>
inline void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& name )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size, mgr.m_mgr, name.get_name() );
}

/**
   @brief Overloaded operator new[], allocates memory block in memory managed by mem_mgr
   @param size size of memory block required to store array,
				this parameter is passed by compiler automatically
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception bad_alloc if manager went out of memory
   @return pointer to allocated memory block
*/
template<class MemMgr>
inline void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size + sizeof( size_t ), mgr.m_mgr );
}

template<class MemMgr>
inline void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& name )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size + sizeof( size_t ), mgr.m_mgr, name.get_name() );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief Overloaded operator delete, deallocates memory block in memory managed by mem_mgr
@param p pointer to memory block
@param size size of memory block that should be deallocated
@exception newer throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
inline void operator delete( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}

/**
@brief Overloaded operator delete for named objects,
deallocates memory block in memory managed by mem_mgr

@param p pointer to memory block
@param size size of memory block that should be deallocated
@param name name of the object that should be deallocated
@exception newer  throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
inline void operator delete( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& /*name*/ )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief Overloaded operator delete[], deallocates memory block in memory managed by mem_mgr
@param p pointer to memory block
@param size size of memory block that should be deallocated
@exception newer throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
inline void operator delete[]( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

/**
@brief Overloaded operator delete[] for named objects,
deallocates memory block in memory managed by mem_mgr

@param p pointer to memory block
@param size size of memory block that should be deallocated
@param name name of the object that should be deallocated
@exception newer  throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
inline void operator delete[]( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& /*name*/ )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

/**
   @brief Sort of overloaded operator delete, deallocates memory block in memory managed by mem_mgr
   @param p pointer to memory block
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception newer throws
*/
template<class T, class MemMgr>
static inline void delete_( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}


template<class T, class MemMgr>
static inline void delete_( T* p, MemMgr& mgr )
{
	delete_( p, memory_mgr::mem_mgr(mgr) );
}

template<class MemMgr, class T>
static inline void delete_( T* p )
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
static inline void delete_array( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;
		
		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

#endif //MGR_NEW_HEADER
