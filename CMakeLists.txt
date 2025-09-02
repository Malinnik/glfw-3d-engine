cmake_minimum_required(VERSION 3.12)
project(3d-engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake")

# Установка путей для исходных файлов
set(SOURCE src/main.cpp)
set(SRC src/modules)
include_directories(src/include)

set(CORE_SRC
    ${SRC}/window.cpp 
    ${SRC}/gui.cpp
    ${SRC}/shader.cpp
    ${SRC}/transform.cpp
    ${SRC}/triangle.cpp
    ${SRC}/events.cpp
    ${SRC}/file.cpp
)

# Поиск пакетов
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(loguru CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Создание исполняемого файла
add_executable(3d-engine ${SOURCE} ${CORE_SRC})

# Подключение библиотек
target_link_libraries(3d-engine PRIVATE
    glad::glad
    glfw
    glm::glm
    imgui::imgui
    loguru::loguru
    fmt::fmt
    ${OPENGL_LIBRARIES}
)

# Копирование ассетов
add_custom_command(TARGET 3d-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:3d-engine>/assets
)

# Альтернативный способ копирования ассетов
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})