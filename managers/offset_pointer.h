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

#ifndef MGR_OFFSET_POINTER_HEADER
#define MGR_OFFSET_POINTER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/offset_traits.h"
#include "detail/cmp_helper.h"
#include "pointer_convert.h"
#include "manager_traits.h"

namespace memory_mgr
{	
// 	template <class T>
// 	class mutable_random_access_iterator_archetype
// 	{
// 	public:
// 		typedef mutable_random_access_iterator_archetype self;
// 	public:
// 		typedef std::random_access_iterator_tag iterator_category;
// 		typedef T value_type;
// 		typedef T& reference;
// 		typedef T* pointer;
// 		typedef std::ptrdiff_t difference_type;
// 		mutable_random_access_iterator_archetype() { }
// 		self& operator=(const self&) { return *this;  }
// 		bool operator==(const self&) const { return true; }
// 		bool operator!=(const self&) const { return true; }
// 		reference operator*() const { return static_object<T>::get(); }
// 		self& operator++() { return *this; }
// 		self operator++(int) { return *this; }
// 		self& operator--() { return *this; }
// 		self operator--(int) { return *this; }
// 		reference operator[](difference_type) const
// 		{ return static_object<T>::get(); }
// 		self& operator+=(difference_type) { return *this; }
// 		self& operator-=(difference_type) { return *this; }
// 		difference_type operator-(const self&) const
// 		{ return difference_type(); }
// 		self operator+(difference_type) const { return *this; }
// 		self operator-(difference_type) const { return *this; }
// 		bool operator<(const self&) const { return true; }
// 		bool operator<=(const self&) const { return true; }
// 		bool operator>(const self&) const { return true; }
// 		bool operator>=(const self&) const { return true; }
// 	};
// 	template <class T>
// 	mutable_random_access_iterator_archetype<T> 
// 		operator+
// 		(typename mutable_random_access_iterator_archetype<T>::difference_type, 
// 		const mutable_random_access_iterator_archetype<T>& x) 
// 	{ return x; }

	//Offset pointer class
	template< class T, class Mgr >
	class offset_pointer : public detail::cmp_helper< offset_pointer< T, Mgr > >
	{		
	public:
		typedef offset_pointer							self_type;
		typedef self_type&								self_type_ref;
		typedef const self_type							const_self_type;
		typedef const_self_type&						const_self_type_ref;

		typedef Mgr										mgr_type;
		typedef manager_traits<mgr_type>				manager_traits;
		typedef typename manager_traits::offset_type	offset_type;

		typedef T					value_type;
		typedef value_type*			pointer_type;
		typedef const value_type*	const_pointer_type;

		typedef value_type&			reference_type;
		typedef const value_type&	const_reference_type;

		typedef ptrdiff_t			difference_type;

		//Default constructor
		//Constructs null pointer
		offset_pointer()
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{}


		//Construct pointer from offset
		offset_pointer( const offset_type offset )
			:m_offset( offset )
		{}

		//Copy constructor
		offset_pointer( const offset_pointer& ptr )
			:m_offset( ptr.m_offset )
		{}

		//Pointer constructor
		offset_pointer( const_pointer_type p )
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{
			do_set_pointer(p);
		}

		//Polymorph copy constructor
		template < typename U >
		offset_pointer( const offset_pointer< U, mgr_type >& ptr )
			:m_offset( get_offset( ptr ) )
		{
			STATIC_ASSERT( ( type_manip::super_subclass<T, U>::value ), invalid_conversion );
		}
// 
// 		//Construct pointer from memory address
// 		offset_pointer( const mgr_type& mgr, const void* ptr )			
// 			:m_offset( detail::diff( ptr, mgr.get_base() ) )
// 		{ assert( ptr >= mgr.get_base() && "Invalid pointer value" ); }

		const_pointer_type operator->() const
		{
			return get_poiner();
		}

		reference_type operator*()
		{
			return *get_poiner();
		}

		const_reference_type operator*() const
		{
			return *get_poiner();
		}

		pointer_type operator&()
		{
			return get_poiner();
		}

		const_pointer_type operator&() const
		{
			return get_poiner();
		}

		operator T* ()
		{
			return get_poiner();
		}

		operator const T*() const
		{
			return get_poiner();
		}

		bool is_null() const { return m_offset == offset_traits<offset_type>::invalid_offset; }
		bool is_not_null() const { return !is_null(); }
		bool operator!() const { return  is_null(); }

		//It is risky to add such an operator
		//operator bool() const { return  !m_ptr.is_null(); }

		// 		self_type operator+( const size_t count ) const
		// 		{			
		// 			return self_type( get_poiner() + count );
		// 		}
		// 
		// 		self_type operator-( const size_t count ) const
		// 		{			
		// 			return self_type( get_poiner() - count );
		// 		}

		self_type& operator--()
		{			
			do_set_pointer( get_poiner() - 1 );			
			return *this;
		}

		difference_type operator-( const self_type& ptr ) const
		{			
			return get_poiner() - ptr.get_poiner();
		}
		
		//////////////////////////////////////////////////////////////////////////
		offset_pointer& operator=( const offset_pointer& ptr )				
		{
			m_offset = ptr.m_offset;
			return *this;
		}


		bool operator==( const self_type& rhs ) const
		{
			return m_offset == rhs.m_offset;
		}

		bool operator<( const self_type& rhs ) const
		{
			return m_offset < rhs.m_offset;
		}

		bool operator>( const self_type& rhs ) const
		{
			return m_offset > rhs.m_offset;
		}
	private:
		offset_type m_offset;

		typedef pointer_convert<typename manager_traits::manager_type> converter;

		inline pointer_type unconst_poiner( const_pointer_type ptr )
		{
			return const_cast<pointer_type>( ptr );
		}

		inline pointer_type get_poiner()
		{
			return unconst_poiner( do_get_poiner() );
		}

		inline const_pointer_type get_poiner() const
		{
			return do_get_poiner();
		}

		inline const_pointer_type do_get_poiner() const
		{
			return static_cast<const_pointer_type>( detail::offset_to_pointer( m_offset, mgr_type::instance() ) );
		}

		inline void do_set_pointer( const_pointer_type ptr )
		{
			m_offset = detail::pointer_to_offset( ptr, mgr_type::instance() );
		}

	//friends:
		//Call this method to get offset
		friend static inline const offset_type get_offset( const_self_type_ref ptr )
		{
			return ptr.m_offset;
		}

		friend static inline pointer_type get_poiner( self_type_ref ptr )
		{
			return ptr.get_poiner();
		}

		friend static inline const_pointer_type get_poiner( const_self_type_ref ptr )
		{
			return ptr.get_poiner();
		}
	};

	
}


#endif// MGR_OFFSET_POINTER_HEADER
