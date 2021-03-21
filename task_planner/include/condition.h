#include<vector>
#include "state.h"

class Condition {
	private:
		struct sub_condition {
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
		std::string arg_L;
	public:	
		static const int LABEL;
		static const int VAR;	
		static const int DOMAIN;	
		static const int GROUP;
		static const int ARG_L;	
		static const int EQUALS;	
		static const int IN_DOMAIN;	
		static const int ARG_EQUALS;	
		static const int CONJUNCTION;	
		static const int DISJUNCTION;

		Condition();
		void addPreCondition(int ARG_1_TYPE, std::string arg_1, int OPERATOR, int ARG_2_TYPE, std::string arg_2, int );
		void setPreCondJunctType(int LOGICAL_OPERATOR);
		void addPostCondition(int ARG_1_TYPE, std::string arg_1, int OPERATOR, int ARG_2_TYPE, std::string arg_2, int );
		void setPostCondJunctType(int LOGICAL_OPERATOR);
		subEvaluate(const State* state, const sub_condition& cond);
};
