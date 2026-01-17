add_catch(test_raytracer_reader test.cpp)

target_link_libraries(test_raytracer_reader PRIVATE rt_reader rt_utils)
target_include_directories(test_raytracer_reader PRIVATE ${PROJECT_SOURCE_DIR}/tests)
target_compile_definitions(test_raytracer_reader
  PRIVATE RT_TESTDATA_DIR="${PROJECT_SOURCE_DIR}/testdata/reader"
)
