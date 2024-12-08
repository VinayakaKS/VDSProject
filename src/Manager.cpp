#include "Manager.h"

using namespace std;
using namespace ClassProject;

typedef size_t BDD_ID;

BDD_ID Manager::createVar(const std::string &label) {
    std::regex pattern("^!?[A-Za-z]([+*^!][A-Za-z])*$");
    if(!std::regex_match(label, pattern)) {
        throw std::runtime_error("Variable label is not valid. A valid label should contain one or more single alphabets seperated by logical operators (+ * ^ !) or starting with !");
    }

    if(unique_table.getRowByLabel(label)) {
        throw std::runtime_error("Variable label already exists. Please try a new label");
    } else {
        TableRow new_var = { 0 , label};
        return unique_table.addRow(&new_var);
    }
};

bool Manager::isConstant(BDD_ID f) {
    TableRow* tr = unique_table.getRowById(f);
    if(tr) {
        return (f == FALSE_ROW || f == TRUE_ROW) ;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
};

bool Manager::isVariable(BDD_ID x) {
    TableRow* tr = unique_table.getRowById(x); 
    if(tr) {
        return (x != FALSE_ROW && x != TRUE_ROW && tr->high == TRUE_ROW && tr->low == FALSE_ROW && tr->topVar == x) ;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
};

BDD_ID Manager::topVar(BDD_ID f) {
    TableRow* tr = unique_table.getRowById(f); 
    if(tr) {
        return tr->topVar;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
};


BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x){
    TableRow* row;
    if(topVar(f) == x)
    {
        row = unique_table.getRowById(f);
        return row->high;
    }
        
    else return f;

};

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x){
    TableRow* row;
    if(topVar(f) == x)
    {
        row = unique_table.getRowById(f);
        return row->low;
    }
        
    else return f;

};

BDD_ID Manager::coFactorTrue(BDD_ID f){
    return -1;
};

BDD_ID Manager::coFactorFalse(BDD_ID f){
    return -1;
};

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{
    BDD_ID topVariable,topVari,topVart,topVare;
    BDD_ID high, low;
    TableRow new_row_data;

    if(i > return_lastID() || t > return_lastID() || e > return_lastID())
        throw std::runtime_error("Invalid BDD_ID");

    //check for terminal cases
    if(i==1) return t; //ite(1,t,e)
    if(i==0) return e; //ite(0,t,e)
    if(t==e) return t; //ite(i,t,t)
    if(t==1 && e==0) return i; //ite(i,1,0)
    //ite(i,0,1) should return neg(i)

    //determine the top variable
    topVari =  (isVariable(i))? topVar(i): LIMIT;
    topVart =  (isVariable(t))? topVar(t): LIMIT;
    topVare =  (isVariable(e))? topVar(e): LIMIT;

    topVariable = min(topVari,topVart);
    topVariable = min(topVariable,topVare);

    high = ite(coFactorTrue(i,topVariable),coFactorTrue(t,topVariable),coFactorTrue(e,topVariable));
    low = ite(coFactorFalse(i,topVariable),coFactorFalse(t,topVariable),coFactorFalse(e,topVariable));

    if(high == low) return high;
    new_row_data = {0,"funct",high,low,topVariable};
    return unique_table.addRow(&new_row_data); //TODO: check if already present in the table
};
