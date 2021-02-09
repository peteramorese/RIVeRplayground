cmake_minimum_required(VERSION 3.10.2)

project(orientationDetermination)

add_executable(${PROJECT_NAME} src/orDetMain.cpp)
add_subdirectory(include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
link_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} oriDetermineClass matrixClass)

