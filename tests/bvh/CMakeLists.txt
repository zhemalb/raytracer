function(add_target NAME FILE)
  add_catch(${NAME} ${FILE})

  target_link_libraries(${NAME} PRIVATE rt_core)
  target_include_directories(${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/tests)
  target_compile_definitions(${NAME}
    PRIVATE RT_TESTDATA_DIR="${PROJECT_SOURCE_DIR}/testdata/bvh"
  )
endfunction()

add_target(test_raytracer_bvh_asan test_asan.cpp)
add_target(test_raytracer_bvh_release test_release.cpp)
