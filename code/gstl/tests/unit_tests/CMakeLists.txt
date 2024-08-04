set(TARGET gstl_unit_tests)

add_executable(${TARGET})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TARGET} gstl memory-mgr)

target_sources(${TARGET} PRIVATE
  # test_basic_string.cpp # Not ready yet
  unit_tests.cpp
)

add_test(NAME  ${TARGET} COMMAND  ${TARGET} --color_output=no --report_level=detailed)

# Set properties for the test
set_tests_properties( ${TARGET} PROPERTIES
    PASS_REGULAR_EXPRESSION "assertions.*passed"
    FAIL_REGULAR_EXPRESSION "assertions.*failed"
    # SKIP_REGULAR_EXPRESSION "skipped"
)
