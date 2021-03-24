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
	conds[0].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup location domain");
	conds[0].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[0].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc",Condition::TRUE, "arg");
	conds[0].setPreCondJunctType(Condition::CONJUNCTION);

	conds[0].addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee",Condition::TRUE, "arg");
	conds[0].addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[0].setPostCondJunctType(Condition::CONJUNCTION);
	conds[0].setActionLabel("grasp");
	//conds[0].print();


	// Move around pickup domain with an object 
	conds[1].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[1].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[1].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[1].addPreCondition(Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg2");
	conds[1].setPreCondJunctType(Condition::CONJUNCTION); // Used to store eeLoc pre-state variable

	conds[1].addPostCondition(Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg2"); // Stored eeLoc pre-state variable is not the same as post-state eeLoc (eeLoc has moved)
	conds[1].addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[1].addPostCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE,"na");
	conds[1].setPostCondJunctType(Condition::CONJUNCTION);
	conds[1].setActionLabel("move");
	//conds[1].print();

	// Release the object that the eef is holding
	conds[2].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup location domain");
	conds[2].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[2].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[2].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::VAR, "ee",Condition::TRUE, "arg2");
	conds[2].setPreCondJunctType(Condition::CONJUNCTION);

	conds[2].addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::TRUE, "arg2");
	conds[2].addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[2].setPostCondJunctType(Condition::CONJUNCTION);
	conds[2].setActionLabel("release");
	//conds[2].print();


	// Move around pickup domain without an object
	conds[3].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[3].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[3].addPreCondition(Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg");
	conds[3].setPreCondJunctType(Condition::CONJUNCTION);

	conds[3].addPostCondition(Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE,"arg");
	conds[3].addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	conds[3].setPostCondJunctType(Condition::CONJUNCTION);
	conds[3].setActionLabel("move");
	//conds[3].print();


	// Translate from pickup domain to drop off domain
	conds[4].addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	conds[4].setPreCondJunctType(Condition::CONJUNCTION);

	conds[4].addPostCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	conds[4].setPostCondJunctType(Condition::CONJUNCTION);
	conds[4].setActionLabel("translate");
	//conds[4].print();


	// Dropoff domain conditions:
	
	// Grasp in dropoff domain
	conds[5].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff location domain");
	conds[5].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[5].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::TRUE, "arg");
	conds[5].setPreCondJunctType(Condition::CONJUNCTION);

	conds[5].addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee", Condition::TRUE, "arg");
	conds[5].addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[5].setPostCondJunctType(Condition::CONJUNCTION);
	conds[5].setActionLabel("grasp");
	//conds[5].print();

	// Move around dropoff domain with an object 
	conds[6].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[6].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[6].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[6].addPreCondition(Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg2");
	conds[6].setPreCondJunctType(Condition::CONJUNCTION); // Used to store eeLoc pre-state variable

	conds[6].addPostCondition(Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg2"); // Stored eeLoc pre-state variable is not the same as post-state eeLoc (eeLoc has moved)
	conds[6].addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[6].addPostCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "na");
	conds[6].setPostCondJunctType(Condition::CONJUNCTION);
	conds[6].setActionLabel("move");
	//conds[6].print();

	// Release the object that the eef is holding
	conds[7].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff location domain");
	conds[7].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	conds[7].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg1");
	conds[7].addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::VAR, "ee", Condition::TRUE, "arg2");
	conds[7].setPreCondJunctType(Condition::CONJUNCTION);

	conds[7].addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::TRUE, "arg2");
	conds[7].addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[7].setPostCondJunctType(Condition::CONJUNCTION);
	conds[7].setActionLabel("release");
	//conds[7].print();
	
	// Move around dropoff domain without an object
	conds[8].addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	conds[8].addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[8].addPreCondition(Condition::LABEL, "eeLoc", Condition::ARG_FIND, Condition::NONE, Condition::FILLER, Condition::TRUE, "arg");
	conds[8].setPreCondJunctType(Condition::CONJUNCTION);

	conds[8].addPostCondition(Condition::ARG_V, Condition::FILLER, Condition::ARG_EQUALS, Condition::LABEL, "eeLoc", Condition::NEGATE, "arg");
	conds[8].addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	conds[8].setPostCondJunctType(Condition::CONJUNCTION);
	conds[8].setActionLabel("move");
	//conds[8].print();


	// Translate from pickup domain to drop off domain
	conds[9].addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	conds[9].setPreCondJunctType(Condition::CONJUNCTION);

	conds[9].addPostCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	conds[9].setPostCondJunctType(Condition::CONJUNCTION);
	conds[9].setActionLabel("translate");
	//conds[9].print();


	
	for (int i=0; i<conds.size(); ++i){
		cond_ptrs[i] = &conds[i];
	}

	Edge TS_graph(true);
	TransitionSystem<BlockingState> TS(&TS_graph);
	TS.setConditions(cond_ptrs);
	TS.setInitState(&init_state);
	TS.generate();
	//TS_graph.print();
	TS.print();
	
	std::cout<<"\nYOOOOGA :"<<std::endl;
	BlockingState teststate;
	teststate.initNewSS();
	teststate.toggleDebug(true);
	teststate.setState("L1", 0);
	teststate.setState("ee", 1);
	teststate.setState("L2", 2);
	teststate.setState("true", 3);
	teststate.print();

	return 0;
}
