cmake_minimum_required(VERSION 3.10)

# set the project name
project(Position)

# Bring the headers into the project
include_directories(headers)
include_directories(matplotlib-cpp-master)

file(GLOB SOURCES "src/*.cpp")

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

find_package(PythonInterp)
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIR})


# add the executable
add_executable(positionDetermination ${SOURCES})

target_link_libraries(positionDetermination ${ARMADILLO_LIBRARIES})
target_link_libraries(positionDetermination ${PYTHON_LIBRARY})
