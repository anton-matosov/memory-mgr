# set(TARGET memory-mgr_examples)

# add_executable(${TARGET})
# target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(${TARGET} memory-mgr)

# target_sources(${TARGET} PRIVATE
#     main.cpp
# )

# set(TARGET memory-mgr_test_offset_ptr_optimizer_bugtest_offset_ptr_optimizer_bug)

# add_executable(${TARGET})
# target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(${TARGET} memory-mgr)

# # skipping 1 context in backtrace; use -ftemplate-backtrace-limit=0 to see all
# add_compile_options(${TARGET} PRIVATE -ftemplate-backtrace-limit=0)

# target_sources(${TARGET} PRIVATE
#     test_offset_ptr_optimizer_bug.cpp
# )

# add_test(NAME ${TARGET} COMMAND ${TARGET})
