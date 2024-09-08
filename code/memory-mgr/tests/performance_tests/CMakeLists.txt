
add_executable(memory-mgr-performance_tests)
target_sources(memory-mgr-performance_tests PRIVATE
  $<IF:$<PLATFORM_ID:Windows>,detail/win_perf_timer.cpp,detail/posix_perf_timer.cpp>
  detail/test.cpp
  perf_tests.cpp
  test_math.cpp
  test_memory_manager.cpp
  test_managed_base.cpp
  test_pool.cpp
  test_offset_pointer.cpp
  test_multithreaded.cpp
)

# FIXME: CODECRAFT-98 throws bad_alloc
# target_compile_definitions(memory-mgr-performance_tests PRIVATE INCLUDE_BOOST_INTERPROCESS_TESTS)

target_include_directories(memory-mgr-performance_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(memory-mgr-performance_tests PRIVATE memory-mgr memory-mgr_tests_shared)
