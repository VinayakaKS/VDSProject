#include "Manager.h"

using namespace std;
using namespace ClassProject;
#include <fstream>
#include <iostream>
#include <string>

typedef size_t BDD_ID;
string label_storage;

/**
 * Creates a node for the variable given.
 */
BDD_ID Manager::createVar(const std::string &label) {
    std::regex pattern("^!?[A-Za-z0-9]([+*^!][A-Za-z0-9])*$");
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
    TableRow* tr = getData(f);
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
    TableRow* tr = getData(x); 
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
    TableRow* tr = getData(f); 
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
    {
        throw std::runtime_error("Invalid BDD_ID - True cofactor doesnt exist");
    }
        
    if(!isVariable(x) && !isConstant(x))
    {
        throw std::runtime_error("Not a variable or constant - True cofactor doesnt exist");
    }
         
    TableRow* row;
    if(topVar(f) == x)
    {
        row = getData(f);
        return row->high;
    } else if (f == FALSE_ROW || f == TRUE_ROW) {
        return f;
    }
        
    // x is not the top variable: recursively compute cofactor for subfunctions
    BDD_ID lowBranch = coFactorTrue(getData(f)->low, x);
    BDD_ID highBranch = coFactorTrue(getData(f)->high, x);
    
    // Reconstruct the ITE for the function without x
    return ite(topVar(f), highBranch, lowBranch);
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
    {
        throw std::runtime_error("Invalid BDD_ID - False cofactor doesnt exist");
    }
        
    if(!isVariable(x) && !isConstant(x))
    {
        throw std::runtime_error("Not a variable or constant - True cofactor doesnt exist");
    }
        
    TableRow* row;
    if(topVar(f) == x)
    {
        row = getData(f);
        return row->low;
    } else if (f == FALSE_ROW || f == TRUE_ROW) {
        return f;
    }

    // x is not the top variable: recursively compute cofactor for subfunctions
    BDD_ID lowBranch = coFactorFalse(getData(f)->low, x);
    BDD_ID highBranch = coFactorFalse(getData(f)->high, x);
    
    // Reconstruct the ITE for the function without x
    return ite(topVar(f), highBranch, lowBranch);
}

/**
 * Returns the true co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorTrue(BDD_ID f){
    if(f > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID");
    }
    return getData(f)->high;
}

/**
 * Returns the false co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorFalse(BDD_ID f){
    if(f > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID");
    }
    return getData(f)->low;
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
    BDD_ID Computed_table_ID;
    TableRow new_row_data;
    TableRow *check_row_data;
    string* temp_label = &label_storage;

    /*Invalid BDD_ID exception*/
    if(i > return_lastID() || t > return_lastID() || e > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for ite operation");
    }
        
    /*Terminal cases*/
    if(i==1) return t;                       //ite(1,t,e)
    if(i==0) return e;                       //ite(0,t,e)
    if(t==e) return t;                       //ite(i,t,t)
    if(t==1 && e==0) return i;               //ite(i,1,0)
    if(t==0 && e==1)                         //ite(i,0,1) should return neg(i)
    {
        TableRow *data = getData(i);
        if(isVariable(i)) {
            /*Check for the redundancy*/
            check_row_data = unique_table.getRowByData(data->low,data->high,data->topVar);
            if(check_row_data != nullptr){
                return check_row_data->id;
            }
            else  
            {
                string label = "neg("+ data->label+")";
                new_row_data = {0,label,data->low,data->high,data->topVar};
                return unique_table.addRow(&new_row_data);
            }
        }
    }
    check_row_data = computed_table.getRowByData(high,low,topVariable);
    if (check_row_data != nullptr)
    {
        return check_row_data->id;
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
    if(high == low) {
        return high;
    }
    /*Check for the redundancy*/
    check_row_data = unique_table.getRowByData(high,low,topVariable);
    if(check_row_data != nullptr)
    {
        return check_row_data->id;
    }
    else
    {
        new_row_data = {0,*temp_label,high,low,topVariable};
        label_storage = "";
        Computed_table_ID = unique_table.addRow(&new_row_data);
        new_row_data = {Computed_table_ID,*temp_label,high,low,topVariable};
        computed_table.addRow_Computed(&new_row_data);
        return Computed_table_ID;


    }
}

/**
 *  Performs negation on the given node and returns the row ID of result
 */
BDD_ID Manager::neg(BDD_ID a) {
    TableRow* tr = getData(a); 
    if(tr) {
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
    {
        throw std::runtime_error("Invalid BDD_ID given for and operation");
    }
        
    label_storage = getData(a)->label + " and " + getData(b)->label; 
    return ite(a,b,0);
}

/**
 * Performs or operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::or2(BDD_ID a, BDD_ID b) //ite(a,1,b)
{
    if(a > return_lastID() || b > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for or operation");
    }
        
    label_storage = getData(a)->label + " or " + getData(b)->label; 
    return ite(a,1,b);
}

/**
 * Performs xor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::xor2(BDD_ID a, BDD_ID b) //ite(a,neg_b,b)
{
    if(a > return_lastID() || b > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for xor operation");
    }
        
    BDD_ID neg_b = neg(b);
    label_storage = getData(a)->label + " xor " + getData(b)->label; 
    return ite(a,neg_b,b);
}

/**
 * Performs nand operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::nand2(BDD_ID a, BDD_ID b) //ite(a,b,0)
{
    TableRow* tra = getData(a); 
    TableRow* trb = getData(b); 
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
    TableRow* tra = getData(a); 
    TableRow* trb = getData(b); 
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
    TableRow* tra = getData(a); 
    TableRow* trb = getData(b); 
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
    TableRow* topVariable = getData(root);
    if(topVariable == nullptr)
    {
        throw std::runtime_error("Invalid BDD_ID given");
    }
    else
    {
        return getData(topVariable->topVar)->label;
    }     
}

/**
 * Returns the size of the Unique table
 */
size_t Manager::uniqueTableSize() {
    return unique_table.tableSize();
}

/**
 * It fills the set nodes_of_root with all nodes which are reachable from root.
 */
void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    findNodesOrVars(root , nodes_of_root , true);
};

/**
 * It fills the set nodes_of_root with all variables which are reachable from root.
 */
void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {
    findNodesOrVars(root , vars_of_root , false);
};

/**
 * It fills the set nodes_of_root with all variables or nodes which are reachable from root based on the node/variable condition.
 */
void Manager::findNodesOrVars(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root , bool node = true) {
    TableRow* tr = getData(root); 
    if(tr) 
    {
        addToSet(nodes_of_root , tr->id , node);
        if(!node && isVariable(tr->topVar))
        {
            addToSet(nodes_of_root , tr->topVar , node);
        }
        
        // Only continue recursing when addToSet is successful
        addToSet(nodes_of_root , tr->high , node);
        addToSet(nodes_of_root , tr->low , node);
        if(!isConstant(tr->high)) {
            findNodesOrVars(tr->high , nodes_of_root , node);
        }
        if(!isConstant(tr->low)) {
            findNodesOrVars(tr->low , nodes_of_root , node);
        }
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
}    

void Manager::addToSet(std::set<BDD_ID> &nodes_of_root , BDD_ID id , bool node = true ) {
    if(node || (!node && isVariable(id))) {
        nodes_of_root.insert(id);
    }
}

/**
 * Prints Unique table
 */
void Manager::print_table() {
    unique_table.displayTable();
    computed_table.displayTable();
}

/**
 * Returns the last assigned BDD
 */
int Manager::return_lastID(){
    return unique_table.last_id-1;
}

/**
 * Gets the data from the Row
 */
TableRow* Manager::getData(BDD_ID f) {
    TableRow* tr = unique_table.getRowById(f); 
    if(tr) {
        return tr;
    } else {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Creates the dot file and copies the data into it
 */
void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{
    string graph_styling,node_styling, constant_styling, style;
    /*File operation*/
    ofstream file;
    file.open(filepath);
    
    if (!file) {
        throw std::runtime_error("Invalid filepath");
        return;
    }

    graph_styling = "\tgraph [ranksep=0.5, nodesep=0.5];\n";
    node_styling = "\tnode [style=filled, fillcolor=lightblue, color=darkblue, shape=circle, fixedsize=true, height=0.55];\n";
    constant_styling = "\tFalse,True\t[shape=square, fillcolor=white, color=black];\n";
    style = graph_styling + node_styling + constant_styling;
    
    file << "graph BDD {\n";
    file << style;
    file << cleanString(graphString(root));   //Data
    file << "}";

    file.close();
    
    /*
    int returnCode = system("chmod +x BDD.sh");
    returnCode = system("../src/graph/BDD.sh");

    // Check the return code
    if (returnCode == 0) {
        std::cout << "Graph created successfully!" << std::endl;
    } else {
        std::cout << "Error: Script execution failed with code " << returnCode << std::endl;
    }
    */
}

/**
 * Pushes all the connections into the string recursively
 */
string Manager::graphString(BDD_ID id)
{
    string return_string = "";
    TableRow* tr = getData(id);

    if(tr)
    {
        if(!isConstant(tr->id))
        {
            return_string += getTopVarName(tr->id) + "--" + getTopVarName(tr->low) + "\t[style = dotted];\n";
            return_string += getTopVarName(tr->id) + "--" + getTopVarName(tr->high)+ ";\n";

            return_string += graphString(tr->low);
            return_string += graphString(tr->high);

            return return_string;

        }
        else    
        {
            return return_string;
        }   
    }

    else
    {
        throw std::runtime_error("Rows with these ids do not exist.");
    }          
}

/**
 * Cleans the string for any duplicate connections
 */
string Manager::cleanString(string graph_string)
{
    stringstream ss(graph_string);       //Stream to process the input string
    string line;
    string out = "";

    // Read each line from the input string
    while (std::getline(ss, line)) {
        if (out.find(line) == std::string::npos)
        {
            out += "\t" + line + "\n";
        } 
    }

    return out;
}




