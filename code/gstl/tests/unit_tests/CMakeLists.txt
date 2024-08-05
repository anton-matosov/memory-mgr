set(TARGET gstl_unit_tests)

add_executable(${TARGET})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET} gstl memory-mgr)

target_sources(${TARGET} PRIVATE
  test_algorithm.cpp
  test_allocator.cpp
  test_basic_string.cpp
  test_boost_range.cpp
  test_char_traits.cpp
  test_class.cpp
  # test_deque.cpp # TODO: Implement deque
  test_dynamic_buffer.cpp
  test_iterator.cpp
  test_list.cpp
  test_named_allocator.cpp
  test_pointer_traits.cpp
  test_queue.cpp
  test_stack.cpp
  test_utilities.cpp
  test_vector.cpp
  unit_tests.cpp
)

add_boost_test(${TARGET})

