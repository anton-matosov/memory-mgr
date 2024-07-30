
#pragma once

#include <boost/type_traits/is_polymorphic.hpp>

namespace memory_mgr
{
	namespace detail
	{
		template<bool IsClass>
		struct dynamic_cast_impl;

		template<>
		struct dynamic_cast_impl<true>
		{
			template<class To, class From>
			static To cast( From* ptr )
			{
				return dynamic_cast<To>( ptr );
			}
		};

		template<>
		struct dynamic_cast_impl<false>
		{
			template<class To, class From>
			static To cast( From* ptr )
			{
				return static_cast<To>( ptr );
			}
		};
	}

	template<class To, class From>
	To dynamic_cast_ex( From* ptr )
	{
		return detail::dynamic_cast_impl<boost::is_polymorphic<From>::value >::template cast<To>( ptr );
	}
}
