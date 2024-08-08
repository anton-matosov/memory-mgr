
#include "memory-mgr/pool/object_pool.hpp"
#include "memory-mgr/pool/pool.hpp"
#include "memory-mgr/pool/pool_alloc.hpp"
#include "memory-mgr/pool/poolfwd.hpp"
#include "memory-mgr/pool/simple_segregated_storage.hpp"
#include "memory-mgr/pool/singleton_pool.hpp"

#include "memory-mgr/allocator_decorator.h"
#include "memory-mgr/allocator.h"
#include "memory-mgr/get_pointer.h"
#include "memory-mgr/heap_segment.h"
#include "memory-mgr/low_fragmentation_manager.h"
#include "memory-mgr/managed_base.h"
#include "memory-mgr/manager_category.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/memory_debug.h"
#include "memory-mgr/memory_header.h"
#include "memory-mgr/memory_manager.h"
#include "memory-mgr/memory_segment.h"
#include "memory-mgr/memory_usage_tracker.h"
#include "memory-mgr/named_objects.h"
#include "memory-mgr/new.h"
#include "memory-mgr/offset_allocator.h"
#include "memory-mgr/offset_pointer.h"
#include "memory-mgr/offset_ptr.h"
#include "memory-mgr/params_binder.h"
#include "memory-mgr/pointer_cast.h"
#include "memory-mgr/polymorphic_allocator.h"
#include "memory-mgr/segment_traits.h"
#include "memory-mgr/shared_segment.h"
#include "memory-mgr/singleton_manager.h"
#include "memory-mgr/size_tracking.h"


#include "memory-mgr/detail/allocator_base.h"
#include "memory-mgr/detail/assert.h"
#include "memory-mgr/detail/aux_data_decorator.h"
#include "memory-mgr/detail/bit_manager.h"
#include "memory-mgr/detail/cmp_helper.h"
#include "memory-mgr/detail/compatibility_types.h"
#include "memory-mgr/detail/decorator_base.h"
#include "memory-mgr/detail/dynamic_cast_ex.h"
#include "memory-mgr/detail/helpers.h"
#include "memory-mgr/detail/lfm_pool_id_helpers.h"
#include "memory-mgr/detail/malloc_allocator.h"
#include "memory-mgr/detail/math.h"
#include "memory-mgr/detail/member_allocator_impl.h"
#include "memory-mgr/detail/mgr_impl_allocator.h"
#include "memory-mgr/detail/named_allocator.h"
#include "memory-mgr/detail/new_helpers.h"
#include "memory-mgr/detail/offset_ptr_base.h"
#include "memory-mgr/detail/offset_traits.h"
#include "memory-mgr/detail/polymorphic_allocator_base.h"
#include "memory-mgr/detail/ptr_casts.h"
#include "memory-mgr/detail/ptr_helpers.h"
#include "memory-mgr/detail/segment_storage_base.h"
#include "memory-mgr/detail/segment_storage_map.h"
#include "memory-mgr/detail/segment_storage_vector.h"
#include "memory-mgr/detail/singleton_allocator_impl.h"
#include "memory-mgr/detail/singleton.h"
#include "memory-mgr/detail/sp_typeinfo.hpp"
#include "memory-mgr/detail/static_bitset.h"
#include "memory-mgr/detail/temp_buffer.h"
#include "memory-mgr/detail/type_manip.h"
#include "memory-mgr/detail/types.h"
#include "memory-mgr/detail/vector_as_allocator.h"
#include "memory-mgr/detail/virtualmem_allocator.h"

#include "memory-mgr/params_binder.h"
#include "memory-mgr/sync/critical_section.h"
#include "memory-mgr/sync/locks.h"
#include "memory-mgr/sync/named_mutex.h"
#include "memory-mgr/sync/pseudo_sync.h"

template class memory_mgr::static_bitset<unsigned char, 32, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned short, 64, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned int, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long long, 256, memory_mgr::static_array>;

using chunk_type = int;
static const size_t chunk_size = 4;
static const size_t memory_size = 200 * 1024 * 1024;
static const size_t segments_count = 1024;

using critical_section = memory_mgr::sync::critical_section;

MGR_DECLARE_BIND_PARAM(MutexName2, const char*, "Default memory sync 2");
typedef MGR_BINDED(memory_mgr::sync::named_mutex, MutexName2) def_named_mutex2;

using memmgr_type = memory_mgr::memory_manager<chunk_type, memory_size, chunk_size, critical_section>;
template class memory_mgr::memory_manager<chunk_type, memory_size, chunk_size, critical_section>;

template class memory_mgr::heap_segment<memmgr_type>;
template class memory_mgr::shared_segment<memmgr_type>;

using heap_sz_mgr = memory_mgr::heap_segment<memory_mgr::size_tracking<memmgr_type> >;
template class memory_mgr::heap_segment<memory_mgr::size_tracking<memmgr_type> >;

template class memory_mgr::shared_segment<memory_mgr::size_tracking<memmgr_type> >;

using sz_lfm_heap_sz_mgr = memory_mgr::size_tracking<memory_mgr::low_fragmentation_manager<memory_mgr::named_objects<heap_sz_mgr> > >;
template class memory_mgr::size_tracking<memory_mgr::low_fragmentation_manager<memory_mgr::named_objects<heap_sz_mgr> > >;
template class memory_mgr::singleton_manager<sz_lfm_heap_sz_mgr>;

template class memory_mgr::singleton_manager<memory_mgr::heap_segment<memory_mgr::size_tracking<memmgr_type> > >;
template class memory_mgr::singleton_manager<memory_mgr::shared_segment<memory_mgr::size_tracking<memmgr_type> > >;

int main()
{
    return 0;
}
