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


#include "memory-mgr/segment_manager.h"
#include "memory-mgr/memory_manager.h"
#include "memory-mgr/pointer_convert.h"
#include "memory-mgr/size_tracking.h"
#include "memory-mgr/heap_segment.h"

BOOST_AUTO_TEST_SUITE( test_segment_manager )

	typedef unsigned int chunk_type;
	const size_t chunk_size = 4;
	const size_t memory_size = 1024;

	typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size > memmgr_type;

	typedef memory_mgr::heap_segment<memmgr_type> heapmgr_type;
	typedef memory_mgr::segment_manager<heapmgr_type, 5> segmgr_type;

	typedef memory_mgr::pointer_convert<segmgr_type> pconv_segmgr_type;
	typedef memory_mgr::size_tracking< pconv_segmgr_type > sz_pconv_segmgr_type;


	typedef boost::mpl::list< segmgr_type, pconv_segmgr_type, sz_pconv_segmgr_type > managers_list;

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_clear, mgr_type, managers_list )
	{
		mgr_type mgr;

		BOOST_CHECKPOINT( "Allocating one object" );
		mgr.allocate( chunk_size, std::nothrow_t() );
		
		BOOST_CHECKPOINT( "Clear memory" );
		mgr.clear();

		BOOST_CHECK( mgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE_TEMPLATE( test_alloc_dealloc, mgr_type, managers_list )
	{
		typedef typename memory_mgr::manager_traits<mgr_type>		traits_type;
		typedef typename traits_type::block_id_type					block_id_type;
		enum
		{
			allocable_chunks = traits_type::allocable_chunks 
			/ (traits_type::memory_overhead ? traits_type::memory_overhead : 1 )
		};

		typedef std::vector< block_id_type >			ptrs_vec;

		mgr_type segmgr;
		ptrs_vec ptrs;

		block_id_type p = 0;
		
		ptrs.reserve( allocable_chunks );
		BOOST_CHECKPOINT( "Allocating maximum allowed number of the objects" );
		for( size_t i = 0; i < allocable_chunks; ++i )
		{
			p = segmgr.allocate( chunk_size, std::nothrow_t() ) ;

			ptrs.push_back( p );
			if( !test::is_valid_ptr( p ) )
			{
				BOOST_ERROR( "Allocation failed" );
				break;
			}
		}

		std::random_shuffle( ptrs.begin(), ptrs.end() );

		BOOST_CHECKPOINT( "Dellocating all objects" );
		for ( typename ptrs_vec::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it )
		{
			segmgr.deallocate( *it, chunk_size );
		}

		BOOST_CHECK( segmgr.is_free() );
	}

	BOOST_AUTO_TEST_CASE( test_offset_convertions )
	{
		typedef segmgr_type								mgr_type;
		typedef memory_mgr::manager_traits<mgr_type>	traits_type;
		typedef traits_type::offset_type				offset_type;
		typedef traits_type::chunk_type					chunk_type;
		typedef traits_type::block_id_type				block_id_type;
		enum
		{
			allocable_chunks = memory_mgr::manager_traits<mgr_type>::allocable_chunks
		};

		typedef std::vector< block_id_type > ptrs_vec;

		mgr_type segmgr;

		block_id_type p = 0;
		ptrs_vec ptrs;

		ptrs.reserve( allocable_chunks );
		BOOST_CHECKPOINT( "Allocating maximum allowed number of the objects" );
		for( size_t i = 0; i < allocable_chunks && test::is_valid_ptr( p ); ++i )
		{
			p = segmgr.allocate( chunk_size, std::nothrow_t() );
			ptrs.push_back( p );
		}

		std::random_shuffle( ptrs.begin(), ptrs.end() );

		for ( ptrs_vec::const_iterator it = ptrs.begin(); it != ptrs.end(); ++it )
		{
			block_id_type p = *it;
			void* p_base = segmgr.get_offset_base( p );
			void* vp = segmgr.offset_to_pointer( p );
			void* vp_base = segmgr.get_ptr_base( vp );
			BOOST_CHECK_EQUAL( vp_base, p_base );

			size_t off = segmgr.pointer_to_offset( vp );
			BOOST_CHECK_EQUAL( off, p );

			segmgr.deallocate( *it, chunk_size );
		}

		BOOST_CHECK( segmgr.is_free() );
	
	}

	BOOST_AUTO_TEST_CASE( test_null_ptr )
	{
		test::test_null_pointer_dealloc_seg<pconv_segmgr_type>();
		test::test_null_pointer_dealloc_seg<sz_pconv_segmgr_type>();
	}

	BOOST_AUTO_TEST_CASE( test_inv_offset )
	{
		test::test_null_pointer_dealloc_seg<segmgr_type>();
	}

BOOST_AUTO_TEST_SUITE_END();


