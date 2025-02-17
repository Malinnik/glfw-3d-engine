cmake_minimum_required( VERSION 3.12 )
project( 3d-engine )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE src/main.cpp)

#OPENGL 
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

# GLAD
set ( GLAD_SRC dependencies/glad/src/glad.c)
include_directories(dependencies/glad/include)

# GLFW
add_subdirectory( dependencies/glfw )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

option( USE-GLFW-GLAD "Use GLAD from GLFW" ON )

if( USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()


# ImGui
set(imgui-directory dependencies/imgui)
set(imgui-source ${imgui-directory}/imconfig.h
	${imgui-directory}/imgui.h
	${imgui-directory}/imgui.cpp
	${imgui-directory}/imgui_draw.cpp
	${imgui-directory}/imgui_internal.h
	${imgui-directory}/imgui_widgets.cpp
	${imgui-directory}/imstb_rectpack.h
	${imgui-directory}/imstb_textedit.h
	${imgui-directory}/imstb_truetype.h
 	${imgui-directory}/imgui_tables.cpp
	${imgui-directory}/imgui_demo.cpp
	${imgui-directory}/backends/imgui_impl_glfw.cpp
	${imgui-directory}/backends/imgui_impl_opengl3.cpp
)
include_directories(dependencies/imgui)
include_directories(dependencies/imgui/backends)


# SRC
set (SRC src/modules)
include_directories(src/include)
set (CORE_SRC
	${SRC}/window.cpp 
	${SRC}/gui.cpp
	${SRC}/shader.cpp
	${SRC}/transform.cpp
	${SRC}/triangle.cpp
	${SRC}/vec.cpp
	${SRC}/events.cpp
)

# Build
add_executable( 3d-engine WIN32 ${SOURCE} ${GLAD_SRC} ${GLAD_GL} ${imgui-source} ${CORE_SRC})
target_link_libraries( 3d-engine ${OPENGL_LIBRARIES} glfw )
target_include_directories(3d-engine PRIVATE ${imgui-directory})
