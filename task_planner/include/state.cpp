#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include "state.h"


/* State DEFINITION */
std::vector<std::vector<std::string>> State::state_space_named;
std::vector<int> State::num_vars;
unsigned int State::state_space_dim;
std::vector<State::domain> State::domains;
const std::string State::UNDEF = "UNDEF";
bool State::is_dimensions_defined = false;

State::State() {
	// Set Static Variables (setStateDimension) before instantiating a State
	initNewSS();
	if (!is_dimensions_defined) {
		std::cout<<"Error: Must define State Dimensions before instantiating a State\n";	
	}
}

void State::resizeAll(unsigned int size) {
	state_space_named.resize(size);
	num_vars.resize(size);
}

void State::resizeAll() {
	state_space_named.resize(state_space_dim);
	num_vars.resize(state_space_dim);
}

void State::initNewSS() {
	state_space.resize(state_space_dim);
}


void State::setStateDimension(const std::vector<std::string>& var_labels, unsigned int dim) {
	is_dimensions_defined = true;
	if (dim+1 > state_space_dim) {
		state_space_dim = dim + 1;
		resizeAll();
	}	
	// The last label for each dimension is automatically set to be undefined
	std::vector<std::string> set_labels = var_labels;
	set_labels.push_back(UNDEF);
	state_space_named[dim] = set_labels;
	num_vars[dim] = set_labels.size();
}

int State::getVarOptionsCount(unsigned int dim) {
	if (dim < state_space_dim){
		return state_space_named[dim].size();
	} else {
		std::cout<<"Error: Index out of bounds\n";
	}
}

void State::setDomain(std::string domain_label, std::vector<std::string> vars){
	bool names_found = true;
	for (int i=0; i<vars.size(); i++){
		bool names_found_i = false;
		for (int ii=0; ii<state_space_dim; ii++){
			for (int iii=0; iii<state_space_named[ii].size(); iii++){
				if (vars[i] == state_space_named[ii][iii]) {
					names_found_i = true;
					break;
				}
			}
			if (names_found_i) {
				break;
			}
		}
		if (!names_found_i){
			names_found = false;
			break;
		}
	}
	if (names_found) {
		domain add_domain;
		add_domain.label = domain_label;
		add_domain.vars = vars;
		domains.push_back(add_domain);
	} else {
		std::cout<<"Error: At least one variable was not recognized in input vector. Make sure to call setStateDimension before setting domains\n";
	}
}

void State::setDomain(std::string domain_label, std::vector<std::string> vars, unsigned int index){
	bool names_found = true;
	for (int i=0; i<vars.size(); i++){
		bool names_found_i = false;
		for (int ii=0; ii<state_space_dim; ii++){
			for (int iii=0; iii<state_space_named[ii].size(); iii++){
				if (vars[i] == state_space_named[ii][iii]) {
					names_found_i = true;
					break;
				}
			}
			if (names_found_i) {
				break;
			}
		}
		if (!names_found_i){
			names_found = false;
			break;
		}
	}
	if (names_found) {
		if (index+1 > domains.size()) {
			domains.resize(index+1);
		}
		domain add_domain;
		add_domain.label = domain_label;
		add_domain.vars = vars;
		domains[index] = add_domain;
	} else {
		std::cout<<"Error: At least one variable was not recognized in input vector. Make sure to call setStateDimension before setting domains\n";
	}
}

bool State::getDomains(std::string var, std::vector<std::string>& in_domains) {
	in_domains.clear();
	bool found = false;
	for (int i=0; i<domains.size(); i++){
		for (int ii=0; ii<domains[i].vars.size(); ii++) {
			if (var == domains[i].vars[i]){
				in_domains.push_back(domains[i].label);
				found = true;
			}
		}
	}
	return found;
}

void State::setState(const std::vector<std::string>& set_state) {
	if (set_state.size() == state_space_dim){
		bool names_found = true;
		for (int i=0; i<state_space_dim; i++){
			bool names_found_i = false;
			for (int ii=0; ii<num_vars[i]; ii++){
				if (state_space_named[i][ii] == set_state[i]){
					names_found_i = true;
					state_space[i] = ii;
				}
			}
			if (!names_found_i) {
				names_found = false;
			}

		}	
		if (!names_found) {
			std::cout<<"Error: Unrecognized label in set state\n";
		}
	} else {
		std::cout<<"Error: Set state must have same dimension as state space\n";
	}
}

void State::setState(std::string set_state_var, unsigned int dim) {
	bool name_found = false;
	if (dim+1 > state_space_dim) {
		std::cout<<"Error: Dimension out of bounds\n";
	} else {
		for (int i=0; i<num_vars[i]; i++){
			if (state_space_named[dim][i] == set_state_var) {
				name_found = true;
				state_space[dim] = i;
			}
		}
	}
}

std::vector<std::string> State::getState() {
	std::vector<std::string> ret_state;
	for (int i=0; i<state_space_dim; i++){
		ret_state[i] = state_space_named[i][state_space[i]];
	}
	return ret_state;
}

bool State::isDefined() const {
	bool ret_bool = true;
	for (int i=0; i<state_space_dim; i++){
		// If the state space index is the last value for the dimension, it was
		// automatically set to be undefined
		if (state_space[i] == num_vars[i]-1) {
			ret_bool = false;
		}	
	}
	return ret_bool;
}

void State::print() const {
	for (int i=0; i<state_space_dim; i++){
		std::cout<<"State variable "<< i << " = " << state_space_named[i][state_space[i]]<< "\n";
	}
}



/* BlockingState DEFINTION */

void BlockingState::setState(const std::vector<std::string>& set_state) {
	if (set_state.size() == state_space_dim){
		bool conflict = false;
		bool names_found = true;
		if (set_state.size() > 1){
			for (int i=0; i<set_state.size()-1; i++) {
				for (int ii=i+1; ii<set_state.size(); ii++) {
					if (set_state[i] == set_state[ii]) {
						std::cout<<"Error: Cannot set Blocking State, duplication location: "<<set_state[i]<<"\n";
						conflict = true;
						goto blocking;
					}
				}
			}
		}

		for (int i=0; i<state_space_dim; i++){
			bool names_found_i = false;
			for (int ii=0; ii<num_vars[i]; ii++){
				if (state_space_named[i][ii] == set_state[i]){
					names_found_i = true;
					state_space[i] = ii;
				}
			}
			if (!names_found_i) {
				names_found = false;
			}

		}	
		if (!names_found) {
			std::cout<<"Error: Unrecognized label in set state\n";
		}
blocking:
		if (conflict) {
			std::cout<<"  Set state will not be set...\n";
		}
	} else {
		std::cout<<"Error: Set state must have same dimension as state space\n";
	}
}

void BlockingState::setState(std::string set_state_var, unsigned int dim) {
	bool name_found = false;
	if (dim+1 > state_space_dim) {
		std::cout<<"Error: Dimension out of bounds\n";
	} else {
		bool conflict = false;
		for (int i=0; i<state_space_dim; i++) {
			if (state_space_named[i][state_space[i]] == set_state_var) {
				std::cout<<"Error: Cannot set Blocking State, duplication location: "<<set_state_var<<"\n";
				conflict = true;
				goto blocking;
			}
		}
		for (int i=0; i<num_vars[i]; i++){
			if (state_space_named[dim][i] == set_state_var) {
				name_found = true;
				state_space[dim] = i;
			}
		}
blocking:
		if (conflict) {
			std::cout<<"  Set state will not be set...\n";
		}

	}
}







/*
   std::vector<std::string> State::location_labels;
   const int State::UNDEF = -1;
   bool State::labels_set;

   int State::label2ind(std::string label, const std::vector<std::string>& labels) const {
   int ind = -1;
   for (int i=0; i<labels.size(); i++){
   if (labels[i] == label){
   ind = i;
   break;
   }
   }
   if (ind != -1){
   return ind;
   } else {
   std::cout<<"Error: label does not match any in labels set\n";
   }
   }

   void State::setLocationLabels(const std::vector<std::string>& location_labels_) {
   location_labels = location_labels_;
   labels_set = true;
   }

   int State::returnNumLocations() const {
   return location_labels.size();
   }

   std::string State::returnLocationLabel(int location_label_ind){
   return location_labels[location_label_ind];
   }

   bool State::isDefined() const {
   bool ret_bool = true;
   for (int i=0; i<state_space.size(); i++){
   if (state_space[i] == UNDEF){
   ret_bool = false;
   break;
   }		
   }
   return ret_bool;
   }

   void State::setState(std::vector<int> state_){
   if (labels_set){
   state_space = state_;
   } else {
   std::cout<<"Error: Set location labels before setting state\n";
   }
   }

   std::vector<int> State::getState() const {
   return state_space;
   }

   void State::copyTo(State* copy_state) const {
   copy_state->setState(state_space);
   }

   void State::copyFrom(const State* copy_state) {
   state_space = copy_state->getState();
   }


// Position 0 in the state space is reserved for the end effector, whereas all
// positions 1, 2, 3,... represent object 0, 1, 2,...

unsigned int ManipulatorState::N_obj;
std::vector<std::string> ManipulatorState::obj_labels;

std::string ManipulatorState::returnEELocationLabel() const {
	return location_labels[state_space[0]];
}

bool ManipulatorState::isGrabbing(std::string eef_flag) {
	bool ret_bool = false;
	for (int i=1; i<state_space.size(); i++){
		if (location_labels[state_space[i]] == eef_flag) {
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}

bool ManipulatorState::isGrabbing(std::string eef_flag, int& grabbing_obj_ind) {
	bool ret_bool = false;
	grabbing_obj_ind = -1;
	for (int i=1; i<state_space.size(); i++){
		if (location_labels[state_space[i]] == eef_flag) {
			grabbing_obj_ind = i - 1;
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}

int ManipulatorState::returnEELocation() const {
	return state_space[0];
}

void ManipulatorState::setEELocation(std::string location_label) {
	int ind = label2ind(location_label, location_labels);
	state_space[0] = ind;
}

void ManipulatorState::setEELocation(int location_label_ind) {
	state_space[0] = location_label_ind;
}

void ManipulatorState::setObjLocationToEELocation(int obj_label_ind) {
	state_space[obj_label_ind] = state_space[0];
}

void ManipulatorState::setObjLabels(const std::vector<std::string>& obj_labels_) {
	N_obj = obj_labels_.size();
	state_space.resize(N_obj+1);	
	obj_labels = obj_labels_;
}


std::string ManipulatorState::returnObjLocation(std::string obj_label) const {
	std::string ret_string;
	int obj_label_ind = label2ind(obj_label, obj_labels);
	if (state_space[obj_label_ind+1] != UNDEF){
		ret_string = location_labels[state_space[obj_label_ind+1]];
	} else {
		ret_string = UNDEF_label;	
	}
	return ret_string;
}

int ManipulatorState::returnObjLocation(int obj_label_ind) const {
	if (obj_label_ind < 0 || obj_label_ind > obj_labels.size()){
		std::cout<<"Error: Obj Label Index out of bounds\n";
	} else {
		if (state_space[obj_label_ind+1] == UNDEF){
			return UNDEF;
		} else {
			return state_space[obj_label_ind+1];
		}
	}
}

void ManipulatorState::setObjLocation(std::string obj_label, std::string location_label){
	int obj_label_ind = label2ind(obj_label, obj_labels);
	int location_label_ind = label2ind(location_label, location_labels);
	state_space[obj_label_ind+1] = location_label_ind;
}

void ManipulatorState::setObjLocation(int obj_label_ind, int location_label_ind){
	state_space[obj_label_ind+1] = location_label_ind;
}

bool ManipulatorState::isOccupied(std::string location_label) const {
	int location_label_ind = label2ind(location_label, location_labels);
	bool ret_bool = false;
	for (int i=1; i<state_space.size(); i++){
		if (state_space[i] == location_label_ind) {
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}

bool ManipulatorState::isOccupied(int location_label_ind) const {
	bool ret_bool = false;
	for (int i=1; i<state_space.size(); i++){
		if (state_space[i] == location_label_ind) {
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}


void ManipulatorState::printState() const {
	std::cout<<"End Effector is in location: "<<location_labels[state_space[0]]<<"\n";
	for (int i=1; i<state_space.size(); i++){
		if (state_space[i]==UNDEF){
			std::cout<<"Object: "<<obj_labels[i-1]<<"   has no location defined\n"; 
		} else {
			std::cout<<"Object: "<<obj_labels[i-1]<<"   is in location: "<<location_labels[state_space[i]]<<"\n";
		}
	}
}

bool ManipulatorState::isEqual(const ManipulatorState* compare_state_ptr) const {
	return (state_space == compare_state_ptr->getState());
}

*/
