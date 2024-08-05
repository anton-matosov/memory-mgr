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


#pragma once

#include <gstl/detail/assert.hpp>
#include <gstl/detail/helpers.hpp>
#include <gstl/detail/iterator_declarer.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/pointer_to_other.hpp>
#include <boost/utility/enable_if.hpp>


namespace gstl
{
	namespace detail
	{
#define GSTL_ASSERT_IF( ifExpr, expr ) GSTL_DEBUG_EXPRESSION( if( ifExpr ) { GSTL_ASSERT(expr); } )
#define GSTL_CHECKED_ITER_ASSERT( expr ) GSTL_ASSERT_IF( checksEnabled_, expr )
		
		template <class PtrT, class ContainerT,
		class CategoryOrTraversal, class DerivedT, class Value = boost::use_default>
		class checked_iterator
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
			typedef checked_iterator					self_type;
			typedef typename self_type::iterator_adaptor_	base_type;
			typedef typename base_type::value_type			value_type;

			typedef PtrT									pointer_type;
			typedef ContainerT								container_type;
			typedef typename boost::pointer_to_other<pointer_type, const container_type>::type container_pointer;

			checked_iterator()
				: base_type( 0 )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA checksEnabled_( true ) )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA container_() )
			{}

			explicit checked_iterator( pointer_type p, const container_type* container, bool checksEnabled )
				: base_type( p )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA checksEnabled_( checksEnabled ) )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA container_( checksEnabled ? container : NULL ) )
			{
				gstl::helpers::unused_variable( checksEnabled );
				gstl::helpers::unused_variable( container );
				do_check_container();
			}

			void do_check_container() const
			{
				GSTL_CHECKED_ITER_ASSERT( !! this->container_ );
			}

			template <class OtherPtrT, class OtherDerivedT>
			checked_iterator( 
				checked_iterator<OtherPtrT, container_type, CategoryOrTraversal, OtherDerivedT, Value> const& other,
				typename boost::enable_if< boost::is_convertible<OtherPtrT, pointer_type>,
				enabler >::type = enabler() )
				: base_type( other.base() )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA checksEnabled_( other.checksEnabled_ ) )
				GSTL_DEBUG_EXPRESSION( GSTL_COMA container_( other.checksEnabled_ ? other.container_ : NULL  ) )
			{

			}

		private:
			friend class boost::iterator_core_access;

			bool equal(self_type const& other) const
			{
				do_check_container();
				GSTL_CHECKED_ITER_ASSERT( !! other.container_ );
				GSTL_CHECKED_ITER_ASSERT( this->container_ == other.container_ );
				return this->base() == other.base();
			}

		public:
			GSTL_DEBUG_EXPRESSION( bool checksEnabled_ );
			GSTL_DEBUG_EXPRESSION( container_pointer container_ );
		};
	}
}


