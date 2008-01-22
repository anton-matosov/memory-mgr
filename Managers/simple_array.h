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

#ifndef MGR_SIMPLE_ARRAY_HEADER
#define MGR_SIMPLE_ARRAY_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include "detail/type_manip.h"
#include "detail/static_assert.h"
#include "detail/cmp_helper.h"

namespace memory_mgr
{
	//A simple wrapper class
	//It has C style array semantic
	template< typename T, typename MemMgr >  
	class simple_array
	{
	public:
		typedef MemMgr				mgr_type;

		typedef T					value_type;
		typedef value_type*			pointer_type;
		typedef const value_type*	const_pointer_type;

		typedef value_type&			reference_type;
		typedef const value_type&	const_reference_type;

		typedef simple_ptr< value_type, mgr_type >	self_type;
		typedef self_type&							self_ref_type;

		//Default constructor
		inline simple_array();

		//Copy constructor
		inline simple_array( const self_ref_type );

		//Copy operator, just copy stored pointer
		inline simple_array& operator=( const self_ref_type );

		//Access operators
		inline pointer_type operator&();
		inline const_pointer_type operator&() const;

		inline reference_type operator[]( const size_t index );
		inline const_reference_type operator[]( const size_t index ) const;

		self_ref_type operator++();
		const self_ref_type operator++() const;
		 
		self_ref_type operator--();
		const self_ref_type operator--() const;


		pointer_t operator->();
		const_pointer_t operator->() const;
		
		reference_t operator*();
		const_reference_t operator*() const;

		bool is_null() const { return m_ptr == pointer_traits<ptr_type>::null_ptr; }
		bool is_not_null() const { return !is_null(); }
		bool operator!() const { return  is_null(); }
	
	private:
		typedef typename mgr_type::ptr_type ptr_type;
		typedef typename ptr_type::offset_type offset_type;
	};
}

#endif// MGR_SIMPLE_ARRAY_HEADER