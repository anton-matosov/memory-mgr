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

#ifndef GSTL_LIST_ITERATOR_HEADER
#define GSTL_LIST_ITERATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/detail/sequence_iterator.hpp>
#include <gstl/detail/iterator_declarer.hpp>

namespace gstl
{
	namespace detail
	{
		template <class NodePtrT, class ContainerT>
		class list_iterator
			: public checked_iterator<
				NodePtrT, ContainerT, boost::bidirectional_traversal_tag,
				list_iterator<NodePtrT, ContainerT>,
				typename ContainerT::value_type
			>
// 			: public boost::iterator_adaptor<
// 			list_iterator<NodePtrT, ContainerT>,	// Derived
// 			NodePtrT,								// Base
// 			typename ContainerT::value_type,		// Value
// 			boost::bidirectional_traversal_tag		// CategoryOrTraversal
// 			>
		{
		private:
			struct enabler {};  // a private type avoids misuse

		public:
			typedef self_type/*from base*/	base_type;
			typedef list_iterator			self_type;
			typedef NodePtrT								pointer_type;
			typedef typename base_type::value_type			value_type;

			list_iterator()
			{}

			explicit list_iterator( pointer_type p, const container_type* container, bool checksEnabled )
				: base_type( p, container, checksEnabled )
			{
			}

			template <class OtherPtrT>
			list_iterator( list_iterator<OtherPtrT, container_type> const& other,
				typename boost::enable_if< boost::is_convertible<OtherPtrT, pointer_type>,
				enabler >::type = enabler() )
				: base_type( other )
			{
			}
		private:
			friend class boost::iterator_core_access;
			void increment()
			{
				GSTL_CHECKED_ITER_ASSERT( !! this->container_ );
				GSTL_CHECKED_ITER_ASSERT( this->base() != this->container_->tail_ );

				this->base_reference() = (*this->base()).next_;
			}

			void decrement()
			{
				GSTL_CHECKED_ITER_ASSERT( !! this->container_ );
				GSTL_CHECKED_ITER_ASSERT( this->base() != this->container_->tail_->next_ );

				this->base_reference() = (*this->base()).prev_;
			}

			typename base_type::reference dereference() const
			{ 
				return (*this->base()).value_; 
			}

		};

		template <class ContainerT>
		class declare_list_iterator
			:public detail::iterator_declarer< ContainerT, list_iterator,
			typename ContainerT::node_pointer,
			typename ContainerT::node_const_pointer >
		{

		};
	}
}

#endif //GSTL_LIST_ITERATOR_HEADER