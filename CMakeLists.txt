cmake_minimum_required(VERSION 3.30)
include(imgui.cmake)

project(ClientRoboticCar CXX)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

file(COPY "consolas.ttf" DESTINATION CMAKE_CURRENT_BINARY_DIR)

add_subdirectory(glad)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} imgui glfw OpenGL::GL glad ws2_32)
