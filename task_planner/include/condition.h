#pragma once
#include<vector>
#include "state.h"
#include<string>

class Condition {
	private:
		struct sub_condition {
			bool LOGICAL;
			int ARG_1_TYPE;
			std::string arg_1;
			int OPERATOR;
			int ARG_2_TYPE;
			std::string arg_2;
			std::string condition_label;
		} cond_struct;
		std::vector<sub_condition> pr_c;
		std::vector<sub_condition> ps_c;
		int pre_cond_junct;
		int post_cond_junct;
		std::pair<bool, std::string> arg_L;
		void sub_print(const std::vector<sub_condition>& p_c) const;
		std::string action_label;
	public:	
		static const bool TRUE;
		static const bool NEGATE;
		static const std::string FILLER;
		static const int LABEL;
		static const int VAR;	
		static const int DOMAIN;	
		static const int GROUP;
		static const int ARG_L;	
		static const int EQUALS;	
		static const int IN_DOMAIN;	
		static const int ARG_FIND;	
		static const int ARG_EQUALS;	
		static const int CONJUNCTION;	
		static const int DISJUNCTION;

		Condition();
		void addPreCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_);
		void addPreCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_, bool LOGICAL_);
		void setPreCondJunctType(int LOGICAL_OPERATOR);
		void addPostCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_);
		void addPostCondition(int ARG_1_TYPE_, std::string arg_1_, int OPERATOR_, int ARG_2_TYPE_, std::string arg_2_, bool LOGICAL_);
		void setPostCondJunctType(int LOGICAL_OPERATOR);
		void setActionLabel(std::string action_label_);
		std::string getActionLabel();
		bool subEvaluate(const State* state, const sub_condition& cond);
		bool evaluate(const State* pre_state, const State* post_state);
		void print() const;
};
