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

#ifndef MGR_OPERATOR_NEW_HEADER
#define MGR_OPERATOR_NEW_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/new_helpers.h>

/**
   @brief Overloaded operator new, allocates memory block in memory managed passed as second parameter
   @param size size of memory block required to store object,
				this parameter is passed by compiler automatically
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception bad_alloc if manager went out of memory  
   @return pointer to allocated memory block
*/
template<class MemMgr>
inline void* operator new( size_t size, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	typedef typename memory_mgr::detail::mem_mgr_wrapper<MemMgr>::new_helper_type helper_type;
	
	return helper_type::allocate( size, mgr.get() );
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
inline void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	typedef typename memory_mgr::detail::mem_mgr_wrapper<MemMgr>::new_helper_type helper_type;

	return helper_type::allocate( size + sizeof( size_t ), mgr.get() );
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
inline void operator delete( void* p, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	if( p )
	{
		typedef typename memory_mgr::detail::mem_mgr_wrapper<MemMgr>::new_helper_type helper_type;

		helper_type::destroy_and_deallocate( p, mgr.get() );
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
inline void operator delete[]( void* p, const memory_mgr::detail::mem_mgr_wrapper<MemMgr>& mgr )
{
	if( p )
	{
		typedef typename memory_mgr::detail::mem_mgr_wrapper<MemMgr>::new_helper_type helper_type;

		helper_type::destroy_and_deallocate_array( p, mgr.get() );
	}
}


#endif //MGR_OPERATOR_NEW_HEADER
