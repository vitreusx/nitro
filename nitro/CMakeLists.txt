add_library(nitro INTERFACE)

file(GLOB_RECURSE PrivateSources CONFIGURE_DEPENDS
    src/*.cpp src/*.hpp src/*.h src/*.inl)

file(GLOB_RECURSE PublicSources CONFIGURE_DEPENDS
    include/*.hpp include/*.h include/*.inl)

target_sources(nitro
    PRIVATE ${PrivateSources}
    PUBLIC ${PublicSources})

target_compile_features(nitro
    # PUBLIC cxx_std_17)
    INTERFACE cxx_std_17)

target_include_directories(nitro
    # PRIVATE
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/nitro>
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    # $<INSTALL_INTERFACE:include/nitro>
    # PUBLIC
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_options(nitro
    # PRIVATE
    INTERFACE
    $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:-g>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-g;-O2;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)
    