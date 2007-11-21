#pragma once

#include "detail/type_manip.h"
#include "detail/static_assert.h"

namespace managers
{
	//Forward declaration
 	template < typename T, typename MemMgr > 
 	class simple_ptr;

	namespace detail
	{
		template< typename T, typename MemMgr >
		typename const MemMgr::ptr_t& get_ptr( const simple_ptr< T, MemMgr >& ptr )
		{
			return ptr.m_ptr;
		}


		template < class BaseT, class DerivedT >
		const BaseT* poly_cast( const DerivedT* p = NULL )
		{
			//Polimorph test
			return p;
		}

	}

	//This class is a basic class for all smart pointers of this library
	//T - is a class pointer to which is owned by class
	//MemMgr - is a singletoned manager which will be used for memory allocations/deallocations
	template < typename T, typename MemMgr > 
	class simple_ptr
	{
		template< typename T, typename MemMgr >
		friend typename const MemMgr::ptr_t& detail::get_ptr( const simple_ptr< T, MemMgr >& ptr );
	public:
		typedef MemMgr mgr_t;
		
		typedef T					value_t;
		typedef value_t*			pointer_t;
		typedef const value_t*		const_pointer_t;

		typedef value_t&			reference_t;
		typedef const value_t&		const_reference_t;

		typedef simple_ptr< value_t, mgr_t > self_type;

		//friend class simple_ptr< value_t, mgr_t >;

		//Default constructor
		simple_ptr()
			:m_ptr( mgr_t::null_ptr )
		{

		}

		//Constructor from common pointer
		explicit simple_ptr( value_t* ptr )
			:m_ptr( mgr_t::instance(), ptr )
		{

		}

		//Polymorph copy constructor
		template < typename U >
		simple_ptr( const simple_ptr< U, mgr_t >& ptr )
			:m_ptr( detail::get_ptr( ptr ) )
		{
			STATIC_ASSERT( ( type_manip::super_subclass<U, T>::value ), invalid_conversion );
		}


		
		//Copy operator
		self_type& operator=( const simple_ptr& ptr )		{			m_ptr = ptr.m_ptr;			return *this;		}
		
		//Access operators
		pointer_t operator->()
		{
			return get_poiner();
		}

		const_pointer_t operator->() const
		{
			return get_poiner();
		}

		reference_t operator*()
		{
			return *get_poiner();
		}

		const_reference_t operator*() const
		{
			return *get_poiner();
		}

		pointer_t operator&()
		{
			return get_poiner();
		}

		const_pointer_t operator&() const
		{
			return get_poiner();
		}
		
		bool is_not_null()  const { return m_ptr.get_off() != mgr_t::null_ptr.get_off(); }
		bool is_null() const { return m_ptr.get_off() == mgr_t::null_ptr.get_off(); }
		bool operator!() const { return  is_null(); }

		//It is risky to add such an operator
		//operator bool() const { return  !m_ptr.is_null(); }
		
		bool operator==(  const self_type& ptr ) const
		{
			return m_ptr.get_off() == ptr.m_ptr.get_off();
		}

		bool operator!=(  const self_type& ptr ) const
		{
			return m_ptr.get_off() != ptr.m_ptr.get_off();
		}

		bool operator<(  const self_type& ptr ) const
		{
			return m_ptr.get_off() < ptr.m_ptr.get_off();
		}

		bool operator<=(  const self_type& ptr ) const
		{
			return m_ptr.get_off() <= ptr.m_ptr.get_off();
		}

		bool operator>(  const self_type& ptr ) const
		{
			return m_ptr.get_off() > ptr.m_ptr.get_off();
		}

		bool operator>=(  const self_type& ptr ) const
		{
			return m_ptr.get_off() >= ptr.m_ptr.get_off();
		}

		
		/*
				//Call this function to construct object using default constructor	
				template < typename T, typename sh_mem_mgr >
				friend void construct( simple_ptr<T, sh_mem_mgr>& arr_ptr, const wchar_t* name );
		
				//Call this function to construct object using constructor with one parameter
				template < typename T, typename parT, typename sh_mem_mgr >
				friend void construct( simple_ptr<T, sh_mem_mgr>& arr_ptr, const wchar_t* name, parT par );
		
				//Call this function to destruct object
				template < typename T, typename sh_mem_mgr >
				friend void destruct( simple_ptr<T, sh_mem_mgr>& ptr );
				*/
		

	private:
		typedef typename mgr_t::ptr_t ptr_t;
		ptr_t m_ptr;

		inline pointer_t unconst_poiner( const_pointer_t ptr )
		{
			return const_cast<pointer_t>( ptr );
		}

		inline pointer_t get_poiner()
		{
			return unconst_poiner( do_get_poiner() );
		}

		inline const_pointer_t get_poiner() const
		{
			return do_get_poiner();
		}

		inline const_pointer_t do_get_poiner() const
		{
			return static_cast<const_pointer_t>( m_ptr.get_ptr( mgr_t::instance() ) );
		}
	};

	namespace detail
	{
// 
// 		template< typename T, typename MemMgr >
// 		const typename MemMgr::ptr_t& get_ptr( const simple_ptr< T, MemMgr >& ptr )
// 		{
// 			return ptr.m_ptr;
// 		}
	}
}