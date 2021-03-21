#include<string>
#include<vector>
#include<iostream>
#include "transitionSystem.h"
#include "edge.h"
#include "state.h"
#include "condition.h"


TransitionSystem::TransitionSystem () {
	
}

void TransitionSystem::setPropagationState(State& prop_state_){
	prop_state = prop_state_;


}
