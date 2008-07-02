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

#include "memory_manager.h"
#include "heap_segment.h"
#include "shared_segment.h"
#include "new.h"
#include "size_tracking.h"
#include "managed_base.h"
#include "singleton_manager.h"
#include "segment_manager.h"
#include "sync/pseudo_sync.h"

typedef unsigned int chunk_type;
static const size_t chunk_size = 4;
static const size_t memory_size = 4 * 1024 * 1024;
static const size_t memory_size_small = 32 * 1024;
static const size_t segments_count = 1024;

typedef memory_mgr::memory_manager<chunk_type, memory_size, chunk_size, size_t, memory_mgr::sync::pseudo_sync > memmgr_type;
typedef memory_mgr::memory_manager<chunk_type, memory_size_small, chunk_size, size_t, memory_mgr::sync::pseudo_sync > memmgr_small_type;

typedef memmgr_type::offset_type offset_type;

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
	memory_mgr::pointer_convert
	< 
		memmgr_type
	>
> heap_pt_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::heap_segment
<
	memory_mgr::size_tracking
	< 
		memory_mgr::pointer_convert
		< 
			memmgr_type
		> 
	>
> heap_sz_pt_mgr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::segment_manager
< 
	typedef memory_mgr::heap_segment
	<
		memmgr_small_type
	>
	,
	segments_count
> seg_heap_mgr;


typedef memory_mgr::size_tracking
<
	memory_mgr::segment_manager
	< 
		typedef memory_mgr::heap_segment
		<
			memmgr_small_type
		>
		,
		segments_count
	> 
> seg_heap_sz_mgr;

typedef memory_mgr::pointer_convert
<
	memory_mgr::segment_manager
	< 
		typedef memory_mgr::heap_segment
		<
			memmgr_small_type
		>
		,
		segments_count
	> 
> seg_heap_pt_mgr;

typedef memory_mgr::size_tracking
<
	memory_mgr::pointer_convert
	< 
		memory_mgr::segment_manager
		< 
			typedef memory_mgr::heap_segment
			<
				memmgr_small_type
			>
			,
			segments_count
		> 
	>
> seg_heap_sz_pt_mgr;

typedef memory_mgr::singleton_manager
< 
	seg_heap_sz_pt_mgr
> sing_seg_heap_sz_pt_mgr;
//////////////////////////////////////////////////////////////////////////

typedef memory_mgr::segment_manager
< 
	typedef memory_mgr::shared_segment
	<
		memmgr_small_type
	>
	,
	segments_count
> seg_shared_mgr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::shared_segment
<
	memory_mgr::size_tracking
	< 
		memory_mgr::pointer_convert
		< 
			memmgr_type
		> 
	>
> shared_sz_pt_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::singleton_manager
< 
	memory_mgr::heap_segment
	< 
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memmgr_type
			> 
		>
	>
> sing_heap_sz_pt_mgr;

//////////////////////////////////////////////////////////////////////////
typedef memory_mgr::singleton_manager
< 
	memory_mgr::shared_segment
	< 
		memory_mgr::size_tracking
		< 
			memory_mgr::pointer_convert
			< 
				memmgr_type
			> 
		>
	>
> sing_shared_sz_pt_mgr;


#endif//MGR_MANAGERS_HEADER
