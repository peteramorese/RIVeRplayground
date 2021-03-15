#pragma once
#include<string>
#include<vector>
#include<iostream>
#include "edge.h"


TransitionSystem::TransitionSystem(unsigned int N_obj_, const std::vector<std::string>& location_labels_, const std::vector<std::string>& obj_labels_) {

}
		void setVInit(EEState* ee_state, ObjState* obj_state);
		void generate(Edge& graph);
	
