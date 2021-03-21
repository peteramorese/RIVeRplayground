#include<iostream>
#include<vector>
#include "state.h"

const int Condition::LABEL = 0;
const int Condition::VAR = 1;
const int Condition::DOMAIN = 2;
const int Condition::GROUP = 3;
const int Condition::ARG_L = 4;
const int Condition::EQUALS = 5;
const int Condition::IN_DOMAIN = 6;
const int Condition::ARG_EQUALS = 7;
const int Condition::CONJUNCTION = 8;
const int Condition::DISJUNCTION = 9;

Condition::Condition() {
	pr_c.clear();
	ps_c.clear();
}

void Condition::addPreCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_) {
	cond_struct.ARG_1_TYPE = ARG_1_TYPE_;
	cond_struct.arg_1 = arg_1_;
	cond_struct.OPERATOR = OPERATOR;
	cond_struct.ARG_2_TYPE = ARG_2_TYPE_;
	cond_struct.arg_2 = arg_2_  ;
	pr_c.push_back(cond_struct);
}

void Condition::setPreCondJunctType(int LOGICAL_OPERATOR) {
	pre_cond_junct = LOGICAL_OPERATOR;
}

void Condition::addPostCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_) {
	cond_struct.ARG_1_TYPE = ARG_1_TYPE_;
	cond_struct.arg_1 = arg_1_;
	cond_struct.OPERATOR = OPERATOR;
	cond_struct.ARG_2_TYPE = ARG_2_TYPE_;
	cond_struct.arg_2 = arg_2_  ;
	ps_c.push_back(cond_struct);
}

void Condition::setPostCondJunctType(int LOGICAL_OPERATOR) {
	post_cond_junct = LOGICAL_OPERATOR;
}

bool Condition::subEvaluate(const State* state-> const sub_condition& cond) {
	bool eval = false;
	for (int i=0; i<cond.size(); i++) {
		bool sub_eval;
		switch (cond[i].OPERATOR) {
			case EQUALS: 
				if (cond.ARG_2_TYPE == VAR){
					sub_eval = state->getVar(cond.arg_1) == arg_2;
				} else {
					std::cout<<"Error: Condition Syntax error for operator EQUALS\n";
				}
				break;
			case IN_DOMAIN:
				if (cond.ARG_2_TYPE == DOMAIN) {
					sub_eval = false;
					switch (cond[i].ARG_1_TYPE) {
						case LABEL:
							std::vector<std::string> in_domains;
							bool found = state->getDomains(cond.arg_1, in_domains);
							if (found) {
								for (int ii=0; ii<in_domains.size(); ii++) {
									if (in_domains[ii] == cond.arg_2) {
										sub_eval = true;	
										break;
									}
								}
							}
						default:
							std::cout<<"Error: Condition Syntax error for operator IN_DOMAIN\n";
					}
				} else {
					std::cout<<"Error: Condition Syntax error for operator IN_DOMAIN\n";
				}
				break;
			case ARG_EQUALS:
				if (cond.ARG_1_TYPE == GROUP) {
					sub_eval = false;
					switch (cond[i].ARG_2_TYPE) {
						case VAR:
							std::string arg_dimension_label;
							sub_eval = state->argFindGroup(cond.arg_2, cond.arg_1, arg_dimension_label);
							if (sub_eval) {
								arg_L = arg_dimension_label;
							}
						case LABEL:
							std::string arg_dimension_label;
							std::string temp_var;
							temp_var = state->getVar(cond.arg_2);
							sub_eval = state->argFindGroup(temp_var, cond.arg_1, arg_dimension_label);
							if (sub_eval) {
								arg_L = arg_dimension_label;
							}
					}
				} else {
					std::cout<<"Error: Condition Syntax error for operator ARG_EQUALS\n";
				}
				break;
		}
	}	
}


