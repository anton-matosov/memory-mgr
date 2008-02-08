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

#include "manager_traits.h"
#include "manager_category.h"
#include "pointer_convert.h"

namespace memory_mgr
{
	namespace detail
	{
		template< class MemMgr >
		class size_tracking_impl_base
		{
			typedef MemMgr									mgr_type;
		public:	
 			typedef typename manager_traits<mgr_type>::size_type			size_type;

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

		//base class for templates specializations
		template<class MemMgr, class pointer_conv_flag >
		class size_tracking_impl;

		template< class MemMgr >
		class size_tracking_impl<
			MemMgr,
			typename is_category_supported<MemMgr, pointer_convertion_tag>::yes_type >
			: public size_tracking_impl_base< MemMgr >
		{
			typedef MemMgr								mgr_type;
			typedef size_tracking_impl_base< mgr_type >	impl_base_type;

		protected:
			explicit size_tracking_impl( void* mem_base )
				:impl_base_type( mem_base )
			{}

		public:
			typedef typename impl_base_type::size_type			size_type;

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

		template< class MemMgr >
		class size_tracking_impl<
			MemMgr,
			typename is_category_supported<MemMgr, pointer_convertion_tag>::no_type >
			: public size_tracking_impl< 
				pointer_convert< MemMgr >,
				typename is_category_supported<pointer_convert< MemMgr >, pointer_convertion_tag>::yes_type
			>
		{
			typedef pointer_convert< MemMgr >				mgr_type;
			typedef size_tracking_impl< 
				mgr_type,
				typename is_category_supported<mgr_type, pointer_convertion_tag>::yes_type
			>	impl_base_type;

			typedef pointer_convert< mgr_type > pconvert;
		protected:
			explicit size_tracking_impl( void* mem_base )
				:impl_base_type( mem_base )
			{}
		public:
			typedef typename impl_base_type::size_type				size_type;
			typedef typename manager_traits<mgr_type>::offset_type	offset_type;

			//Call this method to allocate memory block
			//size - block size in bytes
			//Returns: offset in bytes from memory base address.
			offset_type allocate( size_type size )
			{					
				return pconvert::pointer_to_offset( impl_base_type::allocate( size ), this->m_memmgr );
			}

			//Call this method to allocate memory block
			//Newer throws
			//size - block size in bytes
			//Returns: offset in bytes from memory base address.
			offset_type allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
			{			
				return pconvert::pointer_to_offset( impl_base_type::allocate( size, nothrow ), this->m_memmgr );
			}

			//Call this method to deallocate memory block
			//offset - offset returned by allocate method
			//size - block size in bytes
			void deallocate( const offset_type offset )
			{
				impl_base_type::deallocate( pconvert::offset_to_pointer( offset, this->m_memmgr ) );
			}
		};


		
	}

	//Size tracking decorator for memory manager
	//MemMgr - must support PointerConvertConcept
	template< class MemMgr >
	class size_tracking 
		: public detail::size_tracking_impl
		< 
			MemMgr, 
			typename is_category_supported<MemMgr, pointer_convertion_tag>::result 
		>
	{
		typedef MemMgr									memmgr_type;
		typedef detail::size_tracking_impl
		< 
			MemMgr, 
			typename is_category_supported<MemMgr, pointer_convertion_tag>::result 
		>	impl_type;

	public:
		explicit size_tracking( void* mem_base )
			:impl_type( mem_base )
		{}
	};

	template<class MemMgr>
	struct manager_traits< size_tracking< MemMgr > > 
		: public manager_traits< typename manager_traits<MemMgr>::manager_type >
	{
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual size_tracking_tag
		{};
	};
}

#endif //MGR_SIZE_TRACKING_HEADER
