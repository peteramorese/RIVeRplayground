#ifndef STATE_H
#define STATE_H
#include<vector>
#include<iostream>
#include<unordered_map>
class State {
	protected:
		static std::vector<std::vector<std::string>> state_space_named;
		static std::unordered_map<std::string, unsigned int> index_labels;
		static std::vector<int> num_vars;
		std::vector<int> state_space;
		static unsigned int state_space_dim;
		static bool is_dimensions_defined;
		struct domain {
			std::string label;
			std::vector<std::string> vars;
		};
		static std::vector<domain> domains;
		static std::vector<domain> groups;
	public:
		State();
		static void resizeAll(unsigned int size);
		static void resizeAll();
		void initNewSS();
		static const std::string UNDEF;
		static void setStateDimension(const std::vector<std::string>& var_labels, unsigned int dim);
		static int getVarOptionsCount(unsigned int dim);
		static void setStateDimensionLabel(unsigned int dim, std::string dimension_label);
		static void setDomain(std::string domain_label, std::vector<std::string> vars);
		static void setDomain(std::string domain_label, std::vector<std::string> vars, unsigned int index);
		static bool getDomains(std::string var, std::vector<std::string>& in_domains);
		static void setLabelGroup(std::string group_label, std::vector<std::string> dimension_labels);
		static void setLabelGroup(std::string group_label, std::vector<std::string> dimension_labels, unsigned int index);
		bool argFindGroup(std::string var_find, std::string group_label, std::string& arg_dimension_label); 
		virtual void setState(const std::vector<std::string>& set_state);
		virtual void setState(std::string set_state_var, unsigned int dim);
		std::vector<std::string> getState();
		std::string getVar(std::string dimension_label);
		bool isDefined() const;
		void print() const;
		void operator= (const State& state_eq);
};

#endif

#ifndef BLOCKINGSTATE_H
#define BLOCKINGSTATE_H

class BlockingState : public State {
	public:
		void setState(const std::vector<std::string>& set_state);
		void setState(std::string set_state_var, unsigned int dim);
};

#endif
