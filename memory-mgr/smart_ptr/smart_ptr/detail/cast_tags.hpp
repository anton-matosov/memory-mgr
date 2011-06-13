#ifndef MEMORY_MGR_SMART_PTR_CAST_TAGS_HPP_INCLUDED
#define MEMORY_MGR_SMART_PTR_CAST_TAGS_HPP_INCLUDED

//  This file is the adaptation for Generic Memory Manager library
// 
// 

namespace memory_mgr
{
	namespace detail
	{

		struct static_cast_tag {};
		struct const_cast_tag {};
		struct dynamic_cast_tag {};
		struct polymorphic_cast_tag {};

	}
}

#endif //MEMORY_MGR_SMART_PTR_CAST_TAGS_HPP_INCLUDED
