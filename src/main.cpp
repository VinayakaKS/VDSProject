//
// Created by Carolina P. Nogueira 2016
//

#include <iostream>
#include <string>
#include "Manager.h"
#include "Reachability.h"
ClassProject::Manager manager;
ClassProject::Reachability fsm2(2);

#define StateSpace 1

using namespace ClassProject;
int main(int argc, char* argv[])
{
    #ifndef StateSpace
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    manager.createVar("d");
    manager.or2(2,3);
    manager.and2(4,5);
    manager.and2(6,7);
    #else

    vector<ClassProject::BDD_ID> stateVars2 = fsm2.getStates();
    vector<ClassProject::BDD_ID> transitionFunctions;

    ClassProject::BDD_ID s0 = stateVars2.at(0);
    ClassProject::BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2.neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2.neg(s1)); // s1' = not(s1)

    fsm2.setTransitionFunctions(transitionFunctions);
    fsm2.setInitState({false,false});
    bool R;

    R = fsm2.isReachable({true, true});
    cout<<"\nReachability {true, true} - "<<R<<endl;
    fsm2.stateDistance({true,true});
    cout<<endl;

    R = fsm2.isReachable({true, false});
    cout<<"Reachability {true, false} - "<<R<<endl;    
    fsm2.stateDistance({true,false});
    cout<<endl;

    R = fsm2.isReachable({false, true});
    cout<<"Reachability {false, true} - "<<R<<endl;
    fsm2.stateDistance({false,false});
    cout<<endl;

    R = fsm2.isReachable({false, false});
    cout<<"Reachability {false, false} - "<<R<<endl;
    fsm2.stateDistance({false,true});
    cout<<endl;


    #endif
}
