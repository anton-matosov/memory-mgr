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

// template<class MemMgr, class OffsetT = typename boost::mpl::if_c< 
// 		is_category_supported< MemMgr, pointer_conversion_tag>::value,
// 		void*,
// 		typename manager_traits<MemMgr>::offset_type>::type>
// void test_null_ptr()
// {
// 	typedef MemMgr	memmgr_type;
// 	typedef typename memory_mgr::manager_traits<memmgr_type>::block_type	block_type;
// 	typedef typename memory_mgr::manager_traits<memmgr_type>::offset_type	offset_type;
// 
// 	std::vector<block_type> memory( memory_mgr::manager_traits<memmgr_type>::memory_size );
// 	memmgr_type mgr( &*memory.begin() );
// 
// 	offset_type null_ptr = memory_mgr::offset_traits<offset_type>::invalid_offset;
// 
// 	BOOST_CHECKPOINT( "before deallocation of null ptr" );
// 	mgr.deallocate( null_ptr, 0 );
// 	BOOST_CHECKPOINT( "after deallocation of null ptr" );
// }

#endif //MGR_COMMON_MANAGER_TESTS_HEADER


