add_library(rt_utils INTERFACE)

target_include_directories(rt_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(TARGET PNG::PNG)
  target_link_libraries(rt_utils INTERFACE PNG::PNG)
else()
  target_link_libraries(rt_utils INTERFACE ${PNG_LIBRARY})
  target_include_directories(rt_utils INTERFACE ${PNG_INCLUDE_DIRS})
endif()
