#include "edge.h"
#include "astar.h"
#include "state.h"
int main(){			
	/*
	Edge test_graph(false);
	Edge* test_graph_ptr = &test_graph;
	test_graph.connect(0, 1, 1.0f);
	test_graph.connect(0, 2, 1.0f);
	test_graph.connect(0, 3, 1.0f);
	test_graph.connect(1, 4, 1.0f);
	test_graph.connect(1, 5, 1.0f);
	test_graph.connect(1, 6, 3.0f);
	test_graph.connect(2, 7, 4.0f);
	test_graph.connect(2, 8, 1.0f);
	test_graph.connect(2, 9, 2.0f);
	test_graph.connect(3, 10, 1.0f);
	test_graph.connect(3, 11, 1.0f);
	test_graph.connect(3, 12, 1.0f);
	test_graph.connect(5, 13, 3.0f);
	test_graph.connect(7, 13, 3.0f);
	test_graph.connect(9, 13, 3.0f);
	test_graph.connect(11, 13, 2.0f);

	test_graph.print();
	
	Astar astar;
	astar.setGraph(test_graph_ptr);
	astar.setVInit(0);
	astar.setVGoal(13);
	std::vector<int> path;
	float pathlength;
	bool success = astar.searchDijkstra(path, pathlength);
	for (int i=0; i<path.size(); i++){
		std::cout<<path[i]<<std::endl;
	}
	*/
	/*	
	Edge graph_1(true);
	graph_1.connect(0,1,1.0f);
	Edge graph_2(true);
	graph_2.connect(0,1,1.0f);
	graph_2.connect(1,2,1.0f);
	graph_2.connect(2,0,1.0f);
	graph_2.connect(2,1,1.0f);

	Edge graph_product(true);
	graph_1.compose(graph_2, graph_product);
	graph_product.print();
	*/
	ObjState ObjectState(3);
	std::vector<std::string> hello = {"ooga", "booga", "scoobydoo"};
	std::vector<std::string> objnames = {"thing", "bing", "bong"};
	ObjectState.setLocationLabels(hello);
	ObjectState.setObjLabels(objnames);
	std::vector<int> set_state = {1, 0, State::UNDEF};
	ObjectState.setState(set_state);
	std::cout<< ObjectState.isDefined()<<std::endl;
	ObjectState.printState();
	std::cout<< " is occ" << ObjectState.isOccupied("scoobydoo")<<std::endl;
	std::cout<< " is occ" << ObjectState.isOccupied(2)<<std::endl;
	std::cout<< " is occ" << ObjectState.isOccupied(1)<<std::endl;
	return 0;
}
