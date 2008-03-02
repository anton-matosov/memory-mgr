INCLUDES = -I$(top_srcdir)/managers
METASOURCES = AUTO
SUBDIRS = detail

noinst_PROGRAMS = perf_tests
perf_tests_LDFLAGS = -pthread -lrt
perf_tests_SOURCES = perf_timer.h perf_tests.cpp perf_timer.h \
	test_managed_base.cpp test_memory_manager.cpp test_offset_pointer.cpp
perf_tests_LDADD = $(top_builddir)/perf_tests/detail/libdetail.a
