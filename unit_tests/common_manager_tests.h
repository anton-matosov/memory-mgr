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

#ifndef MGR_COMMON_MANAGER_TESTS_HEADER
#define MGR_COMMON_MANAGER_TESTS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/static_assert.h>
#include <memory-mgr/detail/ptr_helpers.h>
#include <memory-mgr/allocator.h>

namespace test
{
	template<class MemMgr, class ValueType, class Allocator>
	void test_data_validness_impl()
	{
		typedef MemMgr										mgr_type;
		typedef ValueType									value_type;
		typedef memory_mgr::manager_traits<mgr_type>		traits_type;
		typedef typename traits_type::chunk_type			chunk_type;
		typedef typename Allocator::template rebind<value_type>::other		allocator;

		STATIC_ASSERT( traits_type::memory_size >= 64 * 1024/*minimum_memory_size*/, memory_size_is_too_small );

		std::vector<chunk_type> memory( traits_type::memory_size );
		mgr_type mgr( &*memory.begin() );
		allocator alloc( &mgr );
		
		typedef std::map<value_type*, value_type> ptrval_map_type;
		ptrval_map_type real_vals;

		for( int i = 0; i < 1000; ++i )
		{
			value_type *p = alloc.allocate(1);
			BOOST_CHECK( p != 0 );
			value_type val = static_cast<value_type>( rand() );
			*p = val;
			BOOST_CHECK( real_vals.find( p ) == real_vals.end() );
			real_vals[p] = val;
		}

		for( typename ptrval_map_type::const_iterator it = real_vals.begin(); it != real_vals.end(); ++it )
		{
			BOOST_CHECK_EQUAL( *it->first, it->second );
			//mgr.deallocate( memory_mgr::detail::to_offset( it->first, mgr ), sizeof(int) );
			alloc.deallocate( it->first, 1 );
		}
	}

	template<class MemMgr, class ValueType>
	void test_data_validness_type()
	{
		test_data_validness_impl<MemMgr, ValueType, memory_mgr::member_allocator<int, MemMgr> >();
	}

	template<class MemMgr>
	void test_data_validness()
	{
		test_data_validness_impl<MemMgr, char, memory_mgr::member_allocator<char, MemMgr> >();
		test_data_validness_impl<MemMgr, int, memory_mgr::member_allocator<int, MemMgr> >();
	}

	template<class MemMgr>
	void test_data_validness_singleton()
	{
		test_data_validness_impl<MemMgr, int, memory_mgr::allocator<int, MemMgr> >();
	}


	template<class T>
	bool is_valid_ptr( T p )
	{
		const T inv_off = memory_mgr::offset_traits<T>::invalid_offset;
		BOOST_CHECK_NE( p, inv_off );
		return p != inv_off;
	}

	//Test pointers on validness
	template<class T1, class T2, class T3, class T4, class T5>
	void check_pointers( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, int object_size = 0 )
	{
		is_valid_ptr( p1 );
		is_valid_ptr( p2 );
		is_valid_ptr( p3 );
		is_valid_ptr( p4 );
		is_valid_ptr( p5 );

		if( object_size )
		{
			BOOST_CHECK_GE( memory_mgr::detail::diff( p2, p1 ), object_size );
			BOOST_CHECK_GE( memory_mgr::detail::diff( p3, p2 ), object_size );
			BOOST_CHECK_GE( memory_mgr::detail::diff( p4, p3 ), object_size );
			BOOST_CHECK_GE( memory_mgr::detail::diff( p5, p4 ), object_size );
		}

		BOOST_CHECK_NE( p1, p2 );
		BOOST_CHECK_NE( p2, p3 );
		BOOST_CHECK_NE( p3, p4 );
		BOOST_CHECK_NE( p4, p5 );
	}

	template<class MemMgr>
	void test_null_pointer_dealloc_impl( MemMgr& mgr )
	{
		typedef MemMgr										memmgr_type;
		typedef memory_mgr::manager_traits<memmgr_type>		traits_type;

		void* p = NULL;
		BOOST_CHECKPOINT( "before deallocation of null ptr" );
		mgr.deallocate( p, 0 );
		BOOST_CHECKPOINT( "after deallocation of null ptr" );
	}

	template<class MemMgr>
	void test_null_pointer_dealloc_seg()
	{
		MemMgr mgr;
		test_null_pointer_dealloc_impl( mgr );
	}


	template<class MemMgr>
	void test_null_pointer_dealloc()
	{
		typedef MemMgr										memmgr_type;
		typedef memory_mgr::manager_traits<memmgr_type>		traits_type;
		typedef typename traits_type::chunk_type			chunk_type;

		std::vector<chunk_type> memory( traits_type::memory_size );
		memmgr_type mgr( &*memory.begin() );
		test_null_pointer_dealloc_impl( mgr );
	}
};


#endif //MGR_COMMON_MANAGER_TESTS_HEADER


