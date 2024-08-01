INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/perf_tests -I$(top_srcdir)/boost \
	-I$(top_srcdir)/managers
METASOURCES = AUTO
noinst_HEADERS = posix_perf_timer.h win_perf_timer.h posix_perf_timer.h \
	win_perf_timer.h
noinst_LIBRARIES = libdetail.a
libdetail_a_SOURCES = posix_perf_timer.cpp posix_perf_timer.h\
	win_perf_timer.cpp	win_perf_timer.h test.cpp test.h posix_perf_timer.cpp\
	posix_perf_timer.h test.cpp test.h win_perf_timer.cpp win_perf_timer.h
