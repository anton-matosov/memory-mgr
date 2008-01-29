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
	namespace detail
	{
		template< class MemMgr >
		class size_tracking_impl_base
		{
			typedef MemMgr									mgr_type;
		public:
// 			enum
// 			{
// 				chunk_size	= memmgr_type::chunk_size,
// 				memory_size = memmgr_type::memory_size,
// 				num_chunks	= memmgr_type::num_chunks
// 			};

// 			typedef typename memmgr_type::block_ptr_type	block_ptr_type;		
 			typedef typename manager_traits<mgr_type>::size_type			size_type;
// 			typedef typename memmgr_type::offset_type		offset_type;

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

		protected:
			explicit size_tracking_impl_base( void* mem_base )
				:m_memmgr( mem_base )
			{}

			void update_size( size_type size )
			{
				//Additional memory for size storing
				size += sizeof( size_type );
			}

			void* store_size( void* ptr, size_type size )
			{
				//Store size
				size_type* psize = size_cast( ptr );
				*psize = size;
				return ++psize;
			}

			size_type* size_cast( void* p ) const
			{
				return static_cast< size_type* >( p );
			}

			mgr_type m_memmgr;
		};

// 		template< class MemMgr >
// 		class size_tracking_impl
// 		{
// 		protected:
// 			explicit size_tracking_impl( void* mem_base )
// 			{
// 				STATIC_ASSERT( false, size_tracking_for_offset_mgr_not_implemented );
// 			}
// 		};

		template< class MemMgr >
		class size_tracking_impl/*< pointer_convert< MemMgr > > */: public size_tracking_impl_base< MemMgr >
		{
			typedef MemMgr								mgr_type;
			typedef size_tracking_impl_base< mgr_type >	impl_base_type;
			
		protected:
			explicit size_tracking_impl( void* mem_base )
				:impl_base_type( mem_base )
			{}

		public:
// 			enum
// 			{
// 				chunk_size	= impl_base_type::chunk_size,
// 				memory_size = impl_base_type::memory_size,
// 				num_chunks	= impl_base_type::num_chunks
// 			};
// 
// 			typedef typename impl_base_type::block_ptr_type		block_ptr_type;		
 			typedef typename impl_base_type::size_type			size_type;
// 			typedef typename impl_base_type::offset_type		offset_type;

			//Call this method to allocate memory block
			//size - block size in bytes
			void* allocate( size_type size )
			{			
				update_size( size );
				return store_size( this->m_memmgr.allocate( size ), size );
			}

			//Call this method to allocate memory block
			//Newer throws
			//size - block size in bytes
			void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
			{
				update_size( size );
				return store_size( this->m_memmgr.allocate( size, nothrow ), size );
			}

			//Call this method to deallocate memory block
			//p - pointer calculated as mgr_mem_base + offset, returned by allocate method
			//size - block size in bytes
			void deallocate( void* p )
			{
				assert( p >=this->m_memmgr.get_base() && (p < ( this->m_memmgr.get_base() + manager_traits<mgr_type>::memory_size ) )
					&& "Invalid pointer value" );

				size_type *ps = this->size_cast( p );
				--ps;
				this->m_memmgr.deallocate( ps, *ps );
			}
		};
	}

	//Size tracking decorator for memory manager
	//MemMgr - must support PointerConvertConcept
	template< class MemMgr >
	class size_tracking : public detail::size_tracking_impl< MemMgr >
	{
		typedef MemMgr									memmgr_type;
		typedef detail::size_tracking_impl< MemMgr >	impl_type;

	public:
// 		enum
// 		{
// 			chunk_size	= impl_type::chunk_size,
// 			memory_size = impl_type::memory_size,
// 			num_chunks	= impl_type::num_chunks
// 		};
// 
// 		typedef typename impl_type::block_ptr_type	block_ptr_type;		
// 		typedef typename impl_type::size_type		size_type;
// 		typedef typename impl_type::offset_type		offset_type;

		explicit size_tracking( void* mem_base )
			:impl_type( mem_base )
		{}
	};
}

#endif //MGR_SIZE_TRACKING_HEADER
