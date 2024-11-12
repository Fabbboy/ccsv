enable_testing()

set(TEST_EXECUTABLES)
set(DATA_FOLDER_PATH "${CMAKE_SOURCE_DIR}/data")
set(DATA_DEST_PATH "${CMAKE_BINARY_DIR}/tests/data")

add_custom_target(copy_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${DATA_FOLDER_PATH}" "${DATA_DEST_PATH}"
    COMMENT "Copying data folder to binary directory"
)



file(GLOB_RECURSE TEST_SOURCES "./test_*.c")

foreach(TEST_SOURCE ${TEST_SOURCES})
   get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

   add_executable(${TEST_NAME} ${TEST_SOURCE})

   target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

   add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
   list(APPEND TEST_EXECUTABLES ${TEST_NAME})
 endforeach()


add_custom_target(
  test
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS ${CTEST_EXECUTABLES}
)
