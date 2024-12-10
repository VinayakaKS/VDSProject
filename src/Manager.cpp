#include "Manager.h"

using namespace std;
using namespace ClassProject;
#include <fstream>
#include <iostream>
#include <string>

typedef size_t BDD_ID;
string label_storage;

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
}

/**
 * Checks if the passed on Node is a constant which are True node and False node
 * 
 * @param   BDD_ID of the node to be checked
 * @return  true : if a constant
 *          false : if not a constant 
 */
bool Manager::isConstant(BDD_ID f) {
    TableRow* tr = unique_table.getRowById(f);
    if(tr) {
        return (f == FALSE_ROW || f == TRUE_ROW) ;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Checks if the passed on Node is a variable which will be pointing to a constant
 * 
 * @param   BDD_ID of the node to be checked
 * @return  true : if a variable
 *          false : if not a variable 
 */
bool Manager::isVariable(BDD_ID x) {
    TableRow* tr = unique_table.getRowById(x); 
    if(tr) {
        return (x != FALSE_ROW && x != TRUE_ROW && tr->high == TRUE_ROW && tr->low == FALSE_ROW && tr->topVar == x) ;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Returns the ID of the top variable of the passed node
 */
BDD_ID Manager::topVar(BDD_ID f) {
    TableRow* tr = unique_table.getRowById(f); 
    if(tr) {
        return tr->topVar;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Returns the true cofactor of the node with respect to the variable passed
 * 
 * @param   f : BDD_ID of the node to be checked
 *          x : variable with respect to
 * @return  ID : returns the true - cofactor of the function
 */
BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x){
    if(f > return_lastID() || x > return_lastID())
        throw std::runtime_error("Invalid BDD_ID - True cofactor doesnt exist");
    if(!isVariable(x))
        throw runtime_error("Not a variable - True cofactor doesnt exist");
        
    TableRow* row;
    if(topVar(f) == x)
    {
        row = unique_table.getRowById(f);
        return row->high;
    }
        
    else return f;//vfo ite(x,f,0)
}

/**
 * Returns the false cofactor of the node with respect to the variable passed
 * 
 * @param   f : BDD_ID of the node to be checked
 *          x : variable with respect to
 * @return  ID : returns the false - cofactor of the function
 */
BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x){
    if(f > return_lastID() || x > return_lastID())
        throw std::runtime_error("Invalid BDD_ID - False cofactor doesnt exist");

    if(!isVariable(x))
        throw runtime_error("Not a variable - True cofactor doesnt exist");

    TableRow* row;
    if(topVar(f) == x)
    {
        row = unique_table.getRowById(f);
        return row->low;
    }
        
    else return f;//v0f ite(x,0,f)
}

/**
 * Returns the true co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorTrue(BDD_ID f){
    if(f > return_lastID())
        throw std::runtime_error("Invalid BDD_ID");
        
    return unique_table.getRowById(f)->high;
}

/**
 * Returns the false co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorFalse(BDD_ID f){
    if(f > return_lastID())
        throw std::runtime_error("Invalid BDD_ID");

    return unique_table.getRowById(f)->low;
}

/**
 * Performs an If Then Else(ITE) : [i*t+~i*e] operation on the given nodes
 * Function implementaion in accordance with the Prof. Kunz lectures
 * 
 * @param   i,t,e : BDD_ID of nodes 
 * @return  returns the ID of the resulting row
 */
BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)  
{
    BDD_ID topVariable,topVari,topVart,topVare;
    BDD_ID high, low;
    TableRow new_row_data;
    TableRow *check_row_data;
    string* temp_label = &label_storage;

    /*Invalid BDD_ID exception*/
    if(i > return_lastID() || t > return_lastID() || e > return_lastID())
        throw std::runtime_error("Invalid BDD_ID given for ite operation");

    /*Terminal cases*/
    if(i==1) return t;                       //ite(1,t,e)
    if(i==0) return e;                       //ite(0,t,e)
    if(t==e) return t;                       //ite(i,t,t)
    if(t==1 && e==0) return i;               //ite(i,1,0)
    if(t==0 && e==1)                         //ite(i,0,1) should return neg(i)
    {
        TableRow *data = unique_table.getRowById(i);
        /*Check for the redundancy*/
        check_row_data = unique_table.getRowByData(data->low,data->high,data->topVar);
        if(check_row_data != nullptr)
            return check_row_data->id;
        else  
        {
            string label = "neg("+ data->label+")";
            new_row_data = {0,label,data->low,data->high,data->topVar};
            return unique_table.addRow(&new_row_data);
        }

    }

    /*Find top variable*/
    topVari =  ((i!=1)&(i!=0))? topVar(i): LIMIT;
    topVart =  ((t!=1)&(t!=0))? topVar(t): LIMIT;
    topVare =  ((e!=1)&(e!=0))? topVar(e): LIMIT;

    topVariable = min(topVari,topVart);
    topVariable = min(topVariable,topVare);

    /*Recursive ite to find successors*/
    high = ite(coFactorTrue(i,topVariable),coFactorTrue(t,topVariable),coFactorTrue(e,topVariable));
    low = ite(coFactorFalse(i,topVariable),coFactorFalse(t,topVariable),coFactorFalse(e,topVariable));

    /*Reduction rule*/
    if(high == low) return high;

    /*Check for the redundancy*/
    check_row_data = unique_table.getRowByData(high,low,topVariable);
    if(check_row_data != nullptr)
        return check_row_data->id;
    else
    {
        new_row_data = {0,*temp_label,high,low,topVariable};
        label_storage = "";
        return unique_table.addRow(&new_row_data); 
    }
}

/**
 *  Performs negation on the given node and returns the row ID of result
 */
BDD_ID Manager::neg(BDD_ID a) {
    TableRow* tr = unique_table.getRowById(a); 
    if(&tr) {
        return ite(a , FALSE_ROW , TRUE_ROW);
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
};

/**
 * Performs and operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::and2(BDD_ID a, BDD_ID b) //ite(a,b,0)
{
    if(a > return_lastID() || b > return_lastID())
        throw std::runtime_error("Invalid BDD_ID given for and operation");

    label_storage = unique_table.getRowById(a)->label + " and " + unique_table.getRowById(b)->label; 
    return ite(a,b,0);
}

/**
 * Performs or operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::or2(BDD_ID a, BDD_ID b) //ite(a,1,b)
{
    if(a > return_lastID() || b > return_lastID())
        throw std::runtime_error("Invalid BDD_ID given for or operation");

    label_storage = unique_table.getRowById(a)->label + " or " + unique_table.getRowById(b)->label; 
    return ite(a,1,b);
}

/**
 * Performs xor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::xor2(BDD_ID a, BDD_ID b) //ite(a,neg_b,b)
{
    if(a > return_lastID() || b > return_lastID())
        throw std::runtime_error("Invalid BDD_ID given for xor operation");

    BDD_ID neg_b = neg(b);
    label_storage = unique_table.getRowById(a)->label + " xor " + unique_table.getRowById(b)->label; 
    return ite(a,neg_b,b);
}

/**
 * Performs nand operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::nand2(BDD_ID a, BDD_ID b) //ite(a,b,0)
{
    TableRow* tra = unique_table.getRowById(a); 
    TableRow* trb = unique_table.getRowById(b); 
    if(&tra && &trb) {
        return ite(a , neg(b) , TRUE_ROW);
    } else {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
}

/**
 * Performs nor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::nor2(BDD_ID a, BDD_ID b) {
    TableRow* tra = unique_table.getRowById(a); 
    TableRow* trb = unique_table.getRowById(b); 
    if(&tra && &trb) {
        return ite(a , FALSE_ROW , neg(b));
    } else {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
};

/**
 * Performs exclusive nor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b) {
    TableRow* tra = unique_table.getRowById(a); 
    TableRow* trb = unique_table.getRowById(b); 
    if(&tra && &trb) {
        return ite(a , b , neg(b));
    } else {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
};

/**
 * Returns the label of the top variable of the given node
 */
string Manager::getTopVarName(const BDD_ID &root)
{
    TableRow* topVariable = unique_table.getRowById(root);
    if(topVariable == nullptr)
        throw std::runtime_error("Invalid BDD_ID given");
    else
        return unique_table.getRowById(topVariable->topVar)->label;
}

/**
 * Returns the size of the Unique table
 */
size_t Manager::uniqueTableSize() {
    return unique_table.tableSize();
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    findNodesOrVars(root , nodes_of_root , true);
};

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {
    findNodesOrVars(root , vars_of_root , false);
};

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{
    ofstream file;
    file.open(filepath);
    
    if (!file) {
        throw std::runtime_error("Invalid filepath");
        return;
    }

    file << "digraph BDD {\n";
    /*Implementation*/
    file << "}\n";

    file.close();
}