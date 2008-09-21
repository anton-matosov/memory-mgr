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
// 	class mgr_allocator
// 	{
// 	public:
// 		inline void deallocate( void* ptr, size_type size )
// 		{
// 			m_mgr->deallocate( ptr, size );
// 		}
// 
// 		// allocate array of count elements
// 		inline void* allocate(size_type size)
// 		{	
// 			return m_mgr->allocate( size );
// 		}
// 	};

	template<class MemMgr, class Allocator>
	void test_data_validness_impl()
	{
		enum{ minimum_memory_size = 64 * 1024 };
		typedef MemMgr										mgr_type;
		typedef memory_mgr::manager_traits<mgr_type>		traits_type;
		typedef traits_type::offset_type					offset_type;
		typedef traits_type::chunk_type						chunk_type;
		typedef Allocator::rebind<int>::other				allocator;

		STATIC_ASSERT( traits_type::memory_size >= minimum_memory_size, memory_size_is_too_low );

		std::vector<chunk_type> memory( traits_type::memory_size );
		mgr_type mgr( &*memory.begin() );
		allocator alloc( &mgr );
		
		typedef std::map<int*, int> ptrval_map_type;
		ptrval_map_type real_vals;

		for( int i = 0; i < 1000; ++i )
		{
			//int* p = memory_mgr::detail::to_pointer<int>( mgr.allocate( sizeof(int) ), mgr );
			int *p = alloc.allocate(1);
			BOOST_CHECK( p != 0 );
			int val = rand();
			*p = val;
			BOOST_CHECK( real_vals.find( p ) == real_vals.end() );
			real_vals[p] = val;
		}

		for( ptrval_map_type::const_iterator it = real_vals.begin(); it != real_vals.end(); ++it )
		{
			BOOST_CHECK_EQUAL( *it->first, it->second );
			//mgr.deallocate( memory_mgr::detail::to_offset( it->first, mgr ), sizeof(int) );
			alloc.deallocate( it->first, 1 );
		}
	}

	template<class MemMgr>
	void test_data_validness()
	{
		test_data_validness_impl<MemMgr, memory_mgr::member_allocator<int, MemMgr> >();
	}

	template<class MemMgr>
	void test_data_validness_singleton()
	{
		test_data_validness_impl<MemMgr, memory_mgr::allocator<int, MemMgr> >();
	}

	//Test pointers on validness
	template<class T>
	void check_pointers( T p1, T p2, T p3, T p4, T p5 )
	{
		const T inv_off = memory_mgr::offset_traits<T>::invalid_offset;

		BOOST_CHECK_NE( p1, inv_off );
		BOOST_CHECK_NE( p2, inv_off );
		BOOST_CHECK_NE( p3, inv_off );
		BOOST_CHECK_NE( p4, inv_off );
		BOOST_CHECK_NE( p5, inv_off );

		BOOST_CHECK_NE( p1, p2 );
		BOOST_CHECK_NE( p2, p3 );
		BOOST_CHECK_NE( p3, p4 );
		BOOST_CHECK_NE( p4, p5 );
	}

	template<class MemMgr>
	void test_null_pointer_dealloc_impl( MemMgr& mgr )
	{
		typedef MemMgr memmgr_type;
		typedef memory_mgr::manager_traits<memmgr_type>		traits_type;
		typedef traits_type::block_id_type					block_id_type;

		block_id_type null_ptr = memory_mgr::offset_traits<block_id_type>::invalid_offset;

		BOOST_CHECKPOINT( "before deallocation of null ptr" );
		mgr.deallocate( null_ptr, 0 );
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
		typedef MemMgr memmgr_type;
		typedef memory_mgr::manager_traits<memmgr_type>		traits_type;
		typedef traits_type::chunk_type						chunk_type;

		std::vector<chunk_type> memory( traits_type::memory_size );
		memmgr_type mgr( &*memory.begin() );
		test_null_pointer_dealloc_impl( mgr );
	}
};


#endif //MGR_COMMON_MANAGER_TESTS_HEADER


