#pragma once
#include<string>
#include<vector>
#include<iostream>
class Edge {
	private:
		struct edgelist {
			//edgelist(int Nobj_) : Nobjs(Nobj_) {};

			int nodeind; // Node index or name
			float weight; // Weight of connecting edge, representing resource function
			int label;
			edgelist* adjptr;
		};
		bool checking, ordered;
		unsigned int ind, ind_checkout;
		edgelist* head;
		edgelist* prev;
		std::vector<edgelist*> prevs;
		std::vector<edgelist*> heads;
		bool isEmpty(); 
		int augmentedStateFunc(int i, int j, int n, int m) const;
	public:
		Edge(bool ordered); 
		bool isListEmpty(edgelist* head) const; 
		void append(unsigned int nodeind_, float weight_, int label); 
		void checkout(int ind_checkout); 
		void newlist();
		int returnListCount() const; 
		const std::vector<edgelist*> getHeads() const;
		std::vector<unsigned int> returnListNodes(unsigned int ind_) const;
		std::vector<float> returnListWeights(unsigned int ind_) const;
		void connect(unsigned int ind_from, unsigned int ind_to, float weight_, int label_);
		virtual void print() const;
		void compose(const Edge &mult_graph, Edge& product_graph);
		std::pair<unsigned int, unsigned int> augmentedStateMap(unsigned int ind_product, int n, int m) const;
		~Edge(); 

};


