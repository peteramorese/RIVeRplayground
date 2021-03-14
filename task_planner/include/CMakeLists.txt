add_library(Edgeclass edge.cpp)
add_library(Astarclass astar.cpp)
target_link_libraries(Astarclass Edgeclass)
