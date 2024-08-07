
#include "gstl/detail/algorithm.hpp"
#include "gstl/detail/allocator.hpp"
#include "gstl/detail/assert.hpp"
#include "gstl/detail/basic_string.hpp"
#include "gstl/detail/boost_warnings_off.hpp"
#include "gstl/detail/boost_warnings_on.hpp"
#include "gstl/detail/buffer_helpers.hpp"
#include "gstl/detail/char_traits.hpp"
#include "gstl/detail/checked_iterator.hpp"
#include "gstl/detail/container_helpers.hpp"
#include "gstl/detail/deque.hpp"
#include "gstl/detail/dynamic_buffer.hpp"
#include "gstl/detail/fill_iterator.hpp"
#include "gstl/detail/helpers.hpp"
#include "gstl/detail/iterator.hpp"
#include "gstl/detail/iterator_declarer.hpp"
#include "gstl/detail/list.hpp"
#include "gstl/detail/list_base.hpp"
#include "gstl/detail/list_iterator.hpp"
#include "gstl/detail/list_node.hpp"
#include "gstl/detail/map.hpp"
#include "gstl/detail/pointer_traits.hpp"
#include "gstl/detail/priority_queue.hpp"
#include "gstl/detail/queue.hpp"
#include "gstl/detail/raw_memory.hpp"
#include "gstl/detail/sequence_iterator.hpp"
#include "gstl/detail/stack.hpp"
#include "gstl/detail/temp_buffer.hpp"
#include "gstl/detail/types.hpp"
#include "gstl/detail/utility.hpp"
#include "gstl/detail/vector.hpp"


#include "gstl/algorithm"
#include "gstl/allocator"
#include "gstl/deque"
#include "gstl/functional"
#include "gstl/hash.hpp"
#include "gstl/iterator"
#include "gstl/list"
#include "gstl/map"
#include "gstl/memory"
#include "gstl/pointer_traits"
#include "gstl/priority_queue"
#include "gstl/queue"
#include "gstl/stack"
#include "gstl/string"
#include "gstl/string_serialization.hpp"
#include "gstl/utility"
#include "gstl/vector"


template class gstl::basic_string<char>;
template class gstl::basic_string<wchar_t>;

template class gstl::vector<int>;
template class gstl::vector<std::pair<int, int>>;

template class gstl::list<int>;
template class gstl::list<std::pair<gstl::string, gstl::wstring>>;

template class gstl::deque<int>;
template class gstl::stack<int>;

template class gstl::priority_queue<int>;

template class gstl::queue<int, gstl::list<int>>; // queue needs deque which is not implemented yet, so use list instead
template class gstl::map<int, int>;
template class gstl::map<gstl::string, gstl::wstring>;

int main()
{
    return 0;
}
