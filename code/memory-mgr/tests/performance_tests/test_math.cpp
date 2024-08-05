/* 
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

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


#include "memory-mgr/detail/math.h"
#include <boost/type_traits/make_unsigned.hpp>

namespace
{
	struct stub
	{
		static bool  do_work_;
		static void work( int ii )
		{
			if( do_work_ )
			{
				std::vector<char> s( ii && 0 + 1 );
			}

		}
	};
	bool stub::do_work_ = false;

	template<class TestType>
	long double test_lowest_bit( const int op_repeat, const int per_alloc )
	{
		typedef	TestType test_type;


		

		enum
		{
			numBytes = sizeof( test_type ),
			bitsInByte = 8,
			numBits = numBytes * bitsInByte
		};
		for( int i = 0; i < numBits; ++i )
		{
			test_type var = test_type(1) << i;
			int ii = 0;
		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			ii = memory_mgr::math::lowest_bit2<boost::make_unsigned<test_type>::type >( var );	
			stub::work( ii );
		}
		TEST_END_LOOP( std::wcout );
			//This is required because otherwise the call will be removed by the optimizer
			
		}
		return 0;//TEST_ELAPCED_MCS;

	//memory_mgr::math::lowest_bit2( var );

	}

	const char* lowest_bit_category = "lowest_bit";

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		run_perf_test( lowest_bit_category, "lowest_bit (log2)",
			test_lowest_bit<unsigned long>, op_repeat, per_alloc, test_repeat );
	}

}


bool test_math( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<memmgr_type>( L"Testing math library",
		0, 0, 0 );
	run_all_tests( op_repeat, per_alloc, test_repeat );

	return true;
}
