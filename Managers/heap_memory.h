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

#ifndef MGR_HEAP_MEMORY_HEADER
#define MGR_HEAP_MEMORY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "memory_manager.h"
#include "singleton_manager.h"
#include "detail/scoped_ptr.h"
#include "std_pointer.h"

namespace memory_mgr
{

	template< class MemMgr >	
	class heap_memory
	{
		typedef MemMgr		memmgr_t;
		
				
		scoped_ptr<char, ::detail::array_deleter> m_memory;
		memmgr_t m_mgr;
	public:	
		enum
		{
			chunk_size = memmgr_t::chunk_size,
			memory_size =  memmgr_t::memory_size,
			num_chunks =  memmgr_t::num_chunks
		};
		typedef typename memmgr_t::block_ptr_type		block_ptr_type;		
		typedef typename memmgr_t::size_type			size_type;

		typedef typename memmgr_t::self_type			self_type;
		typedef typename memmgr_t::ptr_type				ptr_type;

		static const ptr_type null_ptr;

		heap_memory()
			:m_memory( new char[memory_size] ),
			m_mgr( &*m_memory )
		{}

		~heap_memory()
		{}

 		operator memmgr_t&()
 		{
 			return m_mgr;
 		}

		operator const memmgr_t&() const
		{
			return m_mgr;
		}

		//Call this method to allocate memory block
		//size - block size in bytes
 		ptr_type allocate( size_type size )
 		{
 			return m_mgr.allocate( size );
 		}
 
		//Call this method to deallocate memory block
		//off - offset returned by allocate method
		//size - block size in bytes
		void deallocate( const ptr_type ptr, size_type size )
 		{
 			m_mgr.deallocate( ptr, size );
 		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		//size - block size in bytes
		void deallocate( const void* p, size_type size )
		{
			m_mgr.deallocate( p, size );
		}
  
  		//Returns base address
  		const char* get_base() const
  		{
  			return m_mgr.get_base();
  		}

		bool empty()
		{
			return m_mgr.empty();
		}

		bool free()
		{
			return m_mgr.free();
		}
	};

	template< class MemMgr >
	typename const heap_memory<MemMgr>::ptr_type heap_memory<MemMgr>::null_ptr( heap_memory<MemMgr>::memmgr_t::null_ptr );

	typedef singleton_manager< heap_memory< memory_manager<size_t, 1024 * 1024, 4, std_pointer> > > def_heap_mgr;
}

#endif// MGR_HEAP_MEMORY_HEADER