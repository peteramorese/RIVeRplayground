#include "edge.h"
#include "astar.h"
#include "state.h"
#include "condition.h"
#include "transitionSystem.h"

int main(){			

	// Set up the environment:
	std::vector<std::string> var_labels_1 = {"safep", "safed", "L1", "L2"};
	std::vector<std::string> var_labels_2 = {"ee", "L1", "L2"};
	std::vector<std::string> var_labels_3 = {"true", "false"};
	std::vector<std::string> domain_pickup = {"L1"};
	std::vector<std::string> domain_dropoff = {"L2"};
	//std::vector<std::string> domain_dropoff = {"L3"};
	
	State::setStateDimension(var_labels_1, 0);
	State::setStateDimension(var_labels_2, 1);
	//State::setStateDimension(var_labels_2, 2);
	//State::setStateDimension(var_labels_2, 3);
	State::setStateDimension(var_labels_3, 2);
	State::setDomain("pickup domain", domain_pickup);
	State::setDomain("dropoff domain", domain_dropoff);
	//State::setDomain("dropoff domain", domain_dropoff);
	State::setStateDimensionLabel(0, "eeLoc");
	State::setStateDimensionLabel(1, "obj1Loc");
	//State::setStateDimensionLabel(2, "obj2Loc");
	//State::setStateDimensionLabel(3, "obj3Loc");
	State::setStateDimensionLabel(2, "holding");
	//std::vector<std::string> grouperino = {"obj1Loc", "obj2Loc", "obj3Loc"};
	std::vector<std::string> grouperino = {"obj1Loc"};
	State::setLabelGroup("object locations", grouperino);
	//std::vector<std::string> set_state_i = {"L1", "L1", "L2", "L3", "false"};
	//std::vector<std::string> set_state_f = {"L1", "ee", "L2", "L3", "false"};
	//std::vector<bool> which_blocking = {false, true, true, true, false};
	//BlockingState::setBlockingDim(which_blocking);
	std::vector<bool> which_blocking = {false, true, false};
	BlockingState::setBlockingDim(which_blocking);
	std::cout<<"yeet:"<<std::endl;

	//State statei;
	//State statef;
	BlockingState init_state;
	BlockingState fin_state;
	init_state.initNewSS();
	fin_state.initNewSS();
	std::vector<std::string> set_state_i = {"safep", "L1", "false"};
	std::vector<std::string> set_state_f = {"L1", "ee", "true"};
	init_state.setState(set_state_i);
	fin_state.setState(set_state_f);
	//statei.setState(set_state_i);	
	//statef.setState(set_state_f);	
	//statei.print();
	//statef.print();
	//std::vector<std::string> indoms;
	//bool yeet = State::getDomains("L2", indoms);
	//std::cout<<indoms[0]<<std::endl;

	/* SET CONDITIONS */
	// Pickup domain conditions:
	Condition cond_1; // Grasp in pickup domain
	cond_1.addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	cond_1.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	cond_1.addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc");
	cond_1.setPreCondJunctType(Condition::CONJUNCTION);

	cond_1.addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee");
	cond_1.addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	cond_1.setPostCondJunctType(Condition::CONJUNCTION);
	cond_1.setActionLabel("grasp");
	cond_1.print();


	Condition cond_2; // Move from safe pose to a location to drop object
	cond_2.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	cond_2.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	cond_2.setPreCondJunctType(Condition::CONJUNCTION);

	cond_2.addPostCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE);
	cond_2.addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	cond_2.setPostCondJunctType(Condition::CONJUNCTION);
	cond_2.setActionLabel("move");


	Condition cond_3; // Move from safe pose to a location to pickup an object
	cond_3.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	cond_3.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	cond_3.setPreCondJunctType(Condition::CONJUNCTION);

	cond_3.addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "pickup domain");
	cond_3.setPostCondJunctType(Condition::CONJUNCTION);
	cond_3.setActionLabel("move");


	Condition cond_4; // Translate from pickup domain to drop off domain
	cond_4.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	cond_4.setPreCondJunctType(Condition::CONJUNCTION);

	cond_4.addPostCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	cond_4.setPostCondJunctType(Condition::CONJUNCTION);
	cond_4.setActionLabel("translate");


	// Dropoff domain conditions:
	Condition cond_5; // Grasp in pickup domain
	cond_5.addPreCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	cond_5.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	cond_5.addPreCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc");
	cond_5.setPreCondJunctType(Condition::CONJUNCTION);

	cond_5.addPostCondition(Condition::ARG_L, Condition::FILLER, Condition::ARG_EQUALS, Condition::VAR, "ee");
	cond_5.addPostCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	cond_5.setPostCondJunctType(Condition::CONJUNCTION);
	cond_5.setActionLabel("grasp");
	cond_5.print();


	Condition cond_6; // Move from safe pose to a location to drop object
	cond_6.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	cond_6.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "true");
	cond_6.setPreCondJunctType(Condition::CONJUNCTION);

	cond_6.addPostCondition(Condition::GROUP, "object locations", Condition::ARG_FIND, Condition::LABEL, "eeLoc", Condition::NEGATE);
	cond_6.addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	cond_6.setPostCondJunctType(Condition::CONJUNCTION);
	cond_6.setActionLabel("move");


	Condition cond_7; // Move from safe pose to a location to pickup an object
	cond_7.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	cond_7.addPreCondition(Condition::LABEL, "holding", Condition::EQUALS, Condition::VAR, "false");
	cond_7.setPreCondJunctType(Condition::CONJUNCTION);

	cond_7.addPostCondition(Condition::LABEL, "eeLoc", Condition::IN_DOMAIN, Condition::DOMAIN, "dropoff domain");
	cond_7.setPostCondJunctType(Condition::CONJUNCTION);
	cond_7.setActionLabel("move");


	Condition cond_8; // Translate from pickup domain to drop off domain
	cond_8.addPreCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safed");
	cond_8.setPreCondJunctType(Condition::CONJUNCTION);

	cond_8.addPostCondition(Condition::LABEL, "eeLoc", Condition::EQUALS, Condition::VAR, "safep");
	cond_8.setPostCondJunctType(Condition::CONJUNCTION);
	cond_8.setActionLabel("translate");







	bool yehaw = cond_1.evaluate(&init_state, &fin_state);
	std::cout<<" can i connect? "<<yehaw<<std::endl;

	Edge TS_graph(true);
	TransitionSystem<State> TS(&TS_graph);
	TS.addCondition(&cond_1);
	TS.addCondition(&cond_2);
	TS.addCondition(&cond_3);
	TS.addCondition(&cond_4);
	TS.addCondition(&cond_5);
	TS.addCondition(&cond_6);
	TS.addCondition(&cond_7);
	TS.addCondition(&cond_8);
	TS.setInitState(&init_state);
	TS.generate();
	//TS_graph.print();
	TS.print();

	return 0;
}
