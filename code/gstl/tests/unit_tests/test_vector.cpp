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



#include <gstl/vector>
#include <vector>
#include "operations_tracer.hpp"
#include "test_class.hpp"
#include "managers.hpp"

class vector_fixture
{
public:
	typedef gstl::test::operations_tracer<int>	tracer_type;
	typedef gstl::vector<tracer_type>			traced_container_type;

};

BOOST_FIXTURE_TEST_SUITE( vector_test, vector_fixture )

	typedef test_class test_value_type;
	typedef std::vector<test_value_type>	std_vector;
	typedef gstl::vector<test_value_type>	gstl_vector;
	typedef gstl::vector<test_value_type,
		memory_mgr::allocator<test_value_type, ptr_alloc_mgr> >			memory_mgr_vector;
	typedef gstl::vector<test_value_type,
		memory_mgr::allocator<test_value_type, off_alloc_mgr> >	memory_mgr_off_vector;
	typedef gstl::vector<test_value_type,
		memory_mgr::offset_allocator<test_value_type, off_alloc_mgr> >	memory_mgr_old_off_vector;

	typedef boost::mpl::list< /**std_vector,/**/ gstl_vector/**/, memory_mgr_vector/**/,
		memory_mgr_off_vector/**/, 	memory_mgr_old_off_vector/**/ > t_list;

	#include "detail/test_construction_throw.hpp"
	#include "detail/test_assign_throw.hpp"

	#include "detail/test_construction.hpp"
	#include "detail/test_assign_operator.hpp"
	#include "detail/test_clear.hpp"
	#include "detail/test_empty.hpp"
	#include "detail/test_resize.hpp"
	#include "detail/test_begin_end.hpp"
	#include "detail/test_swap.hpp"
	#include "detail/test_erase_iter.hpp"
	#include "detail/test_erase_range.hpp"
	#include "detail/test_compare_operators.hpp"
	#include "detail/test_push_pop_back.hpp"
	#include "detail/test_back.hpp"
	#include "detail/test_front.hpp"

	#include "detail/test_access_operators.hpp"
	#include "detail/test_reserve.hpp"
	#include "detail/test_resize_reserve_throw.hpp"

	#include "detail/test_iterator.hpp"


	BOOST_AUTO_TEST_CASE( test_objects_validness )
	{
		typedef traced_container_type::value_type value_type;
		value_type::clear();

		value_type arr[] = { 1, 2, 3, 4, 5, 6, 7 };
		value_type arr2[] = { 7, 7, 7, 7 };
		value_type val = arr2[0];
		size_t arr2_len = GSTL_ARRAY_LEN( arr2 );

		tracer_type::clear();
		traced_container_type vec1;
		long creations = 0;
		long destructions = 0;
		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		
		{
			//Iterators constructor
			traced_container_type vec2;
			vec2.reserve( 200 );
			vec2.assign( arr, GSTL_ARRAY_END( arr ) );
			creations += GSTL_ARRAY_LEN( arr );
			
			BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );
			
			vec2.clear();
			destructions += GSTL_ARRAY_LEN( arr );

			BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

			vec2.insert( vec2.end(), arr2_len, val );
			
			creations += arr2_len;
			BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

			traced_container_type vec3 = vec2;
			creations += vec3.size();
			BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

			destructions += vec2.size();
			vec2 = traced_container_type();
			BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

			size_t num_erases = vec3.size() / 2;
			destructions += num_erases;
			while( num_erases-- )
			{
				vec3.erase( vec3.begin() );
			}
			BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

			//Affect of ~vector
			destructions += vec2.size();
			destructions += vec3.size();
		}

		BOOST_CHECK_EQUAL( tracer_type::creations(), creations );
		BOOST_CHECK_EQUAL( tracer_type::destructions(), destructions );

		tracer_type::clear();
	}

BOOST_AUTO_TEST_SUITE_END()


