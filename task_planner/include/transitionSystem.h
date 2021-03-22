#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<unordered_map>
#include "edge.h"
#include "state.h"
#include "condition.h"

template <class T>
class TransitionSystem {
	private:
		std::vector<Condition*> conditions;
		Edge* graph;
		bool has_conditions;
		bool is_blocking;
		bool has_init_state;
		T* init_state;
		std::vector<T*> state_map;
		std::vector<T> all_states;
		std::vector<bool> state_added;
		unsigned int q_i;
		void safeAddState(T* curr_state_, T* add_state, int add_state_ind, Condition* cond);
	public:
		TransitionSystem(Edge* graph_);
		void addCondition(Condition* condition_);
		void setConditions(std::vector<Condition*> conditions_);
		void setInitState(T* init_state_);
		T* getState(int node_index);
		unsigned int generate();
		void print() const;
};
