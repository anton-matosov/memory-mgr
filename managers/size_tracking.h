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

#ifndef MGR_SIZE_TRACKING_HEADER
#define MGR_SIZE_TRACKING_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{

	//Size tracking decorator for memory manager
	template< class MemMgr >
	class size_tracking
	{
		typedef MemMgr									memmgr_type;				
	public:
		enum
		{
			chunk_size = memmgr_type::chunk_size,
			memory_size =  memmgr_type::memory_size,
			num_chunks =  memmgr_type::num_chunks
		};

		typedef typename memmgr_type::block_ptr_type	block_ptr_type;		
		typedef typename memmgr_type::size_type			size_type;
		typedef typename memmgr_type::ptr_type			ptr_type;

		explicit size_tracking( void* mem_base )
			:m_memmgr( mem_base )
		{}

		ptr_type allocate( size_type size )
		{
			//allocate additional memory for size storing
			size += sizeof( size_type );
			ptr_type ptr = m_memmgr.allocate( size );
			size_type* psize = size_cast( ptr );
			*psize = size;
			return ptr_type( m_memmgr, ++psize );
		}

		void deallocate( ptr_type ptr )
		{
			size_type *ps = size_cast( ptr );
			--ps;
			m_memmgr.deallocate( ps, *ps );
		}

		//Call this method to deallocate memory block
		//p - pointer calculated as mgr_base + offset, returned by allocate method
		void deallocate( const void* p )
		{
			assert( p > m_memmgr.get_base() && "Invalid pointer value" );
			deallocate( ptr_type( m_memmgr, p ) );
		}

		bool empty()
		{
			return m_memmgr.empty();
		}

		bool free()
		{
			return m_memmgr.free();
		}

		//Returns base address
		const char* get_base() const
		{
			return m_memmgr.get_base();
		}

	private:
		size_type* size_cast( ptr_type p ) const
		{
			return static_cast< size_type* >( p.get_ptr(m_memmgr) );
		}

		memmgr_type m_memmgr;
	};
}

#endif //MGR_SIZE_TRACKING_HEADER
