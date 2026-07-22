project(imgui)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
add_subdirectory(glfw)

add_library(${PROJECT_NAME} imgui/imgui_demo.cpp
                            imgui/imgui.cpp
                            imgui/imgui_draw.cpp
                            imgui/imgui_tables.cpp
                            imgui/imgui_widgets.cpp
                            imgui/backends/imgui_impl_glfw.cpp
                            imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui" "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends")
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL glfw)
