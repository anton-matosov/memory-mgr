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

#ifndef MGR_MANAGERS_HEADER
#define MGR_MANAGERS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/memory_manager.h>
#include <memory-mgr/heap_segment.h>
#include <memory-mgr/shared_segment.h>
#include <memory-mgr/new.h>
#include <memory-mgr/size_tracking.h>
#include <memory-mgr/managed_base.h>
#include <memory-mgr/singleton_manager.h>
//#include <memory-mgr/segment_manager.h>
#include <memory-mgr/sync/pseudo_sync.h>

#include <memory-mgr/params_binder.h>
#include <memory-mgr/sync/named_mutex.h>

typedef int chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 200 * 1024 * 1024;
static const size_t memory_size_small = 32 * 1024;
static const size_t segments_count = 1024;

MGR_DECLARE_BIND_PARAM( MutexName2, const char*, "Default memory sync 2" );
typedef MGR_BINDED( memory_mgr::sync::named_mutex, MutexName2 ) def_named_mutex2;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size, def_named_mutex2 > memmgr_type;
typedef memory_mgr::memory_manager<chunk_type, memory_size_small, chunk_size, def_named_mutex2 > memmgr_small_type;

typedef memory_mgr::heap_segment< memmgr_type > heap_mgr;

typedef memory_mgr::shared_segment< memmgr_type > shared_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memmgr_type
	>
> heap_sz_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::heap_segment
<
	memmgr_type
> heap_mgr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// typedef memory_mgr::segment_manager
// < 
// 	memory_mgr::heap_segment
// 	<
// 		memmgr_small_type
// 	>
// 	,
// 	segments_count
// > seg_heap_mgr;


// typedef memory_mgr::size_tracking
// <
// 	memory_mgr::segment_manager
// 	< 
// 		memory_mgr::heap_segment
// 		<
// 			memmgr_small_type
// 		>
// 		,
// 		segments_count
// 	> 
// > seg_heap_sz_mgr;

// 
// typedef memory_mgr::singleton_manager
// < 
// 	seg_heap_sz_mgr
// > sing_seg_heap_sz_mgr;
//////////////////////////////////////////////////////////////////////////
// 
// typedef memory_mgr::segment_manager
// < 
// 	memory_mgr::shared_segment
// 	<
// 		memmgr_small_type
// 	>
// 	,
// 	segments_count
// > seg_shared_mgr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::shared_segment
<
	memory_mgr::size_tracking
	< 
		memmgr_type
	>
> shared_sz_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::size_tracking
		< 
			memmgr_type
		>
	>
> sing_heap_sz_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::singleton_manager
< 
	memory_mgr::shared_segment
	< 
		memory_mgr::size_tracking
		< 
			memmgr_type 
		>
	>
> sing_shared_sz_mgr;


#endif//MGR_MANAGERS_HEADER
