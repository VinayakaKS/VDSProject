#include "Reachability.h"

using namespace std;
using namespace ClassProject;


/**
 * Returns a vector containing all state bits of the state machine.
 *
 * @returns vector with the BDD_ID of each state bit
 */
const std::vector<BDD_ID> &Reachability::getStates() {
     return States;
};


/**
 * Returns a vector containing all input bits of the state machine.
 *
 * @returns vector with the BDD_ID of each input bit
 */
const std::vector<BDD_ID> &Reachability::getInputs() {
    return Inputs;
};


/**
 * This function computes whether a specific state is in the reachable state space or not.
 * The input state is provided as a vector of boolean values for each state bit.
 *
 * @param stateVector provides the assignment for each state bit
 * @returns true, if the given state is in the reachable state set
 * @throws std::runtime_error if size does not match with number of state bits
 */
bool Reachability::isReachable(const std::vector<bool> &stateVector) {

};


 /**
 * This function computes the distance from the initial state to a specified state.
 * i.e., minimum cycles it takes the FSM to transition to the specified state
 * Example: Initial state s0 = 0; s1 = 0
 *          Transition functions: s0' = !s1; s1' = s0
 *          FSM transitions:
 *             {False, False}
 *             {True, False}
 *             {True, True}
 *             {False, True}
 *             {False, False}
 *             ...
 *          stateDistance return values:
 *             {False, False} -> 0
 *             {True, False}  -> 1
 *             {True, True}   -> 2
 *             {False, True}  -> 3
 *             {False, False} -> 0
 * @param stateVector provides the assignment for each state bit
 * @return the shortest distance to the initial state, -1 if unreachable
 * @throws std::runtime_error if size does not match with number of state bits
 */
int Reachability::stateDistance(const std::vector<bool> &stateVector) {

};


/**
 * Each state variable has a transition function.
 * The transition function specifies the value of the state after the transition.
 * The transition functions can be composed of state variables and inputs if present.
 * Example: s0' = s0 XOR s1
 * Example: s1' = (s1 AND s0) OR i0
 * The next state for s0 is defined as XOR of the current values of the state bit s0 and s1
 * The next state for s1 is defined as the AND of the current values of s1 and s0 ORed with input i0
 * An exception is thrown, if
 *  - The number of given transition functions does not match the number of state bits
 *  - An unknown ID is provided
 *
 * @param transitionFunctions provide a transition function exactly for each state bit
 * @throws std::runtime_error
 */
void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) {
    if(transitionFunctions.size() != States.size()) {
        throw std::runtime_error("A transition function must be specified for each state!"); 
    } else {
        for (BDD_ID transition : transitionFunctions) {
            if(transition > manager.return_lastID()) {
                throw std::runtime_error("Invalid BDD_ID");
            } else {
                TranistionFunctions.push_back(transition);
            }
        }
    }
};


/**
 * Provides an initial state for the system as a vector of boolean values.
 * If the entry is true, the state bit is high. Otherwise, the bit is low.
 * E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
 *
 * @param stateVector provides the assignment for each state bit
 * @throws std::runtime_error if size does not match with number of state bits
 */
void Reachability::setInitState(const std::vector<bool> &stateVector) {

};