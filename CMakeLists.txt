cmake_minimum_required(VERSION 3.30.0)
project(memory-mgr LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_subdirectory(vendor)

set(LIBRARY_NAME memory-mgr)
add_library(${LIBRARY_NAME} INTERFACE)
target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${LIBRARY_NAME} INTERFACE Boost)

add_subdirectory(tests/unit_tests)
add_subdirectory(tests/performance_tests)


# set(CPACK_PROJECT_NAME ${LIBRARY_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
