add_library(rt_reader INTERFACE)

target_include_directories(rt_reader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(rt_reader INTERFACE rt_geom)
