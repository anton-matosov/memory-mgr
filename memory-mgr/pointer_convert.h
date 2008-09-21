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

#ifndef MGR_POINTER_CONVERT_HEADER
#define MGR_POINTER_CONVERT_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/detail/ptr_helpers.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/manager_category.h>
//#include <memory-mgr/block_id_converter.h>


namespace memory_mgr
{	
	/**
	   @brief Pointer converting decorator for memory manager. Implements PointerConverterConcept
	   @details Replaces memory_manager allocate/deallocate methods to work with pointers instead of offsets
	   @tparam MemMgr  memory_manager class, with or w/o decorators.
					must support MemoryManagerConcept
	*/
	template< class MemMgr >	
	class pointer_convert
		:public detail::decorator_base<MemMgr>
	{
		typedef detail::decorator_base<MemMgr> base_type;
	public:	
		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
 		typedef typename base_type::size_type		size_type;


		/**
		@brief	memory block id type
		@see	@see memory_manager::block_id_type
		*/
		typedef void*								block_id_type;

		/**
		   @brief Default constructor, creates memory manager 
		   @remarks Can be used only if decorates memory manager with 
					attached memory segment
		   @see memory_manager::memory_segment                        
		*/
		inline pointer_convert()
		{}

		/**
		   @brief Constructor, creates memory manager with specified
		   base address
		   @param segment_base  Pointer to memory which will be managed by
		                    manager
		   @see memory_manager::memory_manager                        
		*/
		inline pointer_convert( void* segment_base )
			:base_type( segment_base )
		{}

		/**
		  @brief Call this method to allocate memory block
		  @param size size of memory block in bytes
		  @exception bad_alloc if manager went out of memory
		  @return pointer to allocated memory block
		*/
		inline block_id_type allocate( size_type size )
		{			
			return detail::offset_to_pointer( this->m_mgr.allocate( size ), this->m_mgr );
		}

		/**
		   @brief Call this method to allocate memory block
		   @param size    size of memory block in bytes 
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   
		   @exception newer  throws
		   @return pointer to allocated memory block         
		*/
		inline block_id_type allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
		{			
			return detail::offset_to_pointer( this->m_mgr.allocate( size, nothrow ), this->m_mgr );
		}

		/**
		   @brief Call this method to deallocate memory block
		   @param p  pointer to memory block, returned by allocate method
		   @param size   size of memory block in bytes
		   @exception newer  throws
		*/
 		inline void deallocate( const block_id_type p, size_type size )
 		{
			this->m_mgr.deallocate( detail::pointer_to_offset( p, this->m_mgr ), size );
 		}		
	};


	/**
	@brief Converter class which should be used to convert objects of block_id_type
	@detail template specialization for pointer convert decorator
	*/
	template<class MemMgr> 
	class block_id_converter< pointer_convert< MemMgr > > 
	{
	public:
		typedef		pointer_convert< MemMgr >							mgr_type;

		template<class block_id_type, class MgrT>
		static inline typename manager_traits<mgr_type>::block_id_type	to_block_id( block_id_type id, MgrT& mgr )
		{
			return detail::to_pointer<typename mgr_type::block_id_type>( id, mgr );
		}
	};

	/**
	   @brief memory_manager + pointer_convert traits
	   @details Adds pointer_conversion_tag to manager_category
	*/
	template< class MemMgr >
	struct manager_traits< pointer_convert< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		typedef pointer_convert< MemMgr >	mgr_type;
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr	base_manager_type;
		
		/**
		@brief	memory block id type
		@see	@see memory_manager::block_id_type
		*/
		typedef typename mgr_type::block_id_type	block_id_type;

		typedef block_id_converter<mgr_type>		block_id_converter_type;

		/**
		   @brief Add pointer_conversion_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual pointer_conversion_tag
		{};
	};

}
#endif// MGR_POINTER_CONVERT_HEADER
