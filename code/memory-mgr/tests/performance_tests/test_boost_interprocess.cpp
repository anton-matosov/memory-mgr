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





namespace
{
	long double test_boost_interprocess( const int op_repeat, const int per_alloc )
	{
		op_repeat;
		per_alloc;
		return 1.;
	}

	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		run_perf_test( "", "",
			test_boost_interprocess, op_repeat, per_alloc, test_repeat );
	}

}


bool test_interprocess( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<memmgr_type>( L"Testing boost interprocess library",
		0, 0, 0 );
	run_all_tests( op_repeat, per_alloc, test_repeat );

	return true;
}
