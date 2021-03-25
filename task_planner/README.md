# Task Planning Library
This library contains various tools for setting up a planning environment for discrete task planning.

###### Installation
In the root directory:
```
mkdir build
cd build
cmake ./..
make
```

## Class Public Member Documentation
###### Edge:
-`Edge(bool ordered)` Ctor. Takes in a flag that tells `connect()` to make a bi-directional edge: `ordered = false`, or a regular directional edge `ordered = true`
-`bool isOrdered() const` Returns the flag specified in the ctor
-`bool isListEmpty(edgelist\* head) const` Pass a list head as an argument, returns a logical true if the head has no members in its list
-`void append(unsigned int nodeind_, float weight_, std::string label)` Appends a node on the end of the current working list
-`void checkout(int ind\_checkout)` Switches current working list to that specified by the argument index. This will save any progress on current list
-`void newlist()` Saves current working list, initializes new list
-`int returnListCount() const` Returns the number lists in current instantiation. This number is equal to the length of the `heads` vector, or the number of nodes in the graph
-`const std::vector\<edgelist\*\> getHeads() const` Returns vector of pointers to each list. This should be used for efficient list element access, or Breadth First Search. The array data type is a private member, however `auto` can be used for specifying a return type.
-`void returnListNodes(unsigned int ind_, std::vector\<int\>& node\_list) const` Fills `node_list` with all nodes (indices) that are connected to the node (index) specified by `ind_`
-`void returnListLabels(unsigned int ind_, std::vector\<std::string\>& label\_list) const` Fills `label_list` with all labels of nodes that are connected to the node (index) specified by `ind_`
-`void returnListWeights(unsigned int ind_, std::vector\<float\>& weights\_list) const` Fills `weights_list` with all weights of nodes that are connected to the node (index) specified by `ind_`
-`void connect(unsigned int ind\_from, unsigned int ind\_to, float weight_, std::string label_)` High level graph construction tool (in most cases use this instead of `append()`). Connects a node (index) `ind_from` to node (index) `ind_to` with an edge of weight `weight_` and a string edge label `label`. If either `ind_from` or `ind_to` are larger than the largest index in the graph before calling this member, then `connect()` will automatically generate new lists until the largest index is `ind_from` or `ind_to` (the larger of the two)
-`void print() const` Prints nodes and connections
-`static int augmentedStateFunc(int i, int j, int n, int m)` **OUT OF DATE** Maps index `i` and `j` of two seprate graphs of size `n` and `m` respectively to the index in the graph composition
-`void compose(const Edge &mult\_graph, Edge& product\_graph)` **OUT OF DATE** Creates a composition of two graphs.
-`static void augmentedStateMap(unsigned int ind\_product, int n, int m, std::pair\<unsigned int, unsigned int>& ret\_indices)` Maps index of graph composition `ind_product` back to the two indices of the 


