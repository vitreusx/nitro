add_library(vcl INTERFACE)

file(GLOB_RECURSE VCL_HEADERS include/vcl/*.h)
target_sources(vcl INTERFACE "${VCL_HEADERS}")

target_include_directories(vcl
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

set_property(TARGET vcl PROPERTY VERSION 2.01.04)

#add_executable(dispatch_example1)
#target_sources(dispatch_example1 PRIVATE src/dispatch_example1.cpp)
#target_link_libraries(dispatch_example1 PRIVATE vcl)
#
#add_executable(dispatch_example2)
#target_sources(dispatch_example2 PRIVATE src/dispatch_example2.cpp)
#target_link_libraries(dispatch_example2 PRIVATE vcl)
#
#add_executable(instrset_detect)
#target_sources(instrset_detect PRIVATE src/instrset_detect.cpp src/instrset.h)
#target_link_libraries(instrset_detect PRIVATE vcl)
