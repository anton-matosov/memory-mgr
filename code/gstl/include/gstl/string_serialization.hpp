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

#include <gstl/string>

#include <boost/serialization/string.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/tracking.hpp>

namespace boost
{
	namespace serialization
	{ 
		template<class Archive, class CharT, class Traits, class Alloc>
		inline void save(
			Archive& ar,
			const gstl::basic_string<CharT, Alloc, Traits>& str,
			const unsigned int version
			)
		{
			typedef std::basic_string<CharT> StdStringType;
			typedef typename StdStringType::size_type size_type;
			StdStringType chars( str.c_str(), static_cast<size_type>( str.length() ) );
			ar & /*BOOST_SERIALIZATION_NVP*/( chars );
		}

		template<class Archive, class CharT, class Traits, class Alloc>
		inline void load(
			Archive& ar,
			gstl::basic_string<CharT, Alloc, Traits>& str,
			const unsigned int version
			)
		{
			typedef std::basic_string<CharT> StdStringType;
			StdStringType chars;
			ar & /*BOOST_SERIALIZATION_NVP*/( chars );
			str.assign( chars.c_str(), chars.length() );
		}

		// split non-intrusive serialization function member into separate
		// non intrusive save/load member functions
		template<class Archive, class CharT, class Traits, class Alloc>
		inline void serialize(
			Archive & ar,
			gstl::basic_string<CharT, Alloc, Traits> & str,
			const unsigned int version
			)
		{
			boost::serialization::split_free(ar, str, version);
		}

		template <class CharT, class Traits, class Alloc>                                     
		struct implementation_level_impl< const gstl::basic_string<CharT, Alloc, Traits> >  
		{
			typedef mpl::integral_c_tag tag;
			typedef mpl::int_< boost::serialization::object_serializable > type;
			BOOST_STATIC_CONSTANT(
				int,
				value = implementation_level_impl::type::value
				);
		};

		template <class CharT, class Traits, class Alloc> 
		struct tracking_level< const gstl::basic_string<CharT, Alloc, Traits> >  
		{
			typedef mpl::integral_c_tag tag;
			typedef mpl::int_<boost::serialization::track_never> type;
			BOOST_STATIC_CONSTANT(
				int,
				value = tracking_level::type::value
				);
		};

	} // serialization
} // namespace boost

