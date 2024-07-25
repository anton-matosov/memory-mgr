
#include "StdAfx.h"
#include <crtdbg.h>
#include <typeinfo>
#include "detect_memory_leaks.h"

#ifndef NDEBUG

namespace test
{
	detect_memory_leaks::detect_memory_leaks()
	{
		enable_leaks_detection();
	}

	void detect_memory_leaks::enable_leaks_detection()
	{
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ) | _CRTDBG_LEAK_CHECK_DF);
	}

	void detect_memory_leaks::break_on_memory_alloc( long memory_alloc_order_number )
	{
		_CrtSetBreakAlloc( memory_alloc_order_number );
	}

	detect_memory_leaks::~detect_memory_leaks()
	{
		free_type_info_memory();
	}

	void detect_memory_leaks::free_type_info_memory()
	{
#ifdef MGR_NEEDS_TO_CLEANUP_TYPEINFO_LEAKS
		__type_info_node *node = __type_info_root_node.next;
		__type_info_node *tmpNode = &__type_info_root_node;

		for( ; node!=NULL; node = tmpNode )
		{
			tmpNode = node->next;
			delete node->memPtr;
			delete node;
		}	
#endif
	}
}
#endif

