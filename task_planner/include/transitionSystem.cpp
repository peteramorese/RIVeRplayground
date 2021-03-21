#include<string>
#include<vector>
#include<iostream>
#include "edge.h"
#include "transitionSystem.h"
#include "state.h"


TransitionSystem::TransitionSystem () {
	
}

void TransitionSystem::setPropagationState(State& prop_state_){
	prop_state = prop_state_;


}
