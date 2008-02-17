INCLUDES = -I$(top_srcdir)/managers
METASOURCES = AUTO
SUBDIRS = detail
noinst_PROGRAMS = perf_tests perf_tests
perf_tests_LDFLAGS = -pthread -lrt
perf_tests_SOURCES = perf_tests.cpp perf_timer.h test_bit_manager.cpp\
	test_memory_manager.cpp test_static_bitset.cpp
