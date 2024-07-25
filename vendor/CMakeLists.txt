set(DOWNLOADS_DIRECTORY "${CMAKE_BINARY_DIR}/downloads")

add_library(Boost INTERFACE)
target_include_directories(Boost INTERFACE ${DOWNLOADS_DIRECTORY}/boost_1_51_0)
