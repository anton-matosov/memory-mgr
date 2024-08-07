
set(LIBRARY_NAME memory-mgr)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE include)
target_link_libraries(${LIBRARY_NAME} INTERFACE VendorBoost gstl $<$<PLATFORM_ID:Linux>:pthread>)

target_compile_definitions(${LIBRARY_NAME} INTERFACE $<$<PLATFORM_ID:Windows>:NOMINMAX>)
target_compile_definitions(${LIBRARY_NAME} INTERFACE $<$<CONFIG:DEBUG>:MGR_ENABLE_ASSERTS>)

add_subdirectory(tests)
add_subdirectory(examples)
