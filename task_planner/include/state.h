#ifndef STATE_H
#define STATE_H
#include<vector>
#include<iostream>
#include<unordered_map>
class State {
	protected:
		static std::vector<std::vector<std::string>> state_space_named;
		static std::vector<int> num_vars;
		//std::vector<std::string> location_labels;

		// State: index: obj_label_ind, location_ind>
		std::vector<int> state_space;
		static unsigned int state_space_dim;
		static bool is_dimensions_defined;
		//int label2ind(std::string label, const std::vector<std::string>& labels) const;
		struct domain {
			std::string label;
			std::vector<std::string> vars;
		};
		static std::vector<domain> domains;
	public:
		State();
		static void resizeAll(unsigned int size);
		static void resizeAll();
		void initNewSS();
		static const std::string UNDEF;
		static void setStateDimension(const std::vector<std::string>& var_labels, unsigned int dim);
		static int getVarOptionsCount(unsigned int dim);
		static void setDomain(std::string domain_label, std::vector<std::string> vars);
		static void setDomain(std::string domain_label, std::vector<std::string> vars, unsigned int index);
		bool getDomains(std::string var, std::vector<std::string>& in_domains);
		std::string getDomain(std::string var) const;
		virtual void setState(const std::vector<std::string>& set_state);
		virtual void setState(std::string set_state_var, unsigned int dim);
		std::vector<std::string> getState();
		bool isDefined() const;
		void print() const;
		//void setLocationLabels(const std::vector<std::string>& location_labels_);
		//int returnNumLocations() const;
		//std::string returnLocationLabel(int location_label_ind);
		//void setState(std::vector<int> state_);
		//std::vector<int> getState() const;
		//void copyTo(State* copy_state) const;
		//void copyFrom(const State* copy_state);
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
/*
#ifndef OBJSTATE_H
#define OBJSTATE_H
class ManipulatorState : public State {
	private:
		static unsigned int N_obj;
		static std::vector<std::string> obj_labels;
	public: 
		std::string returnEELocationLabel() const;
		int returnEELocation() const;
		bool isGrabbing(std::string eef_flag);
		bool isGrabbing(std::string eef_flag, int& grabbing_obj_ind);
		void setEELocation(std::string location_label);
		void setEELocation(int location_label_ind);
		void setObjLocationToEELocation(int obj_label_ind);
		void setObjLabels(const std::vector<std::string>& obj_labels_);
		std::string returnObjLocation(std::string obj_label) const;
		int returnObjLocation(int obj_label_ind) const;
		void setObjLocation(std::string obj_label, std::string location_label);
		void setObjLocation(int obj_label_ind, int location_label_ind);
		bool isOccupied(std::string location_label) const;
		bool isOccupied(int location_label_ind) const;
		void printState() const;
		bool isEqual(const ManipulatorState* compare_state_ptr) const;
};
#endif
*/
