set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(IQFileConverterLib iq_file_converter.cpp gnss_sdr_metadata_wrapper.cpp)
target_include_directories(IQFileConverterLib PUBLIC . ../GNSS-Metadata-Standard/api/inc ../GNSS-Metadata-Standard/converter/inc)

add_executable(IQFileConverter main.cpp)
target_include_directories(IQFileConverter PRIVATE IQFileConverterLib)
target_link_libraries(IQFileConverter IQFileConverterLib api xml cnv)
