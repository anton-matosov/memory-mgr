

noinst_PROGRAMS = unit_tests
unit_tests_LDFLAGS = -pthread -lrt


INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/boost
unit_tests_SOURCES = common_manager_tests.h managers.h stdafx.cpp stdafx.h\
	test_allocator.cpp test_bit_manager.cpp test_class.cpp test_class.h\
	test_managed_base.cpp test_memory_manager.cpp test_memory_segment.cpp\
	test_named_objects.cpp test_new.cpp test_offset_pointer.cpp\
	test_pointer_convert.cpp test_segment_manager.cpp\
	test_shared_segment.cpp test_size_tracking.cpp test_static_bitset.cpp\
	test_temp_buffer.cpp test_type_manip.cpp unit_tests.cpp

unit_tests_LDADD = $(top_builddir)/boost/libboost.a
