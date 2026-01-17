add_library(rt_core INTERFACE)

target_include_directories(rt_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(rt_core INTERFACE rt_geom rt_reader rt_utils)
