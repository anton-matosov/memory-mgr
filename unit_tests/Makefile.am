INCLUDES = -I$(top_srcdir)/managers

noinst_PROGRAMS = unit_tests
unit_tests_LDFLAGS = -pthread -lrt
unit_tests_SOURCES = stdafx.cpp stdafx.h test_bit_manager.cpp test_case.cpp \
				test_case.h test_class.cpp test_class.h test_managed_base.cpp \
				test_memory_manager.cpp test_memory_segment.cpp test_pointer_convert.cpp test_size_tracking.cpp \
				test_static_bitset.cpp test_type_manip.cpp unit_tests.cpp test_offset_pointer.cpp \
			test_shared_segment.cpp test_temp_buffer.cpp test_allocator.cpp
