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

#ifndef MGR_NEW_HELPERS_HEADER
#define MGR_NEW_HELPERS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/static_assert.h>

namespace memory_mgr
{
	namespace detail
	{
		/**
		@brief New/delete implementation base
		@details Bomb-class, all methods generates compile time error
		*/
		template
			<
				class MemMgr,
				class SizeTrackingSupp
			>
		struct new_helpers
		{
			/**
			@brief Bomb-method for global new, generates compile time error if called
			*/
			typedef MemMgr mgr_type;
			static inline void* new_impl( size_t /*size*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}

			/**
			@brief Bomb-method for global new for named objects, generates compile time error if called
			*/
			static inline void* new_impl( size_t /*size*/, mgr_type& /*mgr*/, const char* /*name*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}

			/**
			@brief Bomb-method for global delete_, generates compile time error if called
			*/
			static inline void delete_impl( void* /*p*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}

			/**
			@brief Bomb-method for global delete_arr, generates compile time error if called
			*/
			static inline void delete_arr_impl( void* /*p*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}			
		};

		/**
		@brief Real New/delete implementation class
		@details It is a template specification that used if memory manager 
		implements PointerConverterConcept and SizeTrackingConcept
		*/
		template< class MemMgr >
		struct new_helpers<MemMgr, yes_type>
		{
			typedef MemMgr mgr_type;



			template<class T>
			static inline void construct( T* p )
			{
				new( p ) T();
			}

			template<class T>
			static inline void construct( T* p, size_t count )
			{
				for( size_t i = 0; i < count; ++i )
				{
					construct( p + i );
				}
			}

			template<class T>
			static inline void destroy( T* p )
			{
				p->~T();
			}

			template<class T>
			static inline void destroy( T* p, size_t count )
			{
				for( size_t i = 0; i < count; ++i )
				{
					destroy( p + i );
				}
			}

			/**
			@brief Implementation of global operators new/new[]
			@details allocates size bytes in memory managed by mgr
			@param size size of memory block in bytes
			@param mgr memory manager that will be used for memory allocation
			@exception bad_alloc if manager went out of memory
			@return pointer to allocated memory block  
			*/
			static inline void* new_impl( size_t size, mgr_type& mgr )
			{
				return mgr.allocate( size );
			}

			/**
			@brief Implementation of global operators new/new[]
			@details allocates size bytes in memory managed by mgr
			@param size size of memory block in bytes
			@param mgr memory manager that will be used for memory allocation
			@exception bad_alloc if manager went out of memory
			@return pointer to allocated memory block  
			*/
			static inline void* new_impl( size_t size, mgr_type& mgr, const char* name )
			{
				STATIC_ASSERT( (is_category_supported< mgr_type, named_objects_manager_tag>::value),		
					Memory_manager_does_not_implement_named_objects_concept );
				return mgr.allocate( size, name );
			}

			/**
			@brief Implementation of global operator delete_
			@details deallocates memory that was allocated by new operator,
			and calls destructor before deallocation
			@param	p	pointer memory that was allocated by new operator 
			@param	mgr memory manager that was used for memory allocation
			@exception newer throws
			*/
			template<class T>
			static inline void delete_impl( T* p, mgr_type& mgr )
			{
				p->~T();
				return mgr.deallocate( p );
			}

			/**
			@brief Base class for delete helpers template classes specializations
			@details Delete helper is required because C++ compiler uses memory for arrays
			in different manner for class array and non class arrays
			*/
			template<bool IsClass = type_manip::is_class< T >::value>
			struct delete_helper{};

			typedef std::pair<const void*, size_t> ptr_and_count_pair;
			/**
			@brief Delete helper class for arrays of class objects
			*/
			template<>
			struct delete_helper<true>
			{

				/**
				@brief Call this method to get real base pointer 
				of memory block and number of array objects				  
				@param	p	pointer received in the delete[] call			  
				@return std::pair<real base pointer of memory block, number of array objects>
				*/
				static inline ptr_and_count_pair get_ptr_and_count( const void* p )
				{
					const size_t* count = size_cast(p) - 1;
					return ptr_and_count_pair( count, *count );
				}
			};

			/**
			@brief Delete helper class for arrays of non class objects
			@note Returns zero as a count, because there is no need to call destructor for the
			built in types
			*/
			template<>
			struct delete_helper<false>
			{
				/**
				@brief Call this method to get real base pointer 
				of memory block and number of array objects				  
				@param	p	pointer to array's memory
				@exception newer throws				  
				@return std::pair<real base pointer of memory block, number of array objects>
				*/
				static inline ptr_and_count_pair get_ptr_and_count( const void* p )
				{
					return ptr_and_count_pair( p, 0 );
				}
			};

			/**
			@brief Implementation of global operator delete_arr
			@details deallocates memory that was allocated by new[] operator
			and calls destructor of array objects, if needed
			@param	p	pointer memory that was allocated by new[] operator 
			@param	mgr memory manager that was used for memory allocation
			@exception newer throws
			*/
			template<class T>
			static inline void delete_arr_impl( T* p, mgr_type& mgr )
			{				
				ptr_and_count_pair ptr_and_count = 
					delete_helper< type_manip::is_class< T >::value >::get_ptr_and_count( p );

				destroy( p, ptr_and_count.second );

				return mgr.deallocate( ptr_and_count.first );
			}

			///A destructor of void can't be called, so we need to treat void* array in different manner
 			static inline void delete_arr_impl( const void* p, mgr_type& mgr )
 			{
 				return mgr.deallocate( p );
 			}
		};


		/**
		@brief Helper class for global overloaded new/delete operators
		@details stores reference to memory manager and validates memory manager class
		@tparam MemMgr Memory manager type that should be checked,
		must implement PointerConverterConcept and SizeTrackingConcept
		*/
		template<class MemMgr> 
		class mem_mgr_wrapper
		{
			/**
			@brief Private copy operator. Declared to prevent warning messages
			@details Declared by not defined
			*/
			mem_mgr_wrapper& operator=( const mem_mgr_wrapper& );
		public:
			/**
			@brief Memory manager type that should be checked
			*/
			typedef MemMgr mgr_type;

			/**
			@brief Size tracking category check result
			*/
			typedef ::memory_mgr::is_category_supported
				<
				mgr_type, typename ::memory_mgr::size_tracking_tag
				> size_tracking_check;

			/**
			@brief New/delete implementation class
			*/
			typedef new_helpers
				<
				mgr_type,
				typename size_tracking_check::result
				> new_helper_type;


			/**
			@brief Constructor, stores reference to memory manager
			@details Generates compile time error if memory manager does not
			support required categories
			@param mgr reference to memory manager that should be used by new/delete operators
			*/
			mem_mgr_wrapper( mgr_type& mgr )
				:m_mgr( mgr )
			{
				STATIC_ASSERT( size_tracking_check::value, 
					Memory_manager_does_not_implement_size_tracking_concept );
			}

			mgr_type& get() const
			{
				return m_mgr;
			}
		private:			
			/**
			@brief Reference to memory manager
			@details that was passed as constructor parameter
			*/
			mutable mgr_type& m_mgr;
		};


	}

	/**
	@brief Helper function for global operator new overloads
	@details used to pass memory manager object to operator new
	@param mgr memory manager object that should be used by operator new
	@exception newer throws
	@return helper object that should be passed as parameter to operator new
	*/	
	template<class MemMgr>
	static inline detail::mem_mgr_wrapper<MemMgr> mem_mgr( MemMgr& mgr )
	{
		typedef MemMgr mgr_type;
		return detail::mem_mgr_wrapper<mgr_type>( mgr );
	};

	/**
	@brief Helper class, used to pass object's name to new/new[] operators
	*/
	class object_name			
	{	
		/**
		@brief Pointer to null terminated string that stores object's name
		*/
		const char* m_name;
	public:
		/**
		@brief Constructor 
		@param name Pointer to null terminated string that stores object's name

		@warning Pointer, passed as name parameter, must 
		be valid until the end of allocation operation.
		*/
		object_name( const char* name )
			: m_name( name )
		{}


		/**
		@brief Call this method to get stored name

		@exception newer throws

		@return Pointer to null terminated string that stores object's name 
		*/
		const char* get_name() const
		{
			return m_name;
		}
	};
}

#endif //MGR_NEW_HELPERS_HEADER
