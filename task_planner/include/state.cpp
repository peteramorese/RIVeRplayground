#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include "state.h"


/* State CLASS DEFINITION */

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

void State::setLocationLabels(std::vector<std::string> location_labels_) {
	location_labels = location_labels_;
	labels_set = true;
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


/* EEState CLASS DEFINITION */

EEState::EEState() {
	state_space.resize(1);
}

std::string EEState::returnEELocationLabel() const {
	return location_labels[state_space[0]];
}

int EEState::returnEELocation() const {
	return state_space[0];
}

void EEState::setEELocation(std::string location_label) {
	int ind = label2ind(location_label, location_labels);
	state_space[0] = ind;
}

void EEState::setEELocation(int location_label_ind) {
	state_space[0] = location_label_ind;
}

bool EEState::isEqual(const EEState &compare_state) const {
	return (state_space == compare_state.getState());
}

/* ObjState CLASS DEFINITION */

ObjState::ObjState(unsigned int N_tuple_) : N_tuple(N_tuple_) {
	state_space.resize(N_tuple);	
}



void ObjState::setObjLabels(std::vector<std::string> obj_labels_) {
	if (obj_labels_.size() == N_tuple){
		obj_labels = obj_labels_;
	} else {
		std::cout<<"Error: Number of Object Labels must be the same as N_tuple\n";
	}
}


std::string ObjState::returnObjLocation(std::string obj_label) const {
	std::string ret_string;
	int obj_label_ind = label2ind(obj_label, obj_labels);
	if (state_space[obj_label_ind] != UNDEF){
		ret_string = location_labels[state_space[obj_label_ind]];
	} else {
		ret_string = UNDEF_label;	
	}
	return ret_string;
}

int ObjState::returnObjLocation(int obj_label_ind) const {
	if (obj_label_ind < 0 || obj_label_ind > obj_labels.size()){
		std::cout<<"Error: Obj Label Index out of bounds\n";
	} else {
		if (state_space[obj_label_ind] == UNDEF){
			return UNDEF;
		} else {
			return state_space[obj_label_ind];
		}
	}
}

void ObjState::setObjLocation(std::string obj_label, std::string location_label){
	int obj_label_ind = label2ind(obj_label, obj_labels);
	int location_label_ind = label2ind(location_label, location_labels);
	state_space[obj_label_ind] = location_label_ind;
}

void ObjState::setObjLocation(int obj_label_ind, int location_label_ind){
	state_space[obj_label_ind] = location_label_ind;
}

bool ObjState::isOccupied(std::string location_label) const {
	int location_label_ind = label2ind(location_label, location_labels);
	bool ret_bool = false;
	for (auto& loc_ind : state_space){
		if (loc_ind == location_label_ind) {
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}

bool ObjState::isOccupied(int location_label_ind) const {
	bool ret_bool = false;
	for (auto& loc_ind : state_space){
		if (loc_ind == location_label_ind) {
			ret_bool = true;
			break;
		}
	}
	return ret_bool;
}


void ObjState::printState() const {
	for (int i=0; i<state_space.size(); i++){
		if (state_space[i]==UNDEF){
			std::cout<<"Object: "<<obj_labels[i]<<"   has no location defined\n"; 
		} else {
			std::cout<<"Object: "<<obj_labels[i]<<"   is in location: "<<location_labels[state_space[i]]<<"\n";
		}
	}
}

bool ObjState::isEqual(const ObjState &compare_state) const {
	return (state_space == compare_state.getState());
}

