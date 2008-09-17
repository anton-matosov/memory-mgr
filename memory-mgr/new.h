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

#ifndef MGR_NEW_HEADER
#define MGR_NEW_HEADER

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
			class PointerConvSupp,
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
			typedef MemMgr mgr_type;
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
		struct new_helpers<MemMgr, yes_type,
			yes_type>
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
			template<class T, bool IsClass = type_manip::is_class< T >::value >
			struct delete_helper{};
			
			/**
			   @brief Delete helper class for arrays of class objects
			*/
			template<class T>
			struct delete_helper<T, true>
			{
				
				/**
				   @brief Call this method to get real base pointer 
							of memory block and number of array objects				  
				   @param	p	pointer to array's memory
				   @exception newer throws				  
				   @return std::pair<real base pointer of memory block, number of array objects>
				*/
				static inline std::pair<void*, size_t> get_ptr_and_count( T* p )
				{
					size_t* count = size_cast(p) - 1;
					return std::make_pair( count, *count );
				}
			};

			/**
			   @brief Delete helper class for arrays of non class objects
			*/
			template<class T>
			struct delete_helper<T, false>
			{
				/**
				   @brief Call this method to get real base pointer 
							of memory block and number of array objects				  
				   @param	p	pointer to array's memory
				   @exception newer throws				  
				   @return std::pair<real base pointer of memory block, number of array objects>
				*/
				static inline std::pair<void*, size_t> get_ptr_and_count( T* p )
				{
					size_t* size = size_cast(p) - 1;
					return std::make_pair( p, *size / sizeof( T ) );
				}
			};
			
			/**
			   @brief Implementation of global operator delete_arr
			   @details deallocates memory that was allocated by new[] operator
						and calls destructors of array objects
			   @param	p	pointer memory that was allocated by new[] operator 
			   @param	mgr memory manager that was used for memory allocation
			   @exception newer throws
			*/
			template<class T>
			static inline void delete_arr_impl( T* p, mgr_type& mgr )
			{				
				std::pair<void*, size_t> ptr_n_count = 
					delete_helper<T,
						type_manip::is_class< T >::value //VC8
					>::get_ptr_and_count(p);

				for( size_t i = 0; i < ptr_n_count.second; ++i )
				{
					p[i].~T();
				}

				return mgr.deallocate( ptr_n_count.first );
			}

			static inline void delete_arr_impl( void* p, mgr_type& mgr )
			{
				return mgr.deallocate( p );
			}
		};
	

		/**
-		   @brief Helper class for global overloaded new/delete operators
		   @details stores reference to memory manager and verifies memory manager class
		   @tparam MemMgr Memory manager type that should be checked,
							must implement PointerConverterConcept and SizeTrackingConcept
		*/
		template<class MemMgr> 
		class mem_mgr_helper
		{
			/**
			   @brief Private copy operator. Declared to prevent warning messages
			   @details Declared by not defined
			*/
			mem_mgr_helper& operator=( const mem_mgr_helper& );
		public:
			/**
			   @brief Memory manager type that should be checked
			*/
			typedef MemMgr mgr_type;

			/**
			   @brief Pointer conversion category check result
			*/
			typedef ::memory_mgr::is_category_supported
				<
					mgr_type, typename ::memory_mgr::pointer_conversion_tag
				> pointer_conv_check;

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
					typename size_tracking_check::result,
					typename pointer_conv_check::result
				> new_helper_type;

			/**
			   @brief Reference to memory manager
			   @details that was passed as constructor parameter
			*/
			mutable mgr_type& m_mgr;

			/**
			   @brief Constructor, stores reference to memory manager
			   @details Generates compile time error if memory manager does not
						support required categories
			   @param mgr reference to memory manager that should be used by new/delete operators
			*/
			mem_mgr_helper( mgr_type& mgr )
				:m_mgr( mgr )
			{
				STATIC_ASSERT( pointer_conv_check::value, 
					Memory_manager_does_not_implement_pointer_convertion_concept );
				STATIC_ASSERT( size_tracking_check::value, 
					Memory_manager_does_not_implement_size_tracking_concept );
			}
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
	static inline detail::mem_mgr_helper<MemMgr> mem_mgr( MemMgr& mgr )
	{
		typedef MemMgr mgr_type;
		return detail::mem_mgr_helper<mgr_type>( mgr );
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

/**
   @brief Overloaded operator new, allocates memory block in memory managed passed as second parameter
   @param size size of memory block required to store object,
				this parameter is passed by compiler automatically
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception bad_alloc if manager went out of memory  
   @return pointer to allocated memory block
*/
template<class MemMgr>
inline void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;
	
	return helper_type::new_impl( size, mgr.m_mgr );
}

template<class MemMgr>
inline void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& name )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size, mgr.m_mgr, name.get_name() );
}

/**
   @brief Overloaded operator new[], allocates memory block in memory managed by mem_mgr
   @param size size of memory block required to store array,
				this parameter is passed by compiler automatically
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception bad_alloc if manager went out of memory
   @return pointer to allocated memory block
*/
template<class MemMgr>
inline void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size, mgr.m_mgr );
}

template<class MemMgr>
inline void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& name )
{
	typedef MemMgr mgr_type;
	typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

	return helper_type::new_impl( size, mgr.m_mgr, name.get_name() );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief Overloaded operator delete, deallocates memory block in memory managed by mem_mgr
@param p pointer to memory block
@param size size of memory block that should be deallocated
@exception newer throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
static inline void operator delete( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}

/**
@brief Overloaded operator delete for named objects,
deallocates memory block in memory managed by mem_mgr

@param p pointer to memory block
@param size size of memory block that should be deallocated
@param name name of the object that should be deallocated
@exception newer  throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
static inline void operator delete( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& /*name*/ )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief Overloaded operator delete[], deallocates memory block in memory managed by mem_mgr
@param p pointer to memory block
@param size size of memory block that should be deallocated
@exception newer throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
static inline void operator delete[]( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

/**
@brief Overloaded operator delete[] for named objects,
deallocates memory block in memory managed by mem_mgr

@param p pointer to memory block
@param size size of memory block that should be deallocated
@param name name of the object that should be deallocated
@exception newer  throws
@remark all parameters are passed by compiler automatically
*/
template<class MemMgr>
static inline void operator delete[]( void* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr, const memory_mgr::object_name& /*name*/ )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

/**
   @brief Sort of overloaded operator delete, deallocates memory block in memory managed by mem_mgr
   @param p pointer to memory block
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception newer throws
*/
template<class T, class MemMgr>
static inline void delete_( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;

		helper_type::delete_impl( p, mgr.m_mgr );
	}
}

/**
   @brief Sort of overloaded operator delete[], deallocates memory block in memory managed by mem_mgr
   @param p pointer to memory block
   @param mgr helper object returned by mem_mgr( mgr_obj ) function 
   @exception newer throws
*/
template<class T, class MemMgr>
static inline void delete_array( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	if( p )
	{
		typedef MemMgr mgr_type;
		typedef typename memory_mgr::detail::mem_mgr_helper<mgr_type>::new_helper_type helper_type;
		
		helper_type::delete_arr_impl( p, mgr.m_mgr );
	}
}

#endif //MGR_NEW_HEADER
