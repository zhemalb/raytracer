add_catch(test_raytracer_geom test.cpp)

target_link_libraries(test_raytracer_geom PRIVATE rt_geom rt_utils)
target_include_directories(test_raytracer_geom PRIVATE ${PROJECT_SOURCE_DIR}/tests)
target_compile_definitions(test_raytracer_geom
  PRIVATE RT_TESTDATA_DIR="${PROJECT_SOURCE_DIR}/testdata/geom"
)
