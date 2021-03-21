#include "edge.h"
#include "astar.h"
#include "state.h"
//#include "transitionSystem.h"
int main(){			
	/*
	Edge test_graph(false);
	Edge* test_graph_ptr = &test_graph;
	test_graph.connect(0, 1, 1.0f, 1);
	test_graph.connect(0, 2, 1.0f, 1);
	test_graph.connect(0, 3, 1.0f, 1);
	test_graph.connect(1, 4, 1.0f, 1);
	test_graph.connect(1, 5, 1.0f, 1);
	test_graph.connect(1, 6, 3.0f, 1);
	test_graph.connect(2, 7, 4.0f, 1);
	test_graph.connect(2, 8, 1.0f, 1);
	test_graph.connect(2, 9, 2.0f, 1);
	test_graph.connect(3, 10, 1.0f, 1);
	test_graph.connect(3, 11, 1.0f, 1);
	test_graph.connect(3, 12, 1.0f, 1);
	test_graph.connect(5, 13, 3.0f, 1);
	test_graph.connect(7, 13, 3.0f, 1);
	test_graph.connect(9, 13, 3.0f, 1);
	test_graph.connect(11, 13, 2.0f, 1);

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
	/*

	std::cout<<"isgrab " << ObjectState.isGrabbing("ooga")<<std::endl;
	*/
	std::vector<std::string> var_labels_1 = {"Ooga", "Booga", "Boogie", "Boo"};
	std::vector<std::string> var_labels_2 = {"tall", "small"};
	State::setStateDimension(var_labels_1, 0);
	State::setStateDimension(var_labels_1, 1);
	State::setStateDimension(var_labels_2, 2);
	std::vector<std::string> set_state = {"Ooga", "Boo", "small"};
	State test_state;
	test_state.setState(set_state);
	test_state.print();


	BlockingState block_state;
	//block_state.initNewSS();
	std::vector<std::string> set_state_block = {"Ooga", "Booga", "small"};
	block_state.setState(set_state_block);	
	block_state.setState("Boogie", 1);
	std::vector<std::string> oogas_domain = {"Ooga", "Booga", "Boogie"};
	std::vector<std::string> other_domain = {"Boo", "tall", "small"};
	State::setDomain("the oogas", oogas_domain);
	State::setDomain("the others", other_domain);
	State::setStateDimensionLabel(0, "eeLoc");
	State::setStateDimensionLabel(1, "obj1Loc");
	State::setStateDimensionLabel(1, "obj2Loc");
	std::vector<std::string> grouperino = {"obj1Loc", "obj2Loc"};
	State::setLabelGroup("object locations", grouperino);
	std::vector<std::string> doms;
	bool yeet = block_state.getDomains("tall", doms);
	std::cout<<doms[0]<<std::endl;
	std::string arg;
	bool scoob = block_state.argFindGroup("Boogie", "object locations", arg);
	std::cout<< scoob<< "  " << arg <<std::endl;
	scoob = block_state.argFindGroup("Ooga", "object locations", arg);
	std::cout<< scoob<< "  " << arg <<std::endl;
	/*
	block_state.print();
	BlockingState copied_state;
	copied_state = block_state;
	copied_state.print();
	std::cout<<block_state.getVar("obj1Loc")<<std::endl;
	*/

	return 0;
}
