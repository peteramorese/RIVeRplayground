#ifndef STATE_H
#define STATE_H
#include<vector>
#include<iostream>
#include<unordered_map>
class State {
	protected:
		static std::vector<std::string> location_labels;
		//std::vector<std::string> location_labels;

		// State: index: obj_label_ind, location_ind>
		std::vector<int> state_space;
		static bool labels_set;
		int label2ind(std::string label, const std::vector<std::string>& labels) const;
	public:
		static const int UNDEF;
		//const int UNDEF = -1;
		const std::string UNDEF_label = "UNDEF";

		void setLocationLabels(std::vector<std::string> location_labels_);
		bool isDefined() const;
		void setState(std::vector<int> state_);
		std::vector<int> getState() const;
};

#endif

#ifndef EESTATE_H
#define EESTATE_H

class EEState : public State {
	public:
		EEState();
		std::string returnEELocationLabel() const;
		int returnEELocation() const;
		void setEELocation(std::string location_label);
		void setEELocation(int location_label_ind);

};

#endif

#ifndef OBJSTATE_H
#define OBJSTATE_H

class ObjState : public State {
	private:
		const unsigned int N_tuple;
		std::vector<std::string> obj_labels;
	public: 
		ObjState(unsigned int N_tuple_);
		void setObjLabels(std::vector<std::string> obj_labels_);
		std::string returnObjLocation(std::string obj_label) const;
		int returnObjLocation(int obj_label_ind) const;
		void setObjLocation(std::string obj_label, std::string location_label);
		void setObjLocation(int obj_label_ind, int location_label_ind);
		bool isOccupied(std::string location_label) const;
		bool isOccupied(int location_label_ind) const;
		void printState() const;
};
				
#endif
