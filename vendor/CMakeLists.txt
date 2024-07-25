set(DOWNLOADS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/downloads")

add_library(Boost INTERFACE)
# target_include_directories(Boost INTERFACE ${DOWNLOADS_DIRECTORY}/boost_1_51_0)
target_include_directories(Boost INTERFACE ${DOWNLOADS_DIRECTORY}/boost_1_85_0)

# find_package(Boost REQUIRED COMPONENTS mpl)
