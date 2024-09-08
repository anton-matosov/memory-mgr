
add_executable(memory-mgr-performance_tests)
target_sources(memory-mgr-performance_tests PRIVATE
  # perf_tests.cpp
  # test_math.cpp
  test_offset_pointer.cpp
  detail/test.cpp
  $<IF:$<PLATFORM_ID:Windows>,detail/win_perf_timer.cpp,detail/posix_perf_timer.cpp>
)
target_include_directories(memory-mgr-performance_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(memory-mgr-performance_tests PRIVATE memory-mgr memory-mgr_tests_shared)
