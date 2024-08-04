add_subdirectory(unit_tests)


add_executable(gstl-instantiations)
target_sources(gstl-instantiations PRIVATE instantiations.cpp)
target_link_libraries(gstl-instantiations gstl)
