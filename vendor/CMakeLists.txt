set(DOWNLOADS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/downloads")

add_library(VendorBoost INTERFACE)
# target_include_directories(VendorBoost INTERFACE ${DOWNLOADS_DIRECTORY}/boost_1_51_0)
# target_include_directories(VendorBoost INTERFACE ${DOWNLOADS_DIRECTORY}/boost_1_85_0)

find_package(Boost 1.85.0 EXACT REQUIRED COMPONENTS headers)


target_link_libraries(VendorBoost INTERFACE Boost::headers)
