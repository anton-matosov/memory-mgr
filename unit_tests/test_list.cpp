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

#include "stdafx.h"

#include <gstl/list>
#include <list>
#include <boost/foreach.hpp>
#include "operations_tracer.hpp"
#include "managers.hpp"

class vector_fixture
{
public:
	typedef gstl::test::operations_tracer<int>	tracer_type;
	//typedef gstl::list<tracer_type>			traced_vector_type;

};
// 
// namespace gstl
// {
// 	template <class T, class Alloc, class PtrTraits, class StreamT>
// 	StreamT& operator<<( StreamT& stream, const list<T, Alloc, PtrTraits>& vec )
// 	{
// 		BOOST_FOREACH( const T& val, vec )
// 		{
// 			stream << val << ' ';
// 		}
// 		return stream;
// 	}
// }
// 
// namespace std
// {
// 	template <class T, class Alloc, class StreamT>
// 	StreamT& operator<<( StreamT& stream, const list<T, Alloc>& vec )
// 	{
// 		BOOST_FOREACH( const T& val, vec )
// 		{
// 			stream << val << ' ';
// 		}
// 		return stream;
// 	}
// }

BOOST_FIXTURE_TEST_SUITE( vector_test, vector_fixture )



typedef int								test_value_type;
typedef std::list<test_value_type>		std_list;
// typedef gstl::list<test_value_type>		gstl_listr;
// typedef gstl::list<test_value_type,
// memory_mgr::allocator<test_value_type, ptr_alloc_mgr>, gstl::pointer_traits<test_value_type> >			memory_mgr_list;
// typedef gstl::list<test_value_type,
// memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_list;

typedef boost::mpl::list< /**/std_list/**, gstl_listr/**, memory_mgr_list/**, memory_mgr_off_list/**/> t_list;


#include "detail/test_construction.hpp"


BOOST_AUTO_TEST_SUITE_END()


