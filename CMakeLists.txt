cmake_minimum_required(VERSION 3.8)
project(CG_Labs)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        common++/Linux/MicroGlut.c
        common++/Linux/MicroGlut.h
        common++/GL_utilities.c
        common++/GL_utilities.h
        common++/loadobj.c
        common++/loadobj.h
        common++/LoadTGA.c
        common++/LoadTGA.h
        common++/VectorUtils3.c
        common++/VectorUtils3.h
        common++/zpr.c
        common++/zpr.h
        project/src/entities.cpp
        project/src/entities.hpp
        project/src/entity.cpp
        project/src/entity.hpp
        project/src/game.cpp
        project/src/game.hpp
        project/src/graphics.cpp
        project/src/graphics.hpp
        project/src/helpers.cpp
        project/src/helpers.hpp
        project/src/interface.cpp
        project/src/interface.hpp
        project/src/main.cpp
        project/src/main.hpp
        project/src/random.cpp
        project/src/random.hpp)

add_executable(CG_Labs ${SOURCE_FILES})

set(GL_GLEXT_PROTOTYPES 1)
set(GL3_PROTOTYPES 1)

include_directories(./common++/)
include_directories(./common++/Linux/)
include_directories(/usr/include/GL/)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
