cmake_minimum_required(VERSION 3.15)
project("benchmark"
    VERSION 0.0.1
    LANGUAGES CXX
)
message("\nStarted CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE
        STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel"
        FORCE)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
    REQUIRES
        nanobench/4.0.0
        spy/0.0.3
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

set(COMPILER_WARNINGS
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Werror -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4>)

add_executable(benchmark_nucleotide_count benchmark_nucleotide_count.cpp)
target_compile_features(benchmark_nucleotide_count PRIVATE cxx_std_17)
target_compile_options(benchmark_nucleotide_count PRIVATE ${COMPILER_WARNINGS})
target_link_libraries(benchmark_nucleotide_count CONAN_PKG::nanobench)
target_link_libraries(benchmark_nucleotide_count CONAN_PKG::spy)

# vim:set sw=4 et:
