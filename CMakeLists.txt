cmake_minimum_required(VERSION 3.25)
project(software_rasterizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RASTER_SANITIZE "Build Debug with AddressSanitizer and UBSan" ON)

# Warning flags live on their own target so they reach our code, never third-party code.
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
  -Wall -Wextra -Wpedantic
  -Wconversion -Wsign-conversion -Wdouble-promotion
  -Wshadow
  -Werror)

# Sanitizers, Debug only. GCC does not include float-cast-overflow in -fsanitize=undefined,
# so it is listed explicitly.
add_library(project_sanitizers INTERFACE)
if(RASTER_SANITIZE)
  target_compile_options(project_sanitizers INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined,float-cast-overflow>
    $<$<CONFIG:Debug>:-fno-sanitize-recover=all>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>)
  target_link_options(project_sanitizers INTERFACE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined,float-cast-overflow>)
endif()

# Everything that gets tested lives in the library.
add_library(raster
  src/color.cpp
  src/framebuffer.cpp
  src/ppm.cpp
  src/scenes.cpp)
target_include_directories(raster PUBLIC src)
target_link_libraries(raster
  PUBLIC project_sanitizers
  PRIVATE project_warnings)

add_executable(render app/render.cpp)
target_link_libraries(render PRIVATE raster project_warnings)

# Tests: GoogleTest from the system if present, otherwise a pinned download.
enable_testing()
include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
  URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
  SYSTEM
  FIND_PACKAGE_ARGS NAMES GTest)
FetchContent_MakeAvailable(googletest)

add_executable(raster_tests
  tests/color_test.cpp
  tests/framebuffer_test.cpp
  tests/ppm_test.cpp
  tests/scenes_test.cpp)
target_link_libraries(raster_tests PRIVATE raster project_warnings GTest::gtest_main)
target_compile_definitions(raster_tests PRIVATE
  RASTER_REFERENCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/reference")

include(GoogleTest)
gtest_discover_tests(raster_tests)
