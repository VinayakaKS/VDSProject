#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
        public:

            Reachability(unsigned int stateSize, unsigned int inputSize) : ReachabilityInterface(stateSize, inputSize) {
                if (stateSize == 0) {
                    throw std::runtime_error("stateSize must be a positive integer");
                } else {
                    for (size_t i = 0; i < stateSize; i++)
                    {
                        States.push_back(manager.createVar("S" + i));
                    }

                    for (size_t i = 0; i < inputSize; i++)
                    {
                        Inputs.push_back(manager.createVar("i" + i));
                    }
                }
                manager.print_table();
            };

            virtual const std::vector<BDD_ID> &getStates();

            virtual const std::vector<BDD_ID> &getInputs();
            
            virtual bool isReachable(const std::vector<bool> &stateVector) ;

            virtual int stateDistance(const std::vector<bool> &stateVector);

            virtual void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions);

            virtual void setInitState(const std::vector<bool> &stateVector);

        private: 
            Manager manager;
            vector<BDD_ID> States;
            vector<BDD_ID> Inputs;
            vector<BDD_ID> TranistionFunctions;
    };

}
#endif
