
set(LIBRARY_NAME memory-mgr)

add_library(${LIBRARY_NAME} INTERFACE)

target_include_directories(${LIBRARY_NAME} INTERFACE include)
target_link_libraries(${LIBRARY_NAME} INTERFACE VendorBoost gstl)

add_subdirectory(tests)
