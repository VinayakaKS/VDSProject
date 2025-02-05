#include "Reachability.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace ClassProject;

#define CR_UNCOMPUTED -1

/**
 * Returns a vector containing all state bits of the state machine.
 *
 * @returns vector with the BDD_ID of each state bit
 */
const std::vector<BDD_ID> &Reachability::getStates() const
{
    return States;
};

/**
 * Returns a vector containing all input bits of the state machine.
 *
 * @returns vector with the BDD_ID of each input bit
 */
const std::vector<BDD_ID> &Reachability::getInputs() const
{
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
bool Reachability::isReachable(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != States.size())
    {
        throw std::runtime_error("An initial value must be specified for each state!");
    }
    if (Cr == CR_UNCOMPUTED)
    {
        computeReachableStates();
    }
    BDD_ID states_id = stateFunction(stateVector);
    return Intersection(Cr, states_id);
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
int Reachability::stateDistance(const std::vector<bool> &stateVector)
{
    int distance = 0;

    if (stateVector.size() != States.size())
    {
        throw std::runtime_error("An initial value must be specified for each state!");
    }

    if (Cr == CR_UNCOMPUTED)
    {
        computeReachableStates();
    }

    BDD_ID states_id = stateFunction(stateVector);

    for (auto i : state_space)
    {
        if (Intersection(i, states_id))
        {
            return distance;
        }
        distance++;
    }

    return -1;
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
void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    if (transitionFunctions.size() != States.size())
    {
        throw std::runtime_error("A transition function must be specified for each state!");
    }
    for (BDD_ID transition : transitionFunctions)
    {
        if (transition > return_lastID())
        {
            throw std::runtime_error("Invalid BDD_ID");
        }

        TranistionFunctions.push_back(transition);
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
void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if (stateVector.size() != States.size())
    {
        throw std::runtime_error("An initial value must be specified for each state!");
    }
    initState = stateFunction(stateVector);
};

BDD_ID Reachability::quantifyCr(BDD_ID Crc, bool states, bool next)
{
    BDD_ID quantified = Crc;
    for (BDD_ID input : Inputs)
    {
        quantified = or2(coFactorTrue(quantified, input), coFactorFalse(quantified, input));
    }

    if (states)
    {
        for (BDD_ID st : States)
        {
            quantified = or2(coFactorTrue(quantified, st), coFactorFalse(quantified, st));
        }
    }

    if (next)
    {
        for (BDD_ID nxt : Next_states)
        {
            quantified = or2(coFactorTrue(quantified, nxt), coFactorFalse(quantified, nxt));
        }
    }

    return quantified;
}

void Reachability::computeReachableStates()
{
    TransitionRelation = computeTransitionRelation();
    BDD_ID Crit = initState;
    state_space.push_back(Crit);

    do
    {
        Cr = Crit;
        BDD_ID temp1 = and2(Cr, TransitionRelation);
        BDD_ID img_prime = quantifyCr(temp1, true, false);
        BDD_ID temp = img_prime;
        for (size_t i = 0; i < States.size(); i++)
        {
            temp = and2(xnor2(States.at(i), Next_states.at(i)), temp);
        }
        BDD_ID img = quantifyCr(temp, false, true);
        state_space.push_back(img);
        Crit = or2(Cr, img);
    } while (Cr != Crit);
}

BDD_ID Reachability::computeTransitionRelation()
{
    BDD_ID transitionRelation = xnor2(Next_states.at(0), TranistionFunctions.at(0));
    for (size_t i = 1; i < Next_states.size(); i++)
    {
        transitionRelation = and2(xnor2(Next_states.at(i), TranistionFunctions.at(i)), transitionRelation);
    }
    return transitionRelation;
}

bool Reachability::Intersection(BDD_ID space, BDD_ID state)
{
    return and2(space, state) != False();
}

BDD_ID Reachability::stateFunction(const std::vector<bool> &stateVector)
{
    BDD_ID state_id = stateVector.at(0) ? xnor2(States.at(0), True()) : xnor2(States.at(0), False());
    for (size_t i = 1; i < States.size(); i++)
    {
        state_id = stateVector.at(i) ? and2(xnor2(States.at(i), True()), state_id) : and2(xnor2(States.at(i), False()), state_id);
    }

    return state_id;
}
