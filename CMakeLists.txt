cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(game)

#Find_Package(ViennaCL REQUIRED)
include(ExternalProject)
Find_Package(Git REQUIRED)
ExternalProject_Add(ViennaCL_ext
          PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/external
          GIT_REPOSITORY https://github.com/viennacl/viennacl-dev.git
          PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_LIST_DIR}/external/findOpenCL.patch
          GIT_TAG master
          TIMEOUT 10
          UPDATE_COMMAND ${GIT_EXECUTABLE} pull
          CONFIGURE_COMMAND cmake .
          BUILD_IN_SOURCE 1
          BUILD_COMMAND make
          INSTALL_COMMAND true
          STEP_TARGETS build
          EXCLUDE_FROM_ALL TRUE
          )
add_library( ViennaCL UNKNOWN IMPORTED )
set_property( TARGET ViennaCL PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/external/viennacl/lib/libviennacl.a )
add_dependencies( ViennaCL ViennaCL_ext )

#../external/src/ViennaCL_ext/viennacl/
#ExternalProject_Get_Property(ViennaCL build_dir)
#message(${build_dir})
include_directories(${CMAKE_CURRENT_LIST_DIR}/external/src/ViennaCL_ext/)
#include_directories(${CMAKE_CURRENT_LIST_DIR}/external/src/ViennaCL_ext/)

add_executable(game game.cpp video.cpp simulation.cpp)
add_dependencies( game ViennaCL )
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLU REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(game ${OPENGL_LIBRARIES} ${GLU_LIBRARY} ${GLEW_LIBRARY} glfw)

