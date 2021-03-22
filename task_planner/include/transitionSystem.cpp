#include<string>
#include<vector>
#include<iostream>
#include "transitionSystem.h"
#include "edge.h"
#include "state.h"
#include "condition.h"


template <class T>
TransitionSystem<T>::TransitionSystem (Edge* graph_) : graph(graph_), has_conditions(false) {
	conditions.clear();
	T::generateAllPossibleStates(all_states);
	state_added.resize(all_states.size());
	for (int i=0; i<state_added.size(); ++i) {
		state_added[i] = false;
	}
	for (int i=0; i<all_states.size(); i++) {
		std::cout<<" --- "<<std::endl;
		all_states[i].print();
		std::cout<<" --- "<<std::endl;
	}
}

template <class T>
void TransitionSystem<T>::addCondition(Condition* condition_){
	conditions.push_back(condition_);
	has_conditions = true;
}

template <class T>
void TransitionSystem<T>::setConditions(std::vector<Condition*> conditions_) {
	conditions = conditions_;
	has_conditions = true;
}

template <class T>
void TransitionSystem<T>::setInitState(T* init_state_) {
	init_state = init_state_;
	has_init_state = true;
}

template <class T>
void TransitionSystem<T>::safeAddState(T* curr_state_, T* add_state, int add_state_ind, Condition* cond){
	std::string action = cond->getActionLabel();
	if (!state_added[add_state_ind]) {
		state_map.push_back(add_state);
		state_added[add_state_ind] = true;
		unsigned int new_ind = state_map.size()-1;
		graph->Edge::connect(q_i, new_ind, 1.0f, action);
	} else {
		for (int i=0; i<state_map.size(); ++i) {
			if (add_state == state_map[i]) {
				graph->Edge::connect(q_i, i, 1.0f, action);				
			}
		}
	}
}

template <class T>
T* TransitionSystem<T>::getState(int node_index) {
	return state_map[node_index];
}

template <class T>
unsigned int TransitionSystem<T>::generate() {
	if (has_init_state && has_conditions) {
	int state_count = all_states.size();
	int cond_count = conditions.size();
	/*
	   for (int i=0; i<all_states.size(); i++) {
	   all_states[i].print();
	   }
	   */
	T* init_state_in_set;
	for (int i=0; i<all_states.size(); ++i) {
		if (all_states[i] == init_state) {
			init_state_in_set = &all_states[i];
		}	
	}

	state_map.clear();
	state_map.push_back(init_state_in_set);
	q_i = 0; // State index for current state
	while (q_i<state_map.size()) {
		T* curr_state = state_map[q_i];
		for (unsigned int i=0; i<state_count; ++i) {
			T* new_state = &all_states[i];
			if (!(new_state == curr_state)) {
				for (int ii=0; ii<cond_count; ++ii) {
					bool satisfied;
					satisfied = conditions[ii]->evaluate(curr_state, new_state);
					if (satisfied) {
						safeAddState(curr_state, new_state, i, conditions[ii]);
					}
				}
			}	
		}
		q_i++;
	}
	} else {
		std::cout<<"Error: Must set init state and conditions before calling generate()\n";
	}
}

template <class T>
void TransitionSystem<T>::print() const {
	if (state_map.size() > 1) {
	for (int i=0; i<state_map.size(); ++i) {
		T* curr_state = state_map[i];
		std::vector<int> list_nodes; 
		std::vector<std::string> list_actions; 
		graph->returnListNodes(i, list_nodes);
		graph->returnListLabels(i, list_actions);
		std::cout<<"State "<<i<<": ";
		std::vector<std::string> state_i; 
		curr_state->getState(state_i);
		for (int ii=0; ii<state_i.size(); ++ii) {
			std::cout<<state_i[ii]<<", ";
		}
		std::cout<<"connects to:\n";
		for (int ii=0; ii<list_nodes.size(); ++ii) {
			T* con_state = state_map[list_nodes[ii]];
			std::cout<<"   ~>State "<<list_nodes[ii]<<": ";
			con_state->getState(state_i);
			for (int iii=0; iii<state_i.size(); ++iii) {
				std::cout<<state_i[iii]<<", ";
			}
			std::cout<<" with action: "<<list_actions[ii]<<"\n";
		}
	}
	} else {
		std::cout<<"Warning: Transition has not been generated, or has failed to generate. Cannot print\n";
	}
}


template class TransitionSystem<State>;
template class TransitionSystem<BlockingState>;
