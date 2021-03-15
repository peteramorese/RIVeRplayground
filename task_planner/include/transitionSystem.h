#pragma once
#include<string>
#include<vector>
#include<iostream>
#include "edge.h"
#include "state.h"

class TransitionSystem {
	private:
		std::vector<EEState*> e_state_map;	
		std::vector<ObjState*> obj_state_map;	
	public:
		TransitionSystem(unsigned int N_obj_, const std::vector<std::string>& location_labels_, const std::vector<std::string>& obj_labels_);
		void setVInit(EEState* ee_state, ObjState* obj_state);
		void generate(Edge& graph);
}


		
}
