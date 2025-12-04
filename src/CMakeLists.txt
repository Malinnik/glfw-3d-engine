# Установка путей для исходных файлов
set(SOURCE main.cpp)

file(GLOB_RECURSE CORE_SRC
    "*.cpp"
    "engine/*.cpp"
    "files/*.cpp"
    "graphics/*.cpp"
    "loaders/*.cpp"
    "ui/*.cpp"
    "window/*.cpp"
    "block/*.cpp"
    "world/*.cpp"
)

# Создание исполняемого файла
add_executable(${PROJECT_TARGET} ${SOURCE} ${CORE_SRC})
target_link_libraries(${PROJECT_TARGET} PRIVATE ${ENGINE_DEPENDENCIES})

target_include_directories(${PROJECT_TARGET} PRIVATE .)

copy_assets_for_target(${PROJECT_TARGET})