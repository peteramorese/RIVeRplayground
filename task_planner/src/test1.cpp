#include "edge.h"
#include "astar.h"
#include "state.h"
#include "condition.h"
#include "transitionSystem.h"

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
	std::vector<std::string> var_labels_1 = {"safep", "L1", "L2", "L3"};
	std::vector<std::string> var_labels_2 = {"ee", "L1", "L2", "L3"};
	std::vector<std::string> var_labels_3 = {"true", "false"};
	std::vector<std::string> domain_pickup = {"L1", "L2"};
	std::vector<std::string> domain_dropoff = {"L3"};
	
	State::setStateDimension(var_labels_1, 0);
	State::setStateDimension(var_labels_2, 1);
	State::setStateDimension(var_labels_2, 2);
	State::setStateDimension(var_labels_2, 3);
	State::setStateDimension(var_labels_3, 4);
	State::setDomain("pickup domain", domain_pickup);
	State::setDomain("dropoff domain", domain_dropoff);
	State::setStateDimensionLabel(0, "eeLoc");
	State::setStateDimensionLabel(1, "obj1Loc");
	State::setStateDimensionLabel(2, "obj2Loc");
	State::setStateDimensionLabel(3, "obj3Loc");
	State::setStateDimensionLabel(4, "holding");
	std::vector<std::string> grouperino = {"obj1Loc", "obj2Loc", "obj3Loc"};
	State::setLabelGroup("object locations", grouperino);
	std::vector<std::string> set_state_i = {"L1", "L2", "L2", "L3", "false"};
	std::vector<std::string> set_state_f = {"L1", "ee", "L2", "L3", "false"};
	std::vector<bool> which_blocking = {false, true, true, true, false};
	BlockingState::setBlockingDim(which_blocking);

	State statei;
	State statef;
	BlockingState block_state;
	//block_state.initNewSS();
	statei.setState(set_state_i);	
	statef.setState(set_state_f);	
	//statei.print();
	//statef.print();
	std::vector<std::string> indoms;
	bool yeet = State::getDomains("L2", indoms);
	std::cout<<indoms[0]<<std::endl;

	/*
	std::vector<BlockingState> all_states;
	BlockingState::generateAllPossibleStates(all_states);
	for (int i=0; i<all_states.size(); i++) {
		all_states[i].print();
	}
	*/
	
	SimpleCondition p1;
	SimpleCondition p2;
	p1.addCondition(Condition::SIMPLE, Condition::LABEL, "obj1Loc", Condition::EQUALS, Condition::VAR, "L1");
	p1.setCondJunctType(Condition::SIMPLE, Condition::CONJUNCTION);
	p1.setLabel("p1");
	p2.addCondition(Condition::SIMPLE, Condition::LABEL, "obj2Loc", Condition::EQUALS, Condition::VAR, "L2");
	p2.setCondJunctType(Condition::SIMPLE, Condition::CONJUNCTION);
	p2.setLabel("p2");
	//std::cout<<" is  me  troo? "<<p1.evaluate(&statei)<<std::endl;

	Edge test_TS(true);
	ProductSystem<State> testPS(&test_TS);
	testPS.addProposition(&p1);
	testPS.addProposition(&p2);
	bool yeeterino = testPS.parseLabelAndEval("!p1 & p2", &statei);
	std::cout<<" is  me  troo? "<<yeeterino<<std::endl;
	


	

	return 0;
}
