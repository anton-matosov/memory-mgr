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

#ifndef MGR_self_offset_ptr_HEADER
#define MGR_self_offset_ptr_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/offset_traits.h>
#include <memory-mgr/detail/cmp_helper.h>
#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/new.h>

#include <boost/type_traits/add_reference.hpp>

namespace memory_mgr
{	
	/**
		@brief		Offset pointer class.
		@details	Smart pointer class which stores offset between 'this' and object it points.
					This allows to put this pointer	to the shared memory and/or
					memory mapped file which are mapped to the different base 
					addresses in different processes.

		@note		Declaration examples:
					offset_ptr<char>		char_ptr;
					offset_ptr<const char>	const_char_ptr;
	*/
	template< class T >
	class offset_ptr 
		: public detail::cmp_helper< offset_ptr< T > >
	{		
	public:
		typedef std::ptrdiff_t							offset_type;

		typedef offset_ptr							self_type;		
		typedef self_type&								self_ref_type;
		typedef const self_type							const_self_type;
		typedef const_self_type&						const_self_ref_type;

		//These typedefs are required if this class is used in the containers as iterator
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;

		typedef typename boost::add_reference<value_type>::type			reference;
		typedef typename boost::add_reference<const value_type>::type	const_reference;

		typedef std::ptrdiff_t						difference_type;
		
		typedef std::random_access_iterator_tag		iterator_category;

	//friends:
		//Call this method to get offset
		friend inline const offset_type get_offset_internal( const_self_ref_type ptr )
		{
			return ptr.m_offset;
		}

		friend inline pointer get_pointer_internal( self_ref_type ptr )
		{
			return ptr.get();
		}

		friend inline const_pointer get_pointer_internal( const_self_ref_type ptr )
		{
			return ptr.get();
		}	

		//Default constructor
		//Constructs null pointer
		inline offset_ptr()
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{}

		//Construct pointer from offset
// 		inline explicit offset_ptr( const offset_type offset )
// 			:m_offset( offset )
// 		{}

		//Copy constructor
		inline offset_ptr( const offset_ptr& ptr )
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{
			do_set_pointer( ptr.get() );
		}

		//Pointer constructor
		inline offset_ptr( const_pointer p )
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{
			do_set_pointer(p);	
		}

		//Polymorph copy constructor
		template < typename U >
		inline offset_ptr( const offset_ptr< U >& ptr )
			:m_offset( offset_traits<offset_type>::invalid_offset )
		{
			do_set_pointer( ptr.get() );

			STATIC_ASSERT( ( type_manip::super_subclass<T, U>::value ), invalid_conversion );
		}

		//////////////////////////////////////////////////////////////////////////
		//Polymorph copy operators
		inline offset_ptr& operator=( const_pointer p )			
		{
			do_set_pointer(p);				
			return *this;
		}

		inline offset_ptr& operator=( const offset_ptr& ptr )			
		{
			do_set_pointer( ptr.get() );
			return *this;
		}

		//Polymorph copy operator
		template < typename U >
		inline offset_ptr& operator=( const offset_ptr< U >& ptr )			
		{
			STATIC_ASSERT( ( type_manip::super_subclass<T, U>::value ), invalid_conversion );
			do_set_pointer( ptr.get() );
			return *this;
		}
		//////////////////////////////////////////////////////////////////////////

		inline pointer operator->() const
		{
			return this->get();
		}

		inline reference operator*() const
		{
			return *this->get();
		}


		inline reference operator[](difference_type n) const
		{
			return *(this->get() + n);
		}

		bool is_null() const
		{ 
			return m_offset == offset_traits<offset_type>::invalid_offset;
		}

		bool is_not_null() const
		{ 
			return !is_null();
		}

		bool operator!() const
		{ 
			return  is_null();
		}

		inline self_ref_type operator++()
		{
			return *this += 1;
		}

		inline self_type operator++(int)
		{
			self_type temp(*this);
			++*this;
			return temp; ;
		}

		inline self_ref_type operator--()
		{			
			return *this -= 1;
		}

 		inline self_type operator--(int)
 		{
 			self_type temp(*this);
 			--*this;
 			return temp;
 		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline self_ref_type operator+=(difference_type n)
		{
			this->do_set_pointer( this->get() + n );
			return *this;
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline self_ref_type operator-=(difference_type n)
		{
			this->do_set_pointer( this->get() - n );
			return *this;
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline self_type operator+(difference_type n) const
		{
			return self_type( this->get() + n );
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline self_type operator-(difference_type n) const
		{
			return self_type( this->get() - n );
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline difference_type operator-( const self_type& ptr ) const
		{
			return this->get() - ptr.get();
		}
				
		/**
		@brief
		@exception Never throws.
		*/
		inline bool operator==( const self_type& rhs ) const
		{
			return this->get() == rhs.get();
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline bool operator<( const self_type& rhs ) const
		{
			return this->get() < rhs.get();
		}
		
		/**
		@brief
		@exception Never throws.
		*/
		inline bool operator>( const self_type& rhs ) const
		{
			return this->get() > rhs.get();
		}

		/**
		@brief Obtains raw pointer from offset.
		@exception Never throws.
		*/
		pointer get()const
		{  
			return this->get_pointer_internal();
		}
	private:

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		//__declspec(noinline) //this workaround is needed for msvc-8.0 and msvc-9.0
#endif
		void do_set_pointer( const_pointer ptr )
		{
			//offset == invalid_offset1 && ptr != 0 is not legal for this pointer
			if( ptr == NULL )
			{
				m_offset = offset_traits<offset_type>::invalid_offset;
			}
			else
			{
				m_offset = detail::diff( ptr, this );
				assert( (m_offset != offset_traits<offset_type>::invalid_offset) && "Invalid offset value" );
			}
		}

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		//__declspec(noinline) //this workaround is needed for msvc-8.0 and msvc-9.0
#endif
		const_pointer do_get_pointer() const
		{  
			if( m_offset == offset_traits<offset_type>::invalid_offset )
			{
				return 0;
			}
			return static_cast<const_pointer>( detail::shift( this, m_offset ) );	
		}

		void inc_offset(std::ptrdiff_t bytes)
		{  m_offset += bytes;   }

		void dec_offset(std::ptrdiff_t bytes)
		{  m_offset -= bytes;   }

		offset_type m_offset; //Distance between this object and pointed address

		static inline pointer unconst_pointer( const_pointer ptr )
		{
			return const_cast<pointer>( ptr );
		}

		inline pointer get_pointer_internal() const
		{
			return unconst_pointer( do_get_pointer() );
		}
	};

	template< class T >
	static inline offset_ptr<T> operator+( typename offset_ptr<T>::difference_type n, const offset_ptr<T>& ptr )
	{
		return offset_ptr<T>( get_pointer_internal(ptr) + n );
	}

	//For compatibility with delete_, new_ operators
	template<class Mgr, class T>
	static inline void delete_( offset_ptr<T>& ptr )
	{
		return ::delete_( get_pointer_internal(ptr), mem_mgr(Mgr::instance() ) );
	}

	template<class Mgr, class T>
	static inline void delete_array( offset_ptr<T>& ptr )
	{
		return ::delete_array( get_pointer_internal(ptr), mem_mgr(Mgr::instance() ) );
	}
}


#endif// MGR_self_offset_ptr_HEADER