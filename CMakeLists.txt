cmake_minimum_required(VERSION 3.10)

project(iq-file-converter)

add_subdirectory(GNSS-Metadata-Standard)
add_subdirectory(src)
IF(${BUILD_TEST_EXECUTABLES})
add_subdirectory(test)
ENDIF(${BUILD_TEST_EXECUTABLES})
