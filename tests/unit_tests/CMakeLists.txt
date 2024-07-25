set(TARGET unit_tests)

add_executable(${TARGET})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET} memory-mgr)

target_sources(${TARGET} PRIVATE
  # pool/sys_allocator.hpp
  # pool/test_pool_alloc.cpp
  # pool/time_pool_alloc.cpp

  common_manager_tests.h
  # detect_memory_leaks.cpp
  # detect_memory_leaks.h
  managers.h

  # test_allocator.cpp
  # test_bit_manager.cpp
  # test_class.cpp
  # test_class.h
  # test_intrusive_ptr.cpp
  # test_low_fragmentation_manager.cpp
  # test_managed_base.cpp
  # test_math.cpp
  # test_memory_manager.cpp
  # test_memory_segment.cpp
  # test_named_objects.cpp
  # test_new.cpp
  # test_offset_pointer.cpp
  # test_pool.cpp
  # test_segment_manager.cpp
  # test_shared_ptr.cpp
  # test_shared_segment.cpp
  # test_size_tracking.cpp
  test_static_bitset.cpp
  # test_temp_buffer.cpp
  # test_type_manip.cpp
  stdafx.cpp
  stdafx.h
  unit_tests.cpp
)


