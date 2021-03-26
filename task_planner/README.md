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

## Class Description
###### Edge:
Edge is the main graph data structure used in this library. A graph can be stored as a collection of linked lists that represent edge connectivity to the first element in that list. This provides for simple breadth first search applications (such as Astar/Dijkstra's). Quick list element access can be done by cycling through adjacent pointers. This is detailed in the Public Member Documentation.

###### State:
State holds a private access state space, and members to edit/set/inquire about that state space. There exists a set of static members that can be set without any instantiation. It is advised to use these static members to define necessary properties about your state space before instantiating a state object. These static members allow you to name variables, state space dimensions, and more before ever creating a state. Once these have been set, all state instantiations will share these state space properties.

###### BlockingState (sub-class of State):
BlockingState shares the same global static member properties as State. Blocking state allows the user to set blocking dimensions (indices of the state space) which prevent the same variable from ever existing more than once among all blocking dimensions. For example, you can use a BlockingState if you know that an object's location cannot share the same location as another object (all object locations must be unique at every given state). Using a Blocking state for Transition System generation may save computation time.

###### Condition:
Condition serves as a modular logical condition parser. The user can define "pre" and "post" conditions for a given state transition. The user can then evaluate if two states (one pre-state, one post-state) meet the condition. This functionality is used within Transition System generation for determining possible state transitions. Multiple individual conditions can be conjoined/disjoined in side a pre-condition/post-condition to give the user extensive ability to rigorously define specific conditions. When the condition is evaluated, all sub pre and/or post conditions will be logically joined then evaluated as a whole. If the pre-state fails the evaluation, the Condition will not bother checking the post condition, and will automatically return false. A detailed description of the logical function argument syntax is given in section: "Condition Logical Syntax".

###### SimpleCondition (sub-class of Condition):
SimpleCondition offers similar functionality as Condition, however SimpleCondition allows the user to evaluate the truth of only one state (no pre- and post-conditions/states). When the user instantiates a SimpleCondition, they must also specify that the simple condition type is a simple condition (instead of a pre-condition or post-condition). This is currently redundant because addition of sub-conditions in the SimpleCondition class only supports sub-conditions of type: simple, however keeping this specified may allow for future addtions to possible condition types that are still only evaluated over a single state.

###### Astar:
Astar is simple an implementation of the A-star shortest path graph search algorithm. *Currently, this only supports Dijkstra's algorithm (same as A-star, however Dijkstra's cannot account for a distance heuristic)*. This Class can be used on any graph (see "Edge"). The ProductSystem class (see "ProductSystem") uses this shortest path graph search for discrete task planning.

###### TransitionSystem:
TransitionSystem gives the user the tools to generate a transition system. The user must define a set of conditions (pre- and post- conditions, see "Condition") and an initial state (see "State" or "BlockingState"). Once this has been done, a transition system will be generated on the graph specified in the ctor. The graph only holds node indices, however, during generation, the TransitionSystem class generates a mapping from the state indices appearing in the graph, and pointers to the actual State (object) that is represented by that index. There is a public member "getState()" that allows the user to find the corresponding state pointer from a state index.

###### ProductSystem (sub-class of TransitionSystem):
ProductSystem is a tool for Formal Synthesis that generates a product graph between a TransitionSystem and a Deterministic Automaton, representing a specification. Since this class is derived from TransitionSystem, the user can still generated a Transition System in the same way (see "TransitionSystem"). However, inorder to construct an object of type ProductSystem, the user must also pass in a Deterministic Automaton. It is reccomended that the user fully generate this automaton before instantiation of a ProductSystem object. A set of Atomic Propositions should be defined outside of this object as SimpleConditions. When defining these SimpleConditions, the user must specify a proposition label. Edges on the Deterministic Automaton must be observations that include these proposition labels. A detailed description of the logical form of these observation labels is given in section: "Deterministic Automaton Observation Syntax". Using these SimpleConditions allows the user to reason about the state space, and modularly customize an observation function. Once instantiated, the user must define an initial state on the Transition system (as in "TransitionSystem"), define an initial state on the Deterministic Automaton, add Atomic Proposition Definitions (see "SimpleCondition"), and define a set of accepting state indices on the Deterministic Automaton. Once this has been done, the transition system can be generated, then the TransitionSystem and Deterministic Automaton can be composed into a product graph. This class also incoperates simple discrete planning functionality by allowing the user to "plan()" an action sequence on the automaton that will search for the shortest path on the product graph, returning a "task plan" that represents the most efficient sequence of actions to take a system from the initial transition system state (specified in setup) to a state that is accepting on the Deterministic Automaton. All actions within this plan hold under the Transition System and the specification.

## Condition Logical Syntax

This section will outline the syntax and definitions for defining conditions of type "Condition" or "SimpleCondition".

Function call for adding a condition:
```
void addCondition(COND_TYPE, ARG_1_TYPE, arg_1, OPERATOR, ARG_2_TYPE, arg_2, LOGICAL, condition_label)
```
Either the last argument `condition_label` or the last two arguments `LOGICAL` and `condition_label` can be omitted. If `condition_label` is omitted, the condition will be given label `FILLER`. If both `LOGICAL` and `condition_label` are omitted, the condition will be given logical type `TRUE` and condition label `FILLER`.

**The arguments in capital letters should use the public static variable definitions defined below. The argument in lower-case are `std::string` user inputs that correspond to the respective argument types. An example of this function call is given below:**
```
addCondition(Condition::PRE, Condition::LABEL, "robot_location", Condition::EQUALS, Condition::VAR, "kitchen")
```
This example should read: As a pre-condition, the dimension label: "robot\_location" (state dimension label) must be equal to the variable: "kitchen".


###### COND\_TYPE
- `PRE` specifies the sub-condition should be added to the list of pre-conditions

- `POST` specifies the sub-condition should be added to the list of post-conditions

- `SIMPLE` specifies the sub-condition is a single-state simple condition. This should only be used on SimpleCondition class instantiations

###### ARG\_1\_TYPE
- `LABEL` specifies that the first argument is a state dimension label

- `GROUP` specifies that the first argument is a state dimension label group. Can only be used with operator `ARG_FIND`

- `ARG_L` specifies that the first argument is a state dimension label that was found from calling `ARG_FIND` in a pre-condition. Should only be used in post-condition definition.

- `ARG_V` specifies that the first argument is a state variable that was found from calling `ARG_FIND` in a pre-condition. Should only be used in post-condition definition.

###### OPERATOR

- `EQUALS` argument 1 must be of type: `LABEL` and argument 2 must be of type: `VAR`. Checks to see it the variable in state dimension label `arg_1` is equal to the variable `arg_2`

- `IN_DOMAIN` argument 1 must be of type: `LABEL` and argument 2 must be of type: `DOMAIN`. Check to see if the variable in state dimension label `arg_1` is a member of domain `arg_2`

- `ARG_FIND` argument 1 must be of type: `LABEL` or `GROUP` and argument 2 must be of type `NONE`, `LABEL` or `VAR`. If the first argument is a state dimension label, then `ARG_FIND` will simply store the variable (`ARG_V`) inside the state dimension specified by the first argument. To do this, the second argument must be of type `NONE` as it is not necessary. If the first argument is a state dimension label group, `ARG_FIND` will search through all state dimension labels within the group, and look for a variable. If the second argument is of type `LABEL`, then the searching variable is the variable found in state dimension label `arg_2`. If the second argument is of type `VAR`, the searching variable is simple equal to `arg_2`. If the variable is found in any of the label groups, the condition will return true, and the first state dimension label that the searching variable was found in will be stored in `ARG_L`. **In order to use this operator, a condition label must be specified. This condition label is used to match the corresponding arguments to those passed to **`ARG_EQUALS`.

- `ARG_EQUALS` argument 1 must be of type: `ARG_V` or `ARG_L` and argument 2 must be of type `VAR` or `LABEL`. If the first argument is `ARG_V`, the left-hand matching varible is simply `arg_1`, and if the first argument is `ARG_L`, the left-hand matching variable is whatever variable is inside state dimension label `arg_1`. If the second argument is `VAR`, the right-hand matching variable is simply `arg_2`, and if the second argument is `LABEL`, the right-hand matching variable id whatever variable is inside state dimension label `arg_2`. If the variables match, the condition is satisfied.

###### ARG\_2\_TYPE
Refer to the `OPERATOR` documentation to determine the allowed types and meanings of `ARG_2_TYPE`.

###### LOGICAL
- `TRUE` condition must be true

- `NEGATE` condition must be false. Negate whatever boolean would be returned by evaluating the `TRUE` condition

## Class Public Member Documentation
###### Edge:
- `Edge(bool ordered)` Ctor. Takes in a flag that tells `connect()` to make a bi-directional edge: `ordered = false`, or a regular directional edge `ordered = true`

- `bool isOrdered() const` Returns the flag specified in the ctor

- `bool isListEmpty(edgelist\* head) const` Pass a list head as an argument, returns a logical true if the head has no members in its list

- `void append(unsigned int nodeind_, float weight_, std::string label)` Appends a node on the end of the current working list

- `void checkout(int ind\_checkout)` Switches current working list to that specified by the argument index. This will save any progress on current list

- `void newlist()` Saves current working list, initializes new list

- `int returnListCount() const` Returns the number lists in current instantiation. This number is equal to the length of the `heads` vector, or the number of nodes in the graph

- `const std::vector\<edgelist\*\> getHeads() const` Returns vector of pointers to each list. This should be used for efficient list element access, or Breadth First Search. The array data type is a private member, however `auto` can be used for specifying a return type.

- `void returnListNodes(unsigned int ind_, std::vector\<int\>& node\_list) const` Fills `node_list` with all nodes (indices) that are connected to the node (index) specified by `ind_`

- `void returnListLabels(unsigned int ind_, std::vector\<std::string\>& label\_list) const` Fills `label_list` with all labels of nodes that are connected to the node (index) specified by `ind_`

- `void returnListWeights(unsigned int ind_, std::vector\<float\>& weights\_list) const` Fills `weights_list` with all weights of nodes that are connected to the node (index) specified by `ind_`

- `void connect(unsigned int ind\_from, unsigned int ind\_to, float weight_, std::string label_)` High level graph construction tool (in most cases use this instead of `append()`). Connects a node (index) `ind_from` to node (index) `ind_to` with an edge of weight `weight_` and a string edge label `label`. If either `ind_from` or `ind_to` are larger than the largest index in the graph before calling this member, then `connect()` will automatically generate new lists until the largest index is `ind_from` or `ind_to` (the larger of the two)

- `void print() const` Prints nodes and connections

- `static int augmentedStateFunc(int i, int j, int n, int m)` **OUT OF DATE** Maps index `i` and `j` of two seprate graphs of size `n` and `m` respectively to the index in the graph composition

- `void compose(const Edge &mult\_graph, Edge& product\_graph)` **OUT OF DATE** Creates a composition of two graphs.

- `static void augmentedStateMap(unsigned int ind\_product, int n, int m, std::pair\<unsigned int, unsigned int>& ret\_indices)` Maps index of graph composition `ind_product` back to the two indices of the 
###### State:
TO-DO
###### BlockingState (includes public members of State):
TO-DO
###### Condition:
TO-DO
###### SimpleCondition (includes public members of Condition):
TO-DO
###### Astar:
TO-DO
###### TransitionSystem:
TO-DO
###### ProductSystem (includes public members of TransitionSystem):
TO-DO

