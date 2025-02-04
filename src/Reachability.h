#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
        public:

            Reachability(unsigned int stateSize, unsigned int inputSize = 0) : ReachabilityInterface(stateSize, inputSize) {
                if (stateSize == 0) {
                    throw std::runtime_error("stateSize must be a positive integer");
                } else {
                    for (size_t i = 0; i < stateSize; i++)
                    {

                        States.push_back(createVar("S" + to_string(i)));
                    }

                    for (size_t i = 0; i < inputSize; i++)
                    {
                        Inputs.push_back(createVar("I" + to_string(i)));
                    }

                    for (size_t i = 0; i < stateSize; i++)
                    {
                        Next_states.push_back(createVar("Sn" + to_string(i)));
                    }
                }
            };

            virtual const std::vector<BDD_ID> &getStates() const ;

            virtual const std::vector<BDD_ID> &getInputs() const ;
            
            virtual bool isReachable(const std::vector<bool> &stateVector) ;

            virtual int stateDistance(const std::vector<bool> &stateVector);

            virtual void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions);

            virtual void setInitState(const std::vector<bool> &stateVector);

            BDD_ID quantifyCr(BDD_ID Crc , bool states , bool next);

            void computeReachableStates(); 
            BDD_ID computeTransitionRelation();
            BDD_ID getCR(){
                return Cr;
            }

        private:
            vector<BDD_ID> States;
            vector<BDD_ID> Next_states;
            vector<BDD_ID> Inputs;
            vector<BDD_ID> TranistionFunctions;
            vector<BDD_ID> state_space;
            BDD_ID initState ;
            BDD_ID Cr = -1;
            BDD_ID TransitionRelation ;
    };

}
#endif
