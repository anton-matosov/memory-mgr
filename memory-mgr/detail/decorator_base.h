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

#ifndef MGR_DECORATOR_BASE_HEADER
#define MGR_DECORATOR_BASE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif


#include <memory-mgr/manager_traits.h>
#include <memory-mgr/detail/static_assert.h>

namespace memory_mgr
{
	namespace detail
	{
		template< class MemMgr >
		class decorator_base
		{
		protected:
			/**
			   @brief Memory manager class that should be decorated
			*/
			typedef MemMgr									mgr_type;

		
			/**
			   @brief Memory Manager instance that will be decorated
			*/
			mgr_type m_mgr;
		public:	
			/**
			   @brief Default constructor, creates memory manager 
			   @remarks Can be used only if decorates memory manager with 
						attached memory segment
			   @see memory_manager::memory_segment                        
			*/
			inline decorator_base()
			{
				STATIC_ASSERT( (is_category_supported< mgr_type, memory_segment_tag >::value), Memory_manager_does_not_have_attached_memory_segment );
			}

			/**
			   @missing_comments 
			*/
			inline explicit decorator_base( void* segment_base )
				:m_mgr( segment_base )
			{}

			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
			typedef typename manager_traits<mgr_type>::size_type		size_type;
			
			/**
			   @brief type that used to store memory offset
			   @see memory_manager::offset_type
			*/
			typedef typename manager_traits<mgr_type>::offset_type		offset_type;


			/**
			@brief	memory block id type
			@see	@see memory_manager::block_id_type
			*/
			typedef typename manager_traits<mgr_type>::block_id_type	block_id_type;

			/**
			   @brief Call this method to know is there available memory in
				manager

			   @exception newer  throws
			   @retval true   if there is no more free memory to
				allocate
			   @retval false  otherwise                                    
			*/
			inline bool empty()
			{
				return m_mgr.empty();
			}

			
			/**
			   @brief Call this method to know that there is no allocated blocks
			   @exception newer  throws
			   @retval true   no blocks are allocated by this manager
			   @retval false  otherwise                                     
			*/
			inline bool is_free()
			{
				return m_mgr.is_free();
			}

			/**
			   @brief Call this method to deallocate all allocated memory
			   @exception newer  throws                                     
			*/
			inline void clear()
			{
				m_mgr.clear();
			}

			/**
			   @brief Call this method to get memory base address from which offset
			   is calculated
			   @exception newer  throws
			   @return pointer to memory base address                               
			*/
			inline const char* get_offset_base( const offset_type offset = 0 ) const
			{
				return m_mgr.get_offset_base( offset );
			}

			inline char* get_offset_base( offset_type offset = 0 )
			{
				return m_mgr.get_offset_base( offset );
			}

			/**
			   @add_comments
			*/
			inline const char* get_ptr_base( const void* ptr ) const
			{
				return m_mgr.get_ptr_base( ptr );
			}

			inline char* get_ptr_base( void* ptr )
			{
				return m_mgr.get_ptr_base( ptr );
			}

			inline void* offset_to_pointer( offset_type offset )
			{
				return m_mgr.offset_to_pointer( offset );
			}

			inline offset_type pointer_to_offset( const void* ptr )
			{
				return m_mgr.pointer_to_offset( ptr );
			}
		};

	}

	/**
	   @brief memory_manager + decorator_base traits
	*/
	template<class MemMgr>
	struct manager_traits< detail::decorator_base< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};
}

#endif //MGR_DECORATOR_BASE_HEADER


