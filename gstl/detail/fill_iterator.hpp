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

#include <iterator>
#include <xutility>
#include <gstl/detail/types.hpp>
#include <gstl/detail/iterator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace gstl
{

	namespace detail
	{

		template <class Value>
		class fill_iterator_base
			: public boost::iterator_facade<
			fill_iterator_base<Value>				// Derived
			, Value									// Value
			, boost::random_access_traversal_tag	// CategoryOrTraversal
			>
		{
		private:
			struct enabler {};  // a private type avoids misuse

		public:
			typedef fill_iterator_base						self_type;
			typedef typename self_type::iterator_facade_	base_type;
			typedef Value									value_type;
			typedef value_type&								reference;
			typedef ::gstl::detail::portable_difference_type difference_type;
			typedef ::gstl::detail::portable_difference_type counter_type;

			static const counter_type npos = static_cast<counter_type>( -1 );

			fill_iterator_base()
				: value_( 0 ),
				count_( 0 )
			{}

			explicit fill_iterator_base( value_type* valuePtr, counter_type count )
				: value_( valuePtr ),
				count_( count )
			{}

			template <class OtherValue>
			fill_iterator_base( fill_iterator_base<OtherValue> const& other,
				typename boost::enable_if< boost::is_convertible<OtherValue*,Value*>,
				enabler >::type = enabler() )
				: value_( other.value() ),
				count_( other.count() )
			{

			}

			value_type* value() const { return value_; }
			counter_type count() const { return count_; }
		private:
			friend class boost::iterator_core_access;

			reference dereference() const
			{ 
				return *value_; 
			}

			void advance(difference_type n)
			{
				count_ += n;
			}

			void increment()
			{
				++count_;
			}

			void decrement() 
			{
				--count_;
			}

			template<class OtherValue> 
			difference_type distance_to( fill_iterator_base<OtherValue> const& y ) const
			{
				return y.count() - count_;
			}

			template<class OtherValue> 
			bool equal( fill_iterator_base<OtherValue> const& y ) const
			{
				return y.count() == count_
					&& y.value() == value_;
			}

			value_type* value_;
			counter_type count_;
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