cmake_minimum_required(VERSION 3.30.0)
project(memory-mgr LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(unit_tests)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
