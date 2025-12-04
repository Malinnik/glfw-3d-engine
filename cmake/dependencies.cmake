# Поиск пакетов
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(loguru CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(PNG REQUIRED)

add_library(engine_dependencies INTERFACE)

# Подключение библиотек
target_link_libraries(engine_dependencies INTERFACE
    glad::glad
    glfw
    glm::glm
    imgui::imgui
    loguru::loguru
    fmt::fmt
    PNG::PNG
    ${OPENGL_LIBRARIES}
)

# Кэшируем переменную, чтобы она была видна в поддиректориях
set(ENGINE_DEPENDENCIES engine_dependencies CACHE INTERNAL "Engine dependencies")