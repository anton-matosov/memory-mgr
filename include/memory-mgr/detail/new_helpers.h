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

#	pragma once

#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/manager_category.h>

#include <cstddef>


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
				class SizeTrackingSupported
			>
		struct new_helpers
		{
			typedef MemMgr mgr_type;

			static inline void* allocate( size_t /*size*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
			}

			static inline void* allocate( size_t /*size*/, mgr_type& /*mgr*/, const char* /*name*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
			}

			static inline void destroy_and_deallocate( void* /*p*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
			}

			static inline void destroy_and_deallocate( void* /*p*/, mgr_type& /*mgr*/, const char* /*name*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
			}

			static inline void destroy_and_deallocate_array( void* /*p*/, mgr_type& /*mgr*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
			}	

			static inline void destroy_and_deallocate_array( void* /*p*/, mgr_type& /*mgr*/, const char* /*name*/ )
			{
				STATIC_ASSERT( false, Invalid_manager_type );
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

			/**
			@brief Implementation of global operators new/new[]
			@details allocates size bytes in memory managed by mgr
			@param size size of memory block in bytes
			@param mgr memory manager that will be used for memory allocation
			@exception bad_alloc if manager went out of memory
			@return pointer to allocated memory block  
			*/
			static inline void* allocate( size_t size, mgr_type& mgr )
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
			static inline void* allocate( size_t size, mgr_type& mgr, const char* name )
			{
				STATIC_ASSERT( (is_category_supported< mgr_type, named_objects_manager_tag>::value),		
					Memory_manager_does_not_implement_named_objects_concept );
				return mgr.allocate( size, name );
			}


			/**
			@brief Base class for delete helpers template classes specializations
			@details Delete helper is required, because there is no need to call destructor for the
			built in types. One more reason is that destructor of void can't be called at all.
			*/
			template<bool IsClass>
			struct delete_helper_impl{};
			
			///Delete helper class for arrays of class objects
			template<>
			struct delete_helper_impl<true>
			{
				template<class T>
				static inline size_t get_objects_count( const T* p )
				{
					const size_t* full_size = size_cast(p) - 1;
					return *full_size / sizeof(T);
				}

				template<class T>
				static inline void destroy( T* ptr )
				{
					ptr;//VS 2008 warning
					(*ptr).~T();
				}

				template<class T>
				static inline void destroy_array( T* p )
				{
					size_t count = get_objects_count( p );
					for( size_t i = 0; i < count; ++i )
					{
						destroy( p + i );
					}
				}
			};

			///Delete helper class for arrays of non class objects
			template<>
			struct delete_helper_impl<false>
			{
				static inline void destroy( const void* /*p*/ )
				{
				}

				static inline void destroy_array( const void* /*p*/ )
				{
				}
			};

			template<class T>
			struct delete_helper
				:delete_helper_impl< type_manip::is_class< T >::value >
			{
			};

			/**
			@brief Implementation of global operator delete_
			@details deallocates memory that was allocated by new operator,
			and calls destructor before deallocation
			@param	p	pointer memory that was allocated by new operator 
			@param	mgr memory manager that was used for memory allocation
			@exception newer throws
			*/
			template<class T>
			static inline void destroy_and_deallocate( const T* p, mgr_type& mgr )
			{
				delete_helper<T>::destroy( p );
				return mgr.deallocate( p );
			}

			///an overload for named objects
			template<class T>
			static inline void destroy_and_deallocate( const T* p, mgr_type& mgr, const char* name )
			{
				delete_helper<T>::destroy( p );
				return mgr.deallocate( p, 0, name );
			}

			/**
			@brief Implementation of global operator delete_arr
			@details deallocates memory that was allocated by new[] operator
			and calls destructor of array objects, if needed
			@param	p	pointer memory that was allocated by new[] operator 
			@param	mgr memory manager that was used for memory allocation
			@exception newer throws
			*/
			template<class T>
			static inline void destroy_and_deallocate_array( const T* p, mgr_type& mgr )
			{
				delete_helper<T>::destroy_array( p );
				return mgr.deallocate( p );
			}

			///an overload for named objects
			template<class T>
			static inline void destroy_and_deallocate_array( const T* p, mgr_type& mgr, const char* name )
			{
				delete_helper<T>::destroy_array( p );
				return mgr.deallocate( p, 0, name );
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
			mgr_type& m_mgr;
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
