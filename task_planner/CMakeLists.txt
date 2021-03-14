cmake_minimum_required(VERSION 3.10.2)

project(rrt_imp)

add_executable(${PROJECT_NAME} src/test1.cpp)
add_subdirectory(include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
link_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} Edgeclass Astarclass)

