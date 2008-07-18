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

#ifndef MGR_OFFSET_POINTER_HEADER
#define MGR_OFFSET_POINTER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/offset_traits.h>
#include <memory-mgr/detail/cmp_helper.h>
#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/pointer_convert.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/new.h>

namespace memory_mgr
{	
	//Offset pointer class
	template< class T, class Mgr >
	class offset_pointer : public detail::cmp_helper< offset_pointer< T, Mgr > >
	{		
	public:
		typedef offset_pointer							self_type;		
		typedef self_type&								self_ref_type;
		typedef const self_type							const_self_type;
		typedef const_self_type&						const_self_ref_type;

		typedef Mgr										mgr_type;
		typedef manager_traits<mgr_type>				manager_traits;
		typedef typename manager_traits::offset_type	offset_type;

		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;

		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		typedef std::ptrdiff_t		difference_type;

		//Default constructor
		//Constructs null pointer
		inline offset_pointer()
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{}

		//Construct pointer from offset
		inline explicit offset_pointer( const offset_type offset )
			:m_offset( offset )
		{}

		//Copy constructor
		inline offset_pointer( const offset_pointer& ptr )
			:m_offset( ptr.m_offset )
		{}

		//Pointer constructor
		inline offset_pointer( const_pointer p )
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{
			do_set_pointer(p);
		}

		//Polymorph copy constructor
		template < typename U >
		inline offset_pointer( const offset_pointer< U, mgr_type >& ptr )
			:m_offset( get_offset( ptr ) )
		{
			STATIC_ASSERT( ( type_manip::super_subclass<T, U>::value ), invalid_conversion );
		}

		//Polymorph copy operator
		template < typename U >
		inline offset_pointer& operator=( const offset_pointer< U, mgr_type >& ptr )			
		{
			STATIC_ASSERT( ( type_manip::super_subclass<T, U>::value ), invalid_conversion );
			m_offset = get_offset( ptr );			
			return *this;
		}

		inline pointer operator->()
		{
			return get_pointer();
		}

		inline const_pointer operator->() const
		{
			return get_pointer();
		}

		inline reference operator*()
		{
			return *get_pointer();
		}

		inline const_reference operator*() const
		{
			return *get_pointer();
		}

		inline pointer operator&()
		{
			return get_pointer();
		}

		inline const_pointer operator&() const
		{
			return get_pointer();
		}

		inline reference operator[](difference_type n)
		{
			return *(get_pointer() + n);
		}

		inline const_reference operator[](difference_type n) const
		{
			return *(get_pointer() + n);
		}

// 		operator T* ()
// 		{
// 			return get_pointer();
// 		}
// 
// 		operator const T*() const
// 		{
// 			return get_pointer();
// 		}

		bool is_null() const { return m_offset == offset_traits<offset_type>::invalid_offset; }
		bool is_not_null() const { return !is_null(); }
		bool operator!() const { return  is_null(); }

		inline self_ref_type operator++()
		{
			return *this += 1;
		}

		inline self_type operator++(int)
		{
			offset_type offset = m_offset;
			++*this;
			return self_type( offset );
		}

		inline self_ref_type operator--()
		{			
			return *this -= 1;
		}

 		inline self_type operator--(int)
 		{
 			offset_type offset = m_offset;
 			--*this;
 			return self_type( offset );
 		}

		inline self_ref_type operator+=(difference_type n)
		{
			do_set_pointer( get_pointer() + n );
			return *this;
		}

		inline self_ref_type operator-=(difference_type n)
		{
			do_set_pointer( get_pointer() - n );
			return *this;
		}

		inline self_type operator+(difference_type n) const
		{
			return self_type( get_pointer() + n );
		}

		inline self_type operator-(difference_type n) const
		{
			return self_type( get_pointer() - n );
		}

		inline difference_type operator-( const self_type& ptr ) const
		{
			return get_pointer() - ptr.get_pointer();
		}
		
		inline bool operator==( const self_type& rhs ) const
		{
			return m_offset == rhs.m_offset;
		}

		inline bool operator<( const self_type& rhs ) const
		{
			return m_offset < rhs.m_offset;
		}

		inline bool operator>( const self_type& rhs ) const
		{
			return m_offset > rhs.m_offset;
		}
	private:
		offset_type m_offset;

		typedef pointer_convert<typename manager_traits::manager_type> converter;

		inline pointer unconst_pointer( const_pointer ptr )
		{
			return const_cast<pointer>( ptr );
		}

		inline pointer get_pointer()
		{
			return unconst_pointer( do_get_pointer() );
		}

		inline const_pointer get_pointer() const
		{
			return do_get_pointer();
		}

		inline const_pointer do_get_pointer() const
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, singleton_manager_tag>::value), Memory_manager_should_be_singleton_manager );
			return static_cast<const_pointer>( detail::offset_to_pointer( m_offset, mgr_type::instance() ) );
		}

		inline void do_set_pointer( const_pointer ptr )
		{
			STATIC_ASSERT( (is_category_supported< mgr_type, singleton_manager_tag>::value), Memory_manager_should_be_singleton_manager );
			m_offset = detail::pointer_to_offset( ptr, mgr_type::instance() );
		}

	//friends:
		//Call this method to get offset
		friend inline const offset_type get_offset( const_self_ref_type ptr )
		{
			return ptr.m_offset;
		}

		friend inline pointer get_pointer( self_ref_type ptr )
		{
			return ptr.get_pointer();
		}

		friend inline const_pointer get_pointer( const_self_ref_type ptr )
		{
			return ptr.get_pointer();
		}		
	};

	template< class T, class Mgr >
	static inline offset_pointer<T, Mgr> operator+( typename offset_pointer<T, Mgr>::difference_type n, const offset_pointer<T, Mgr>& ptr )
	{
		return offset_pointer<T, Mgr>( get_pointer(ptr) + n );
	}
	
	template<class T, class Mgr>
	static inline void delete_( offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_( get_pointer(ptr), mem_mgr(Mgr::instance() ) );
	}

	template<class T, class Mgr>
	static inline void delete_array( offset_pointer<T, Mgr>& ptr )
	{
		return ::delete_array( get_pointer(ptr), mem_mgr(Mgr::instance() ) );
	}
}


#endif// MGR_OFFSET_POINTER_HEADER
