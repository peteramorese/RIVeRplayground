#include "edge.h"
#include "astar.h"
#include "state.h"
#include "condition.h"
#include "transitionSystem.h"

int main(){			

	// Set up the environment:
	/* ENVIRONMENT: SINGLE OBJECT, TWO DOMAINS (pickup dropoff), ONE LOCATION IN EACH 
	std::vector<std::string> var_labels_1 = {"safep", "safed", "L1", "L2"};
	std::vector<std::string> var_labels_2 = {"ee", "L1", "L2"};
	std::vector<std::string> var_labels_3 = {"true", "false"};
	std::vector<std::string> domain_pickup = {"L1","safep"};
	std::vector<std::string> domain_loc_p = {"L1"};
	std::vector<std::string> domain_dropoff = {"L2","safed"};
	std::vector<std::string> domain_loc_d = {"L2"};
	
	State::setStateDimension(var_labels_1, 0);
	State::setStateDimension(var_labels_2, 1);
	State::setStateDimension(var_labels_3, 2);
	State::setDomain("pickup domain", domain_pickup);
	State::setDomain("pickup location domain", domain_loc_p);
	State::setDomain("dropoff domain", domain_dropoff);
	State::setDomain("dropoff location domain", domain_loc_d);
	State::setStateDimensionLabel(0, "eeLoc");
	State::setStateDimensionLabel(1, "obj1Loc");
	State::setStateDimensionLabel(2, "holding");
	std::vector<std::string> grouperino = {"obj1Loc"};
	State::setLabelGroup("object locations", grouperino);
	std::vector<bool> which_blocking = {false, true, false};
	BlockingState::setBlockingDim(which_blocking);

	BlockingState init_state;
	init_state.initNewSS();
	std::vector<std::string> set_state_i = {"safep", "L1", "false"};
	init_state.setState(set_state_i);
	*/



	/* ENVIRONMENT: TWO OBJECTS, TWO DOMAINS (pickup dropoff), TWO LOCATIONS IN PICKUP
	 * THREE LOCATIONS IN DROP OFF  */
	std::vector<std::string> var_labels_1 = {"safep", "safed", "L1", "L2", "L3", "L4", "L5"};
	std::vector<std::string> var_labels_2 = {"ee", "L1", "L2", "L3", "L4", "L5"};
	std::vector<std::string> var_labels_3 = {"true", "false"};
	std::vector<std::string> domain_pickup = {"L1", "L2", "safep"};
	std::vector<std::string> domain_loc_p = {"L1", "L2"};
	std::vector<std::string> domain_dropoff = {"L3", "L4", "L5", "safed"};
	std::vector<std::string> domain_loc_d = {"L3", "L4", "L5"};
	//std::vector<std::string> domain_dropoff = {"L3"};
	
	State::setStateDimension(var_labels_1, 0); // eef
	State::setStateDimension(var_labels_2, 1); // obj1
	State::setStateDimension(var_labels_2, 2); // obj2
	//State::setStateDimension(var_labels_2, 2);
	//State::setStateDimension(var_labels_2, 3);
	State::setStateDimension(var_labels_3, 3);
	State::setDomain("pickup domain", domain_pickup);
	State::setDomain("pickup location domain", domain_loc_p);
	State::setDomain("dropoff domain", domain_dropoff);
	State::setDomain("dropoff location domain", domain_loc_d);
	//State::setDomain("dropoff domain", domain_dropoff);
	State::setStateDimensionLabel(0, "eeLoc");
	State::setStateDimensionLabel(1, "obj1Loc");
	State::setStateDimensionLabel(2, "obj2Loc");
	//State::setStateDimensionLabel(3, "obj3Loc");
	State::setStateDimensionLabel(3, "holding");
	//std::vector<std::string> grouperino = {"obj1Loc", "obj2Loc", "obj3Loc"};
	std::vector<std::string> grouperino = {"obj1Loc", "obj2Loc"};
	State::setLabelGroup("object locations", grouperino);
	std::vector<bool> which_blocking = {false, true, true, false};
	BlockingState::setBlockingDim(which_blocking);

	BlockingState init_state;
	init_state.initNewSS();
	std::vector<std::string> set_state_i = {"safep", "L1", "L2", "false"};
	init_state.setState(set_state_i);




	/* SET CONDITIONS */
	// Pickup domain conditions:
	std::vector<Condition> conds;
	std::vector<Condition*> cond_ptrs;
	conds.resize(10);
	cond_ptrs.resize(10);

	// Grasp in pickup domain
	conds[0].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup location domain");
	conds[0].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[0].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc",Condition::TRUE, "arg");
	conds[0].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[0].addCondition(Condition::POST, Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee",Condition::TRUE, "arg");
	conds[0].addCondition(Condition::POST, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[0].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[0].setActionLabel("grasp");
	//conds[0].print();


	// Move around pickup domain with an object 
	conds[1].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[1].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[1].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[1].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg2");
	conds[1].setCondJunctType(Condition::PRE, Condition::CONJUNCTION); // Used to store eeLoc pre-state variable

	conds[1].addCondition(Condition::POST, Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg2"); // Stored eeLoc pre-state variable is not the same as post-state eeLoc (eeLoc has moved)
	conds[1].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[1].addCondition(Condition::POST, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE,"na");
	conds[1].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[1].setActionLabel("move");
	//conds[1].print();

	// Release the object that the eef is holding
	conds[2].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup location domain");
	conds[2].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[2].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[2].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::VAR, "ee",Condition::TRUE, "arg2");
	conds[2].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[2].addCondition(Condition::POST, Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::TRUE, "arg2");
	conds[2].addCondition(Condition::POST, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[2].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[2].setActionLabel("release");
	//conds[2].print();


	// Move around pickup domain without an object
	conds[3].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[3].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[3].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg");
	conds[3].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[3].addCondition(Condition::POST, Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE,"arg");
	conds[3].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[3].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[3].setActionLabel("move");
	//conds[3].print();


	// Translate from pickup domain to drop off domain
	conds[4].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	conds[4].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[4].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	conds[4].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[4].setActionLabel("translate");
	//conds[4].print();


	// Dropoff domain conditions:
	
	// Grasp in dropoff domain
	conds[5].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff location domain");
	conds[5].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[5].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::TRUE, "arg");
	conds[5].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[5].addCondition(Condition::POST, Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee", Condition::TRUE, "arg");
	conds[5].addCondition(Condition::POST, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[5].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[5].setActionLabel("grasp");
	//conds[5].print();

	// Move around dropoff domain with an object 
	conds[6].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[6].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[6].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[6].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg2");
	conds[6].setCondJunctType(Condition::PRE, Condition::CONJUNCTION); // Used to store eeLoc pre-state variable

	conds[6].addCondition(Condition::POST, Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg2"); // Stored eeLoc pre-state variable is not the same as post-state eeLoc (eeLoc has moved)
	conds[6].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[6].addCondition(Condition::POST, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "na");
	conds[6].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[6].setActionLabel("move");
	//conds[6].print();

	// Release the object that the eef is holding
	conds[7].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff location domain");
	conds[7].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[7].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[7].addCondition(Condition::PRE, Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::VAR, "ee", Condition::TRUE, "arg2");
	conds[7].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[7].addCondition(Condition::POST, Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::TRUE, "arg2");
	conds[7].addCondition(Condition::POST, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[7].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[7].setActionLabel("release");
	//conds[7].print();
	
	// Move around dropoff domain without an object
	conds[8].addCondition(Condition::PRE, Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[8].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[8].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg");
	conds[8].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[8].addCondition(Condition::POST, Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg");
	conds[8].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[8].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[8].setActionLabel("move");
	//conds[8].print();


	// Translate from pickup domain to drop off domain
	conds[9].addCondition(Condition::PRE, Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	conds[9].setCondJunctType(Condition::PRE, Condition::CONJUNCTION);

	conds[9].addCondition(Condition::POST, Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	conds[9].setCondJunctType(Condition::POST, Condition::CONJUNCTION);
	conds[9].setActionLabel("translate");
	//conds[9].print();

	for (int i=0; i<conds.size(); ++i){
		cond_ptrs[i] = &conds[i];
	}


	/* Propositions */
	SimpleCondition p1;
	p1.addCondition(Condition::SIMPLE, Condition::LABEL, "obj1Loc", Condition::EQUALS, Condition::VAR, "L2");
	//p1.addCondition(Condition::SIMPLE, Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::LABEL, "Condition::NEGATE);
	p1.setCondJunctType(Condition::SIMPLE, Condition::CONJUNCTION);
	p1.setLabel("p1");

	/*
	Edge TS_graph(true);
	TransitionSystem<BlockingState> TS(&TS_graph);
	TS.setConditions(cond_ptrs);
	TS.setInitState(&init_state);
	TS.generate();
	//TS_graph.print();
	TS.print();
	*/
	


	/* DFA */
	Edge TS(true);
	Edge DFA(true);
	Edge PS(true);
	DFA.connect(0, 1, 0.4, "p1");
	DFA.connect(0, 0, 0.4, "!p1");
	//DFA.connect(1, 0, 0.4, "!p1");
	//DFA.connect(1, 1, 0.4, "p1");
	DFA.print();
	std::cout<<"made it out of print phew"<<std::endl;
	
	ProductSystem<BlockingState> prodsys(&TS, &DFA, &PS);
	// Transition system stuff:
	std::cout<<"uno"<<std::endl;
	prodsys.setConditions(cond_ptrs);
	prodsys.setInitState(&init_state);
	std::cout<<"dos"<<std::endl;
	prodsys.generate();
	// Product system stuff:
	prodsys.addProposition(&p1);
	prodsys.setAutomatonInitStateIndex(0);
	prodsys.addAutomatonAcceptingStateIndex(1);
	std::cout<<"tre"<<std::endl;
	prodsys.compose();
	std::cout<<"qua"<<std::endl;
	prodsys.print();
	std::vector<int> plan;
	float pathlength;
	prodsys.plan(plan);
	std::cout<<"Path: ";
	for (int i=0; i<plan.size(); ++i) {
		std::cout<<" -> "<<plan[i];	
	}
	std::cout<<"\n";
	/*
	Edge TS2(true);
	TransitionSystem<BlockingState> transys(&TS2);
	transys.setConditions(cond_ptrs);
	transys.setInitState(&init_state);
	transys.generate();
	std::cout<<"\n --- \n"<<std::endl;
	transys.print();

	Astar planner;
	std::cout<<"PS size:"<<PS.returnListCount()<<std::endl;
	planner.setGraph(&PS);
	planner.setVInit(0);
	planner.setVGoal(8);
	float pathlength;
	planner.searchDijkstra(path, pathlength);
	
	*/

	return 0;
}
