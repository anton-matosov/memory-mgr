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

#include <gstl/detail/checked_iterator.hpp>

namespace gstl
{
	namespace detail
	{
		template <class PtrT, class ContainerT>
		class sequence_iterator
			:public checked_iterator<
			PtrT,
			ContainerT,
			boost::random_access_traversal_tag,
			sequence_iterator<PtrT, ContainerT>,
			boost::use_default
			>
		{
		public:
			using base_type = checked_iterator<PtrT, ContainerT, boost::random_access_traversal_tag, sequence_iterator<PtrT, ContainerT>,	boost::use_default>;
			typedef sequence_iterator		self_type;

			using typename base_type::value_type;
			using typename base_type::pointer_type;
			using typename base_type::container_type;

		protected:
			using typename base_type::enabler;

		public:

			sequence_iterator()
			{}

			explicit sequence_iterator( pointer_type p, const container_type* container, bool checksEnabled )
				: base_type( p, container, checksEnabled )
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

