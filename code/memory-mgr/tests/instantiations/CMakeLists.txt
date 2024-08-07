
add_executable(memory-mgr-instantiations)
target_sources(memory-mgr-instantiations PRIVATE instantiations.cpp)
target_link_libraries(memory-mgr-instantiations memory-mgr)
