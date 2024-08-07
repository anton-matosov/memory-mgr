
#include "memory-mgr/pool/object_pool.hpp"
#include "memory-mgr/pool/pool.hpp"
#include "memory-mgr/pool/pool_alloc.hpp"
#include "memory-mgr/pool/poolfwd.hpp"
#include "memory-mgr/pool/simple_segregated_storage.hpp"
#include "memory-mgr/pool/singleton_pool.hpp"

#include "memory-mgr/allocator.h"
#include "memory-mgr/allocator_decorator.h"
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
#include "memory-mgr/detail/polymorphic_allocator.h"
#include "memory-mgr/detail/ptr_casts.h"
#include "memory-mgr/detail/ptr_helpers.h"
#include "memory-mgr/detail/segment_storage_base.h"
#include "memory-mgr/detail/segment_storage_map.h"
#include "memory-mgr/detail/segment_storage_vector.h"
#include "memory-mgr/detail/singleton.h"
#include "memory-mgr/detail/singleton_allocator_impl.h"
#include "memory-mgr/detail/sp_typeinfo.hpp"
#include "memory-mgr/detail/static_assert.h"
#include "memory-mgr/detail/static_bitset.h"
#include "memory-mgr/detail/temp_buffer.h"
#include "memory-mgr/detail/type_manip.h"
#include "memory-mgr/detail/types.h"
#include "memory-mgr/detail/vector_as_allocator.h"
#include "memory-mgr/detail/virtualmem_allocator.h"

template class memory_mgr::static_bitset<unsigned char, 32, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned short, 64, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned int, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long long, 256, memory_mgr::static_array>;

int main()
{
    return 0;
}
