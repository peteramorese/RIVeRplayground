#include<iostream>
#include<vector>
#include "state.h"
#include "condition.h"

const std::string Condition::FILLER = "FILLER";
const int Condition::LABEL = 0;
const int Condition::VAR = 1;
const int Condition::DOMAIN = 2;
const int Condition::GROUP = 3;
const int Condition::ARG_L = 4;
const int Condition::EQUALS = 5;
const int Condition::IN_DOMAIN = 6;
const int Condition::ARG_FIND = 7;
const int Condition::ARG_EQUALS = 8;
const int Condition::CONJUNCTION = 9;
const int Condition::DISJUNCTION = 10;

Condition::Condition() {
	pr_c.clear();
	ps_c.clear();
}

void Condition::addPreCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_) {
	cond_struct.ARG_1_TYPE = ARG_1_TYPE_;
	cond_struct.arg_1 = arg_1_;
	cond_struct.OPERATOR = OPERATOR_;
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
	cond_struct.OPERATOR = OPERATOR_;
	cond_struct.ARG_2_TYPE = ARG_2_TYPE_;
	cond_struct.arg_2 = arg_2_  ;
	ps_c.push_back(cond_struct);
}

void Condition::setPostCondJunctType(int LOGICAL_OPERATOR) {
	post_cond_junct = LOGICAL_OPERATOR;
}

bool Condition::subEvaluate(const State* state, const sub_condition& cond) {
	bool sub_eval = false;

	switch (cond.OPERATOR) {
		case EQUALS: 
			if (cond.ARG_2_TYPE == VAR){
				sub_eval = state->getVar(cond.arg_1) == cond.arg_2;
			} else {
				std::cout<<"Error: Condition Syntax error for operator EQUALS\n";
			}
			break;
		case IN_DOMAIN:
			if (cond.ARG_2_TYPE == DOMAIN) {
				sub_eval = false;
				bool found;
				std::vector<std::string> in_domains;
				std::string dom_var;
				switch (cond.ARG_1_TYPE) {
					case LABEL:
						dom_var = state->getVar(cond.arg_1);
						found = state->getDomains(dom_var, in_domains);
						if (found) {
							for (int ii=0; ii<in_domains.size(); ii++) {
								if (in_domains[ii] == cond.arg_2) {
									sub_eval = true;	
									break;
								}
							}
						}
						break;
					default:
						std::cout<<"Error: Condition Syntax error for operator IN_DOMAIN\n";
				}
			} else {
				std::cout<<"Error: Condition Syntax error for operator IN_DOMAIN\n";
			}
			break;
		case ARG_FIND:
			if (cond.ARG_1_TYPE == GROUP) {
				arg_L.first = false;
				sub_eval = false;
				std::string arg_dimension_label;
				std::string temp_var;
				switch (cond.ARG_2_TYPE) {
					case VAR:
						sub_eval = state->argFindGroup(cond.arg_2, cond.arg_1, arg_dimension_label);
						if (sub_eval) {
							arg_L.first = true;
							arg_L.second = arg_dimension_label;
						}
						break;
					case LABEL:
						temp_var = state->getVar(cond.arg_2);
						sub_eval = state->argFindGroup(temp_var, cond.arg_1, arg_dimension_label);
						if (sub_eval) {
							arg_L.first = true;
							arg_L.second = arg_dimension_label;
						}
						break;
					default:
						std::cout<<"Error: Condition Syntax error for operator ARG_FIND\n";
				}
			} else {
				std::cout<<"Error: Condition Syntax error for operator ARG_FIND\n";
			}
			break;
		case ARG_EQUALS:
			if (cond.ARG_1_TYPE == ARG_L && cond.ARG_2_TYPE == VAR) {
				if (arg_L.first) {
					sub_eval = state->getVar(arg_L.second) == cond.arg_2;
				} else {
					std::cout<<"Error: Must call ARG_FIND before call to ARG_EQUALS\n";
				}
			} else {
				std::cout<<"Error: Condition Syntax error for operator ARG_EQUALS\n";
			}
			break;
		default:
			std::cout<<"Error: Condition Syntax error for operator\n";
	}
	return sub_eval;
}	

bool Condition::evaluate(const State* pre_state, const State* post_state) {
	bool eval = false;
	bool pre_eval;
	switch (pre_cond_junct) {
		case CONJUNCTION:
			pre_eval = true;
			break;
		case DISJUNCTION:
			pre_eval = false;
			break;
	}
	for (int i=0; i<pr_c.size(); i++){
		bool pre_eval_i = subEvaluate(pre_state, pr_c[i]);
		switch (pre_cond_junct) {
			case CONJUNCTION:
				pre_eval = pre_eval && pre_eval_i;
				if (pre_eval) {
					break;
				} else {
					goto postcondition;
				}
				break;
			case DISJUNCTION:
				pre_eval = pre_eval || pre_eval_i;
				if (pre_eval) {
					goto postcondition;
				} else {
					break;
				}
				break;
		}	
	}
postcondition:
	if (pre_eval) {
		bool post_eval;
		bool eq_eval;
		std::vector<std::string> excl_dim_labels;
		switch (post_cond_junct) {
			case CONJUNCTION:
				post_eval = true;
				break;
			case DISJUNCTION:
				post_eval = false;
				break;
		}
		for (int i=0; i<ps_c.size(); i++){
			bool post_eval_i = subEvaluate(post_state, ps_c[i]);
			switch (ps_c[i].ARG_1_TYPE) {
				case LABEL:
					std::cout<<"excl vec pushing back: "<<ps_c[i].arg_1<<std::endl;
					excl_dim_labels.push_back(ps_c[i].arg_1);
					break;
				case ARG_L:
					if (arg_L.first) {
						std::cout<<"excl vec pushing back: "<<arg_L.second<<std::endl;
						excl_dim_labels.push_back(arg_L.second);
					} else {
						std::cout<<"Error: Argument not set\n";
					}
					break;
				
				}
			switch (post_cond_junct) {
				case CONJUNCTION:
					post_eval = post_eval && post_eval_i;
					if (post_eval) {
						break;
					} else {
						goto returncondition;
					}
					break;
				case DISJUNCTION:
					post_eval = post_eval || post_eval_i;
					if (post_eval) {
						goto returncondition;
					} else {
						break;
					}
					break;
			}	
		}
returncondition:
		eq_eval = pre_state->exclEquals(post_state, excl_dim_labels);
		std::cout<<"eq_eval: "<<eq_eval<<std::endl;
		std::cout<<"pre_eval: "<<pre_eval<<std::endl;
		std::cout<<"post_eval: "<<post_eval<<std::endl;
		// pre_eval: Are the preconditions satisfied?
		// post_eval: Are the post conditions satisfied?
		// eq_eval: Are the other unmentioned dimensions still equal between states?
		// All of these must be true for the condition to be satisfied
		eval = pre_eval && post_eval && eq_eval;
		return eval;
	} else {
		return false;
	}
}

void Condition::sub_print(const std::vector<sub_condition>& p_c) const {
	for (int i=0; i<p_c.size(); i++){
		std::cout<<"   -"<<i+1<<") ";
		switch (p_c[i].ARG_1_TYPE) {
			case LABEL: 
				std::cout<<"Dimension Label '"<<p_c[i].arg_1<<"' ";
				break;
			case GROUP:
				std::cout<<"Dimension Group '"<<p_c[i].arg_1<<"' ";
				break;
			case ARG_L:
				std::cout<<"Found Argument Label ";
				break;
		}
		switch (p_c[i].OPERATOR) {
			case EQUALS:
				std::cout<<"is equal to ";
				break;
			case IN_DOMAIN:
				std::cout<<"is in ";
				break;
			case ARG_FIND:
				std::cout<<"found ";
				break;
			case ARG_EQUALS:
				std::cout<<"is equal to ";
		}
		switch (p_c[i].ARG_2_TYPE) {
			case LABEL:
				std::cout<<"Dimension Label: '"<<p_c[i].arg_2<<"'\n";
				break;
			case VAR:
				std::cout<<"Dimension Variable: '"<<p_c[i].arg_2<<"'\n";
				break;
			case DOMAIN:
				std::cout<<"Domain: '"<<p_c[i].arg_2<<"'\n";
				break;
		}
	}	
}

void Condition::print() const {
	std::cout<<"Pre-Conditions ";
	switch (pre_cond_junct) {
		case CONJUNCTION:
			std::cout<<"(of type conjunction):\n";
			break;
		case DISJUNCTION:
			std::cout<<"(of type disjunction):\n";
			break;
	}
	sub_print(pr_c);
	std::cout<<"Post-Conditions ";
	switch (post_cond_junct) {
		case CONJUNCTION:
			std::cout<<"(of type conjunction):\n";
			break;
		case DISJUNCTION:
			std::cout<<"(of type disjunction):\n";
			break;
	}
	sub_print(ps_c);
}
