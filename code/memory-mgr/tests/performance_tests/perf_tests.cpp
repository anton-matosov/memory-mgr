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



bool test_memory_manager( const int op_repeat, const int per_alloc, const int test_repeat );
bool test_managed_base( const int op_repeat, const int per_alloc, const int test_repeat );
bool test_offset_pointer( const int op_repeat, const int per_alloc, const int test_repeat );
bool test_math( const int op_repeat, const int per_alloc, const int test_repeat );
bool test_multithreaded_alloc_dealloc( const int op_repeat, const int per_alloc, const int test_repeat );
bool test_pool( const int op_repeat, const int per_alloc, const int test_repeat );

int main(int /*argc*/, char* /*argv*/[])
{
	const int op_repeat = 5;
	const int per_alloc = 10000;
	const int test_repeat = 10;

	/**
	   @todo Get rid of this stub!!!
	*/
	alloc_mgr::instance();

	//test_math( op_repeat, per_alloc, test_repeat );
	//test_multithreaded_alloc_dealloc( op_repeat, per_alloc, test_repeat );
	test_memory_manager( op_repeat, per_alloc, test_repeat );
	//test_managed_base( op_repeat, per_alloc, test_repeat );
	test_pool( op_repeat, per_alloc, test_repeat );
	test_offset_pointer( op_repeat, per_alloc, test_repeat );

	MGR_PRINT_RESULTS;
	return std::cin.get();
}

