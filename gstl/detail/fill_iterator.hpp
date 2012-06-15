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

#ifndef GSTL_FILL_ITERATOR_HEADER
#define GSTL_FILL_ITERATOR_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/*
Starts on page 541 C++ std 2003
curr 556

Table 71 - Relations among iterator categories
+-------------------------------------------------------+
|	Random access -> Bidirectional -> Forward -> Input	|
|											-> Output	|
+-------------------------------------------------------+
*/

#include <gstl/detail/iterator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator.hpp>

namespace gstl
{

	namespace detail
	{

		template <class Value>
		class fill_iterator_base
			: public boost::iterator_adaptor<
			fill_iterator_base<Value>				// Derived
			, detail::portable_difference_type		// Base
			, Value									// Value
			, boost::random_access_traversal_tag	// CategoryOrTraversal
			>
		{
		private:
			struct enabler {};  // a private type avoids misuse

		public:
			typedef fill_iterator_base						self_type;
			typedef typename self_type::iterator_adaptor_	base_type;
			typedef Value									value_type;
			typedef value_type&								reference;
			typedef detail::portable_difference_type		counter_type;

			static const counter_type npos = static_cast<counter_type>( -1 );

			fill_iterator_base()
				: base_type( npos ),
				value_( 0 )
			{}

			explicit fill_iterator_base( value_type* p, counter_type count )
				: base_type( count ),
				value_( p )
			{}

			template <class OtherValue>
			fill_iterator_base( fill_iterator_base<OtherValue> const& other,
				typename boost::enable_if< boost::is_convertible<OtherValue*,Value*>,
				enabler >::type = enabler() )
				: base_type( other.base() ),
				value_( other.value_ )
			{

			}

		private:
			friend class boost::iterator_core_access;

			reference dereference() const
			{ 
				return *value_; 
			}

			value_type* value_;
		};
	}

	template<class Value>
	class fill_iterator
		:public detail::fill_iterator_base<Value>
	{
		typedef detail::fill_iterator_base<Value>	base_type;
	public:
		typedef fill_iterator	self_type;
		typedef typename boost::add_const<typename  self_type::reference>::type const_reference;
		fill_iterator( const_reference val, typename self_type::counter_type count = 0 )
			:base_type( &this->value_, count ),
			value_( val )
		{}

	private:
		typename self_type::value_type value_;
	};

	template<class Value>
	class fill_iterator_ref
		:public detail::fill_iterator_base<Value>
	{
		typedef detail::fill_iterator_base<Value>	base_type;
	public:
		typedef fill_iterator_ref	self_type;
		fill_iterator_ref( typename self_type::reference val, typename self_type::counter_type count = 0 )
			:base_type( &val, count )
		{}
	};
}

#endif //GSTL_FILL_ITERATOR_HEADER