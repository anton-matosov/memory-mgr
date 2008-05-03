/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007 Anton (shikin) Matosov

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

#ifndef MGR_MANAGED_BASE_HEADER
#define MGR_MANAGED_BASE_HEADER

#include "detail/static_assert.h"
#include "detail/ptr_helpers.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{
		static inline size_t calc_full_size( void* p, size_t size )
		{
#ifdef _MSC_VER
			size = *size_cast(p) * size;
			return size + sizeof( size_t );
#else
			return size;
#endif
		}
	}
	class object_name
			
	{	
		const wchar_t* m_name;
	public:
		object_name( const wchar_t* name )
			: m_name( name )
		{}

		const wchar_t* get_name()
		{
			return m_name;
		}
	};

	//MemMgr - must support PointerConvertConcept, SingletonManagerConcept
	template< class MemMgr >
	class managed_base
	{
	protected:
		~managed_base()
		{}

		typedef MemMgr mem_mgr;
	public:	
		static void* operator new( size_t size )/*throw( std::bad_alloc )*/
		{
			return mem_mgr::instance().allocate( size );			
		}

		static void* operator new( size_t size, const object_name& /*name*/ )/*throw( std::bad_alloc )*/
		{
			//TODO:implement correct logic in this method
			//mem_mgr::instance().allocate( size, name );
			STATIC_ASSERT( false, named_objects_not_supported );
			return 0;
		}

		static void* operator new( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		static void* operator new(  size_t, void* p ) /*throw()*/
		{
			return p;
		}

		static void operator delete( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, size );
		}

		static void operator delete( void* /*p*/, size_t size, const object_name& /*name*/ )
		{
			//TODO:implement correct logic in this method
			//mem_mgr::instance().deallocate( p, size );
			STATIC_ASSERT( false, named_objects_not_supported );
		}

		static void* operator new[]( size_t size )/*throw( std::bad_alloc )*/
		{			
			return mem_mgr::instance().allocate( size );			
		}

		static void* operator new[]( size_t size, const object_name& /*name*/ )/*throw( std::bad_alloc )*/
		{
			//TODO:implement correct logic in this method
			//mem_mgr::instance().allocate( size, name );
			STATIC_ASSERT( false, named_objects_not_supported );
			return 0;
		}

		static void* operator new[]( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			
			return mem_mgr::instance().allocate( size, nothrow );			
		}

		static void* operator new[](  size_t, void* p ) /*throw()*/
		{
			return p;
		}

		static void operator delete[]( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( p, detail::calc_full_size( p, size) );
		}

		static void operator delete[]( void* /*p*/, size_t size, const object_name& /*name*/ )
		{
			//TODO:implement correct logic in this method
			//mem_mgr::instance().deallocate( p, size );
			STATIC_ASSERT( false, named_objects_not_supported );
		}
	};
	
	
}

#endif// MGR_MANAGED_BASE_HEADER
