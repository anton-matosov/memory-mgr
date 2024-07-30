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

#pragma once

#include "memory-mgr/manager_category.h"
#include "memory-mgr/manager_traits.h"

#include "memory-mgr/detail/decorator_base.h"

#include <new>


namespace memory_mgr
{
	namespace detail
	{
		/**
		   @brief Decorator class which stores additional Data before
		   @tparam MemMgr memory_manager class, with or w/o decorators
		*/
		template< class MemMgr, class AuxDataType >
		class aux_data_decorator
			:public decorator_base<MemMgr>
		{
			using base_type = decorator_base<MemMgr>;
		public:
			/**
			   @brief Type which represents auxilary data
			   @see static_bitset::size_type
			*/
			using aux_data_type = AuxDataType;
			using typename base_type::size_type;
			using typename base_type::decorated_mgr;


			/**
			   @brief Size of auxiliary data
			*/
			enum{ aux_data_size = sizeof( aux_data_type ) };

			/**
			   @brief Default constructor, creates memory manager 
			   @remarks Can be used only if decorates memory manager with 
						attached memory segment
			   @see memory_manager::memory_segment                        
			*/
			inline aux_data_decorator()
			{}

			/**
			   @missing_comments 
			*/
			inline explicit aux_data_decorator( void* segment_base )
				:base_type( segment_base )
			{}

			using base_type::allocate;
			using base_type::deallocate;
			/**
			  @brief Call this method to allocate memory block
			  @param size size of memory block in bytes
			  @exception bad_alloc if manager went out of memory
			  @return pointer to allocated memory block
			*/
			inline void* allocate( size_type size )
			{
				return decorated_mgr::allocate( size + this->aux_data_size );
			}

			/**
			   @brief Call this method to allocate memory block
			   @param size    size of memory block in bytes 
			   @param nothrow  unused parameter, just to overload existing
							   function
			   
			   @exception newer  throws
			   @return pointer to allocated memory block         
			*/
			inline void* allocate( size_type size, const std::nothrow_t& nothrow )/*throw()*/
			{
				return decorated_mgr::allocate( size + this->aux_data_size, nothrow );
			}

			/**
			   @brief Call this method to deallocate memory block
			   @param ptr  pointer to memory block, returned by allocate method
			   @param size size of the object allocated by allocate method
			   @exception newer  throws
			*/
			inline void deallocate( const void* ptr, size_type size )
			{
				decorated_mgr::deallocate( get_aux_data( ptr, 0 ), size + this->aux_data_size );
			}
			
			/**
			   @brief Puts auxiliary data at the beginning of memory block
			  
			   @param	ptr	pointer to allocated memory block
			   @param	size	size of allocated memory block 
			   @exception newer throws
			  
			   @return pointer to first byte after stored auxiliary data
			*/
			inline void* store_aux_data( void* ptr, aux_data_type aux_data )
			{
				//Store size
				if( ptr )
				{
					aux_data_type* pdata = static_cast<aux_data_type*>( ptr );
					*pdata = aux_data;
					return ++pdata;
				}
				return ptr;
			}

			inline const void* get_aux_data( const void* ptr, aux_data_type* aux_data )
			{
				const aux_data_type* pdata = NULL;
				if( ptr )
				{
					pdata = static_cast<const aux_data_type*>( ptr );
					--pdata;
					
					if( aux_data )
					{
						*aux_data = *pdata;
					}
				}
				return pdata;
			}
		};
	}
}

