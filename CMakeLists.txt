cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME "3d-engine")

project(${PROJECT_NAME})
set(PROJECT_TARGET ${PROJECT_NAME} CACHE STRING "Executable name" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake")


include(cmake/dependencies.cmake)
include(cmake/assets.cmake)
add_subdirectory(src)