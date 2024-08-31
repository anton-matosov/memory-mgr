
function(add_boost_test TARGET)
  add_test(NAME  ${TARGET} COMMAND  ${TARGET} --color_output=no
    # --log_level=all # Enable these options for debugging in CI
    # --catch_system_errors=no # This option makes memory access violation non detected on Windows
  )

  # Set properties for the test
  set_tests_properties( ${TARGET} PROPERTIES
      PASS_REGULAR_EXPRESSION "No errors detected"
      FAIL_REGULAR_EXPRESSION "failures are detected|fatal error"
  )
endfunction()


add_subdirectory(gstl)
add_subdirectory(memory-mgr)
