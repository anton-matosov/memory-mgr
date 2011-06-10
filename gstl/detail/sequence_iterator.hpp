/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#ifndef GSTL_SEQUENCE_ITERATOR_HEADER
#define GSTL_SEQUENCE_ITERATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <boost/iterator/iterator_adaptor.hpp>
#include <gstl/detail/iterator_declarer.hpp>
#include <boost/pointer_to_other.hpp>
#include <gstl/detail/assert.hpp>
#include <gstl/detail/helpers.hpp>

namespace gstl
{
	namespace detail
	{
		template <class PtrT, class ContainerT,
					class CategoryOrTraversal, class DerivedT, class Value = boost::use_default>
 		class sequence_iterator_base
 			: public boost::iterator_adaptor<
 			DerivedT,				// Derived
 			PtrT,					// Base
 			Value,					// Value
 			CategoryOrTraversal		// CategoryOrTraversal
 			>
 		{
 		protected:
 			struct enabler {};  // a protected type avoids misuse

		public:
 			typedef sequence_iterator_base					self_type;
 			typedef typename self_type::iterator_adaptor_	base_type;
			typedef typename base_type::value_type			value_type;

			typedef PtrT									pointer_type;
			typedef ContainerT								container_type;
			typedef typename boost::pointer_to_other<pointer_type, const container_type>::type container_pointer;
 
 			sequence_iterator_base()
 				: base_type( 0 )
 			{}
 
			explicit sequence_iterator_base( pointer_type p, container_pointer container )
				: base_type( p )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA container_( container ) )
			{
				gstl::helpers::unused_variable( container );
				GSTL_ASSERT( !! this->container_ );
			}
 
			template <class OtherPtrT, class OtherDerivedT>
			sequence_iterator_base( 
				sequence_iterator_base<OtherPtrT, container_type, CategoryOrTraversal, OtherDerivedT, Value> const& other,
				typename boost::enable_if< boost::is_convertible<OtherPtrT, pointer_type>,
				enabler >::type = enabler() )
				: base_type( other.base() )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA container_( other.container_ ) )
			{

			}

		private:
			friend class boost::iterator_core_access;

			bool equal(self_type const& other) const
			{
				GSTL_ASSERT( !! this->container_ );
				GSTL_ASSERT( !! other.container_ );
				GSTL_ASSERT( this->container_ == other.container_ );
				return this->base() == other.base();
			}

		public:
			GSTL_DEBUG_EXPRESSION( container_pointer container_ );
 		};

		template <class PtrT, class ContainerT>
		class sequence_iterator
			:public sequence_iterator_base<
			PtrT,
			ContainerT,
			boost::random_access_traversal_tag,
			sequence_iterator<PtrT, ContainerT>,
			boost::use_default
			>
		{
		public:
			typedef self_type/*from base*/	base_type;
			typedef sequence_iterator		self_type;


			sequence_iterator()
			{}

			explicit sequence_iterator( pointer_type p, container_pointer container )
				: base_type( p, container )
			{

			}

			template <class OtherPtrT>
			sequence_iterator( sequence_iterator<OtherPtrT, container_type> const& other,
				typename boost::enable_if< boost::is_convertible<OtherPtrT, pointer_type>,
				enabler >::type = enabler() )
				:base_type(other)
			{
			}
		private:
			friend class boost::iterator_core_access;
		};

		template <class ContainerT>
		class declare_sequence_iterator
			:public detail::iterator_declarer<ContainerT, sequence_iterator>
		{
		
		};
	}
}

#endif //GSTL_SEQUENCE_ITERATOR_HEADER
