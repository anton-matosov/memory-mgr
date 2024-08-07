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

#include "memory-mgr/detail/offset_traits.h"
#include "memory-mgr/detail/compatibility_types.h"
#include "memory-mgr/detail/cmp_helper.h"
#include "memory-mgr/pointer_cast.h"
#include "memory-mgr/new.h"
#include "memory-mgr/detail/type_manip.h"
#include "memory-mgr/smart_ptr/smart_ptr/detail/cast_tags.hpp"

namespace memory_mgr
{	
	namespace detail
	{
		template< class T, class OffsetT, class DerivedT >
		class offset_ptr_base
			: public detail::cmp_helper< offset_ptr_base< T, OffsetT, DerivedT > >
		{		
		public:
			typedef OffsetT					offset_type;

			typedef offset_ptr_base			_offset_ptr_base;
			typedef offset_ptr_base			self_type;
			typedef self_type&				self_ref_type;
			typedef const self_type			const_self_type;
			typedef const_self_type&		const_self_ref_type;

			typedef DerivedT derived_type;
			typedef DerivedT& derived_ref_type;
			typedef DerivedT* derived_ptr_type;

			//These typedefs are required if this class is used in the containers as iterator
			typedef T					value_type;
			typedef value_type*			pointer;
			typedef const value_type*	const_pointer;

			typedef typename type_manip::add_reference<value_type>::type reference;
			typedef typename type_manip::add_reference<const value_type>::type const_reference;

// #ifdef _WIN32
// 			typedef std::ptrdiff_t difference_type;
// #else
// 			typedef memory_mgr::detail::difference_type difference_type;
// #endif
			typedef ::memory_mgr::detail::portable_difference_type difference_type;

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
			inline offset_ptr_base()
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{}

			//Copy constructor
			inline offset_ptr_base( const offset_ptr_base& ptr )
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer( ptr.get() );
			}

			//Pointer constructor
			inline offset_ptr_base( const_pointer p )
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer(p);	
			}

			//Polymorph copy constructor
			template < typename U, typename OffsetU, typename DerivedU >
			inline offset_ptr_base( const offset_ptr_base< U, OffsetU, DerivedU >& ptr )
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer( ptr.get() );
			}

			template<class Y, typename DerivedY>
			offset_ptr_base(offset_ptr_base<Y, offset_type, DerivedY> const & r,
				memory_mgr::detail::static_cast_tag)
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer( static_cast<pointer>(r.get()) );
			}

			template<class Y, typename DerivedY>
			offset_ptr_base(offset_ptr_base<Y, offset_type, DerivedY> const & r,
				memory_mgr::detail::dynamic_cast_tag)
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer( dynamic_cast<pointer>(r.get()) );
			}

			template<class Y, typename DerivedY>
			offset_ptr_base(offset_ptr_base<Y, offset_type, DerivedY> const & r,
				memory_mgr::detail::const_cast_tag)
				:m_offset( offset_traits<offset_type>::invalid_offset )
			{
				derived_this()->do_set_pointer( const_cast<pointer>(r.get()) );
			}


			//////////////////////////////////////////////////////////////////////////
			//Polymorph copy operators
			inline derived_ref_type operator=( const_pointer p )			
			{
				derived_this()->do_set_pointer(p);				
				return *derived_this();
			}

			inline derived_ref_type operator=( const offset_ptr_base& ptr )			
			{
				derived_this()->do_set_pointer( ptr.get() );
				return *derived_this();
			}

			//Polymorph copy operator
			template < typename U, typename OffsetU, typename DerivedU >
			inline derived_ref_type operator=( const offset_ptr_base< U, OffsetU, DerivedU >& ptr )			
			{
				derived_this()->do_set_pointer( ptr.get() );
				return *derived_this();
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

			typedef offset_type self_type::*unspecified_bool_type;

			operator unspecified_bool_type() const // never throws
			{
				return is_null() ? 0: &self_type::m_offset;
			}

			bool operator!() const
			{ 
				return  is_null();
			}

			inline derived_ref_type operator++()
			{
				return static_cast<derived_type&>(*this += 1);
			}

			inline derived_type operator++(int)
			{
				derived_type temp(*derived_this());
				++*this;
				return temp; ;
			}

			inline derived_ref_type operator--()
			{
				return static_cast<derived_type&>(*this -= 1);
			}

 			inline derived_type operator--(int)
 			{
 				self_type temp(*this);
 				--*this;
 				return temp;
 			}
			
			inline derived_ref_type operator+=(difference_type n)
			{
				m_offset = static_cast<offset_type>(m_offset + shift_size(n));
				return *derived_this();
			}

			inline derived_ref_type operator-=(difference_type n)
			{
				m_offset -= shift_size(n);
				return *derived_this();
			}

			inline derived_type operator+(difference_type n) const
			{
				derived_type temp( *derived_this() );
				return temp += n;
			}

			inline derived_type operator-(difference_type n) const
			{
				derived_type temp( *derived_this() );
				return temp -= n;
			}
					
			inline bool operator==( const self_type& rhs ) const
			{
				return this->get() == rhs.get();
			}
			
			inline bool operator<( const self_type& rhs ) const
			{
				return this->get() < rhs.get();
			}
			
			inline bool operator>( const self_type& rhs ) const
			{
				return this->get() > rhs.get();
			}

			pointer get() const
			{  
				return unconst_pointer( derived_this()->do_get_pointer() );
			}

			void swap(self_type& b) // never throws
			{
				std::swap(this->m_offset, b.m_offset);
			}
		private:
			DerivedT* derived_this()
			{
				return static_cast<DerivedT*>( this );
			}

			const DerivedT* derived_this() const
			{
				return static_cast<const DerivedT*>( this );
			}

			static difference_type shift_size( difference_type n )
			{
				return n * sizeof( value_type );
			}

			static inline pointer unconst_pointer( const_pointer ptr )
			{
				return const_cast<pointer>( ptr );
			}

		protected:
			offset_type m_offset; //Distance between this object and pointed address
		};

		template< class T, class OffsetT, class DerivedT >
		inline DerivedT operator+( typename offset_ptr_base<T, OffsetT, DerivedT>::difference_type n, const offset_ptr_base<T, OffsetT, DerivedT>& ptr )
		{
			return DerivedT( get_pointer_internal(ptr) + n );
		}

		template< class T, class U, class OffsetT, class DerivedT >
		inline typename offset_ptr_base<T, OffsetT, DerivedT>::difference_type
			operator-( offset_ptr_base<T, OffsetT, DerivedT> lhs, const offset_ptr_base<U, OffsetT, DerivedT>& rhs )
		{
			return lhs.get() - rhs.get();
		}
		
		template<class T, class OffsetT, class DerivedT>
		inline void swap(offset_ptr_base<T, OffsetT, DerivedT>& a, offset_ptr_base<T, OffsetT, DerivedT>& b)
		{
			a.swap(b);
		}
	} //namespace detail

	template<class T, class OffsetT, class DerivedT>
	T* get_pointer( const detail::offset_ptr_base<T, OffsetT, DerivedT>& p )
	{
		return p.get();
	}
}
