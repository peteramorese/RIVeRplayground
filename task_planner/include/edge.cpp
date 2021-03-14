#include<string>
#include<vector>
#include<iostream>
#include "edge.h"
#include<map>


Edge::Edge(bool ordered_) : ordered(ordered_) {
	ind = 0;
	ind_checkout = 0;
	head = nullptr;
	prev = nullptr;
	checking = false;

}

bool Edge::isEmpty() {
	if (head==nullptr) {
		return true;
	} else {
		return false;
	}
}

bool Edge::isListEmpty(Edge::edgelist* head_) const {
	if (head_==nullptr) {
		return true;
	} else {
		return false;
	}
}

void Edge::append(unsigned int nodeind_, float weight_) {
	if (isEmpty()) {
		Edge::edgelist* newNode = new Edge::edgelist;
		//std::cout<<"PTR: "<<newNode<<std::endl;
		newNode->nodeind = nodeind_;
		/*
		   if (weight_!=0){
		   std::cout<<"Cannot weight first item in list with edge weight. Setting weight to 0\n";
		   }
		   */
		newNode->weight = 0; 
		head = newNode;
		heads[ind_checkout] = head;
		prev = newNode;
		prevs[ind_checkout] = prev;
		newNode->adjptr = nullptr;
	} else {
		Edge::edgelist* newNode = new Edge::edgelist;
		//std::cout<<"PTR: "<<newNode<<std::endl;
		newNode->nodeind = nodeind_;
		newNode->weight = weight_;

		prev->adjptr = newNode;
		prev = newNode;
		prevs[ind_checkout] = prev;
		newNode->adjptr = nullptr;	
	}
}

void Edge::checkout(int ind_checkout_) {
	if (ind_checkout_<=heads.size()){
		if (isEmpty()) {
			//std::cout<<"im checking out empty"<<std::endl;
			ind_checkout = ind_checkout_;
			//std::cout<<" head ptr:"<<head<<std::endl;
			head = heads[ind_checkout];
			prev = prevs[ind_checkout];	
			checking = true;
		} else {
			if (heads[ind_checkout] == head) {
				//heads[ind_checkout] = head;
				//prevs[ind_checkout] = prev; 
				// reset the pointer keeping track of last node in list using the current checkout
				ind_checkout = ind_checkout_;
				head = heads[ind_checkout];
				prev = prevs[ind_checkout];	
				checking = true;
			} else {
				std::cout<<"Error: Heads are mismatched"<<std::endl;
			}

		}
	} else {
		std::cout << "Index out of bounds for number of lists\n";
	}

}

void Edge::newlist(){
	if (checking) {
		//std::cout<< "checking head:"<<heads[ind_checkout]<< ", reset head:"<<head<<std::endl;
		if (heads[ind_checkout] == head) {
			//heads[ind_checkout] = head;
			//prevs[ind_checkout] = prev; 
			// reset the pointer keeping track of last node in list
			ind_checkout = ind;
			checking = false;
		} else {
			std::cout<<"Error: Heads are mismatched"<<std::endl;
		}
	} else {
		heads.push_back(head);
		prevs.push_back(prev);
		ind = heads.size()-1;
		ind_checkout = ind;
	}
	head = nullptr;
	prev = nullptr;
} 


int Edge::returnListCount() const {
	if (ind+1 == heads.size()){
		return ind+1;
	} else {
		std::cout<<"Error: Number of heads does not match number of lists\n";
	}
}

const std::vector<Edge::edgelist*> Edge::getHeads() const {
	const std::vector<Edge::edgelist*> heads_out = heads;
	return heads_out;

}

void Edge::connect(unsigned int ind_from, unsigned int ind_to, float weight_){
	// Add lists until ind_from and ind_to are included in the graph
	while (ind_from > ind){
		newlist();	
	}
	while (ind_to > ind){
		newlist();
	}
	checkout(ind_from);
	append(ind_to, weight_);
	if (!ordered){
		checkout(ind_to);
		append(ind_from, weight_);
	}

}

std::vector<unsigned int> Edge::returnListNodes(unsigned int ind_) const {
	auto currptr = heads[ind_];
	std::vector<unsigned int> ret_list;
	while (currptr!=nullptr) {
		auto nextptr = currptr->adjptr;
		ret_list.push_back(currptr->nodeind);
		currptr = nextptr;
	}
}

std::vector<float> Edge::returnListWeights(unsigned int ind_) const {
	auto currptr = heads[ind_];
	std::vector<float> ret_list;
	while (currptr!=nullptr) {
		auto nextptr = currptr->adjptr;
		ret_list.push_back(currptr->weight);
		currptr = nextptr;
	}
}

void Edge::print() const {
	for (int i=0; i<heads.size(); i++) {
		auto currptr = heads[i];
		std::cout<<"Node: "<<i<<" connects to:\n";
		while (currptr!=nullptr) {
			auto nextptr = currptr->adjptr;
			std::cout<<"  ~> "<<currptr->nodeind<<"\n";
			currptr = nextptr;
		}
	}

}

int Edge::augmentedStateFunc(int i, int j, int n, int m) const {
	int ret_int;
	ret_int = m*i+j;
	if (ret_int<=n*m){
		return ret_int;
	} else {
		std::cout<<"Error: augmentedStateFunc mapping out of bounds\n";
	}
}

void Edge::compose(const Edge &mult_graph, Edge& product_graph){
	int n = heads.size();
	int m = mult_graph.returnListCount();
	std::cout<<"n ="<<n<<std::endl;
	std::cout<<"m ="<<m<<std::endl;
	auto mult_heads = mult_graph.getHeads();
	int ind_from, ind_to;
	for (int i = 0; i<n; i++){
		for (int j = 0; j<m; j++){
			auto currptr_i = heads[i];	
			auto currptr_j = mult_heads[j];	
			ind_from = augmentedStateFunc(i, j, n, m);
			while (currptr_i!=nullptr){
				int i_to = currptr_i->nodeind;
				while (currptr_j!=nullptr){
					int j_to = currptr_j->nodeind;
					ind_to = augmentedStateFunc(i_to, j_to, n, m);

					// Edge weights on composed graph is just the sum of the
					// corresponding edge weights. This line below can be
					// changed if the composition edge weight operator is
					// defined to be something else
					float prod_weight = currptr_i->weight + currptr_j->weight;
					product_graph.connect(ind_from, ind_to, prod_weight);
					currptr_j = currptr_j->adjptr;
				}
				currptr_i = currptr_i->adjptr;
			}
		}
	}
}


std::pair<unsigned int, unsigned int> Edge::augmentedStateMap(unsigned int ind_product, int n, int m) const {
	std::pair<unsigned int, unsigned int> ret_pair;
	int i = 0;
	int j;
	int a = 0;
	while (m*i<ind_product){
		i++; 
	}
	j = ind_product % m; 	
	ret_pair.first = i;
	ret_pair.second = j;
	return ret_pair;
}

Edge::~Edge() {
	std::cout<< "Deconstructing " << heads.size() << " lists...\n";

	for (int i=0; i<heads.size(); i++) {
		auto currptr = heads[i];
		while (currptr!=nullptr) {
			auto nextptr = currptr->adjptr;
			//std::cout<<"PTR DELETE: "<<currptr<<std::endl;
			delete currptr;
			currptr = nextptr;
		}
	}

}



