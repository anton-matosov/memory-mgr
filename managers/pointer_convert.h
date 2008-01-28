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

#ifndef MGR_POINTER_CONVERT_HEADER
#define MGR_POINTER_CONVERT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <vector>

namespace memory_mgr
{	
	//Replaces memory_manager allocate/deallocate methods to work with pointers instead of offsets 
	//MemMgr - must support MemoryManagerConcept
	template< class MemMgr >	
	class pointer_convert 
	{
		typedef MemMgr				memmgr_type;
		memmgr_type m_mgr;
	public:
		enum
		{
			chunk_size	= memmgr_type::chunk_size,
			memory_size = memmgr_type::memory_size,
			num_chunks	= memmgr_type::num_chunks
		};
		typedef typename memmgr_type::block_ptr_type	block_ptr_type;		
		typedef typename memmgr_type::size_type			size_type;

		typedef memmgr_type								self_type;
		typedef typename memmgr_type::offset_type		offset_type;

		pointer_convert( void* base_address )
			:m_mgr( base_address)
		{}

		//Call this method to allocate memory block
		//size - block size in bytes
		void* allocate( size_type size )
		{			
			return detail::shift( m_mgr.get_base(), m_mgr.allocate( size ) );
		}

		//Call this method to allocate memory block
		//Newer throws
		//size - block size in bytes
		void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
		{			
			return detail::shift( m_mgr.get_base(), m_mgr.allocate( size, nothrow ) );
		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_mem_base + offset, returned by allocate method
		//size - block size in bytes
 		void deallocate( const void* p, size_type size )
 		{
			assert( p >= m_mgr.get_base() && (p < ( m_mgr.get_base() + memmgr_type::memory_size ) )
				&& "Invalid pointer value" );
			m_mgr.deallocate( detail::diff( m_mgr.get_base(), p ), size );
 		}
	};
}
#endif// MGR_POINTER_CONVERT_HEADER
