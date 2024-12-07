#include "Manager.h"

using namespace std;
using namespace ClassProject;

typedef size_t BDD_ID;

BDD_ID Manager::createVar(const std::string &label) {
    return -1;
};


BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{
    // BDD_ID topVar;
    // BDD_ID high, low;
    // TableRow new_row_data;

    // //check for terminal cases
    // if(i==1) return t; //ite(1,t,e)
    // if(i==0) return e; //ite(0,t,e)
    // if(t==e) return t; //ite(i,t,t)
    // if(t==1 && e==0) return i; //ite(i,1,0)
    // //ite(i,0,1) should return neg(i)

    // //determine the top variable
    // topVar = min(manager.topVar(i),manager.topVar(t),manager.topVar(e));
    // high = manager.ite(manager.coFactorTrue(i,topVar),manager.coFactorTrue(t,topVar),manager.coFactorTrue(e,topVar));
    // low = manager.ite(manager.coFactorFalse(i,topVar),manager.coFactorFalse(t,topVar),manager.coFactorFalse(e,topVar));

    // if(high == low) return high;
    // new_row_data = {topVar,"funct",high,low};
    // return manager.unique_table.addRow(&new_row_data); //TODO: check if already present  

        return -1;
};