#include "memory-mgr/allocator.h"
#include "memory-mgr/config/config.h"
#include "memory-mgr/heap_segment.h"
#include "memory-mgr/manager_category.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/memory_manager.h"
#include "memory-mgr/offset_ptr.h"
#include "memory-mgr/segment_traits.h"
#include "memory-mgr/singleton_manager.h"
#include "memory-mgr/size_tracking.h"

#include "memory-mgr/detail/aux_data_decorator.h"
#include "memory-mgr/detail/bit_manager.h"
#include "memory-mgr/detail/decorator_base.h"
#include "memory-mgr/detail/offset_ptr_base.h"
#include "memory-mgr/detail/ptr_helpers.h"
#include "memory-mgr/detail/static_bitset.h"
#include "memory-mgr/detail/types.h"

#include "memory-mgr/memory_debug.h"
#include "memory-mgr/memory_header.h"
#include "memory-mgr/memory_segment.h"
#include "memory-mgr/memory_usage_tracker.h"

template class memory_mgr::static_bitset<unsigned char, 32, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned short, 64, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned int, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long, 128, memory_mgr::static_array>;
template class memory_mgr::static_bitset<unsigned long long, 256, memory_mgr::static_array>;

int main()
{
    return 0;
}
