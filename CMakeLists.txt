cmake_minimum_required(VERSION 2.8)

project(game)
add_executable(game game.cpp video.cpp)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLU REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(game ${OPENGL_LIBRARIES} ${GLU_LIBRARY} ${GLEW_LIBRARY} glfw)

