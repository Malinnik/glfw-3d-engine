cmake_minimum_required(VERSION 3.12)
project(3d-engine VERSION 1.0.0 LANGUAGES CXX)

# if(MSVC)
#     # Используем динамическую линковку C++ runtime для совместимости с vcpkg
#     set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    
#     # Отключаем предупреждения о несоответствии библиотек
#     add_compile_options(/wd4099)
    
#     # Для Debug-сборки
#     if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#         add_compile_options(/MDd)  # Динамическая отладка
#         message(STATUS "Using /MDd for Debug build")
#     else()
#         add_compile_options(/MD)   # Динамическая релиз
#         message(STATUS "Using /MD for Release build")
#     endif()
# endif()


set(PROJECT_TARGET ${PROJECT_NAME} CACHE STRING "Executable name" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake")


include(cmake/dependencies.cmake)
include(cmake/assets.cmake)


add_executable(${PROJECT_NAME} src/main.cpp)

file(GLOB_RECURSE CORE_SRC
    "src/*.cpp"
    "src/engine/*.cpp"
    "src/engine/resources/*.cpp"
    "src/files/*.cpp"
    "src/graphics/*.cpp"
    "src/loaders/*.cpp"
    "src/ui/*.cpp"
    "src/window/*.cpp"
    "src/block/*.cpp"
    "src/world/*.cpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${CORE_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_DEPENDENCIES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

copy_assets_for_target(${PROJECT_NAME})