
#pragma once

#ifndef NDEBUG

namespace test
{
	class detect_memory_leaks
	{
	public:
		detect_memory_leaks();
		~detect_memory_leaks();

		static void enable_leaks_detection();
		static void break_on_memory_alloc( long memory_alloc_order_number );
		static void free_type_info_memory();
	};
}

#	define DETECT_MEMORY_LEAKS test::detect_memory_leaks g_leaks_detector;
#else
#	define DETECT_MEMORY_LEAKS
#endif

