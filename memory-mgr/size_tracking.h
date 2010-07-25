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

#ifndef MGR_SIZE_TRACKING_HEADER
#define MGR_SIZE_TRACKING_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif


#include <memory-mgr/manager_traits.h>
#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/decorator_base.h>
#include <memory-mgr/detail/ptr_helpers.h>

namespace memory_mgr
{
	namespace detail
	{
		/**
		   @brief Base class for size_tracking implementation classes
		   @tparam MemMgr memory_manager class, with or w/o decorators
		*/
		template< class MemMgr >
		class size_tracking_impl_base
			:public decorator_base<MemMgr>
		{
			typedef decorator_base<MemMgr> base_type;
		protected:
			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
			typedef typename base_type::size_type			size_type;


			/**
			   @brief Size of auxiliary data required to store size
			*/
			enum{ aux_data_size = sizeof( size_type ) };

			/**
			   @brief Default constructor, creates memory manager 
			   @remarks Can be used only if decorates memory manager with 
						attached memory segment
			   @see memory_manager::memory_segment                        
			*/
			inline size_tracking_impl_base()
			{}

			/**
			   @missing_comments 
			*/
			inline explicit size_tracking_impl_base( void* segment_base )
				:base_type( segment_base )
			{}
			
			/**
			   @brief Puts memory block size at the beginning of memory block
			  
			   @param	ptr	pointer to allocated memory block
			   @param	size	size of allocated memory block 
			   @exception newer throws
			  
			   @return pointer to first byte after stored size
			*/
			inline void* store_size( void* ptr, size_type size )
			{
				//Store size
				if( ptr )
				{
					size_type* psize = detail::size_cast( ptr );
					*psize = size;
					return ++psize;
				}
				return ptr;
			}

		};

		/**
		   @brief Base class for size tracking implementation template classes specializations
		*/
		template<class MemMgr, class pointer_conv_flag >
		class size_tracking_impl;

		/**
		   @brief Size tracking implementation for memory manager that supports PointerConverterConcept
		*/
		template< class MemMgr >
		class size_tracking_impl<
			MemMgr,
			yes_type /* PointerConverterConcept supported*/ >
			: public size_tracking_impl_base< MemMgr >
		{
			/**
			   @brief Memory manager class that should be decorated
			*/
			typedef MemMgr								mgr_type;

			/**
			   @brief size tracking implementation base class
			*/
			typedef size_tracking_impl_base< mgr_type >	impl_base_type;

		protected:
			/**
			   @brief Default constructor, creates memory manager 
			   @remarks Can be used only if decorates memory manager with 
						attached memory segment
			   @see memory_manager::memory_segment                        
			*/
			inline size_tracking_impl()
			{}

			/**
			   @brief Protected constructor, simply passes parameters to base class' constructor
			  
			   @param	segment_base	Pointer to memory which will be managed by
									manager
			   @exception newer throws
			*/
			inline explicit size_tracking_impl( void* segment_base )
				:impl_base_type( segment_base )
			{}

		public:
			/**
			   @brief Type used to store size, commonly std::size_t
			   @see static_bitset::size_type
			*/
			typedef typename impl_base_type::size_type			size_type;
			
			/**
			  @brief Call this method to allocate memory block
			  @param size size of memory block in bytes
			  @exception bad_alloc if manager went out of memory
			  @return pointer to allocated memory block
			*/
			inline void* allocate( size_type size )
			{			
				size += this->aux_data_size;
				return store_size( this->m_mgr.allocate( size ), size );
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
				size += this->aux_data_size;
				return store_size( this->m_mgr.allocate( size, nothrow ), size );
			}

			/**
			   @brief Call this method to deallocate memory block
			   @param ptr  pointer to memory block, returned by allocate method
			   @param size   this value is ignored
			   @exception newer  throws
			*/
			inline void deallocate( void* ptr, size_type /*size*/ = 0)
			{
				if( ptr )
				{
					char* ptr_base = this->m_mgr.get_ptr_base(ptr);
					assert( (ptr >= ptr_base) && "Invalid pointer value");
					assert( (ptr < ( ptr_base + manager_traits<mgr_type>::memory_size ))  && "Invalid pointer value" );

					size_type *ps = detail::size_cast( ptr );
					--ps;
					this->m_mgr.deallocate( ps, *ps );
				}
			}
		};
	}

	/**
	   @brief Size tracking decorator for memory manager.
	   @details Tracks size of allocated memory blocks, so when
	   deallocating them there is no need to specify size of memory
	   block
	   @tparam MemMgr  memory_manager class, with or w/o decorators,
					must support MemoryManagerConcept.
	                If supports PointerConverterConcept then tracking
	                operations will work faster                    
	*/
	template< class MemMgr >
	class size_tracking 
		: public detail::size_tracking_impl
		< 
			MemMgr, 
			yes_type 
		>
	{
		/**
		   @brief Memory manager class that should be decorated
		*/
		typedef MemMgr									mgr_type;

		/**
		   @brief Size tracking implementation type
		*/
		typedef detail::size_tracking_impl
		< 
			mgr_type, 
			yes_type
		>	impl_type;

	public:
		/**
		   @brief Default constructor, creates memory manager 
		   @remarks Can be used only if decorates memory manager with 
					attached memory segment
		   @see memory_manager::memory_segment                        
		*/
		inline size_tracking()
		{}

		/**
		   @brief Constructor, creates memory manager with specified
		   base address
		   @param segment_base  Pointer to memory which will be managed by
		                    manager
		   @see memory_manager::memory_manager                        
		*/
		inline explicit size_tracking( void* segment_base )
			:impl_type( segment_base )
		{}
	};

	/**
	   @brief memory_manager + size_tracking traits
	   @details Adds size_tracking_tag to manager_category
	*/
	template<class MemMgr>
	struct manager_traits< size_tracking< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;

		typedef manager_traits<base_manager_type> base_traits;

		/**
		   @brief Add size_tracking_tag to manager_category
		*/
		struct manager_category 
			: public virtual manager_traits<MemMgr>::manager_category,
			public virtual size_tracking_tag
		{};

		enum
		{
			memory_overhead = base_traits::memory_overhead + sizeof( typename base_traits::size_type ) /**< memory overhead per allocated memory block in bytes*/
		};
	};
}

#endif //MGR_SIZE_TRACKING_HEADER
