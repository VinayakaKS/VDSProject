#include "Manager.h"

using namespace std;
using namespace ClassProject;
#include <fstream>
#include <iostream>
#include <string>

typedef size_t BDD_ID;

/**
 * Creates a node for the variable given.
 */
BDD_ID Manager::createVar(const std::string &label)
{
    if (unique_table.getRowByLabel(label))
    {
        throw std::runtime_error("Variable label already exists. Please try a new label");
    }
    else
    {
        TableRow new_var = {label};
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
bool Manager::isConstant(BDD_ID f)
{
    if (!(f > return_lastID()))
    {
        return (f == FALSE_ROW || f == TRUE_ROW);
    }
    else
    {
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
bool Manager::isVariable(BDD_ID x)
{
    TableRow *tr = getData(x);
    if (tr)
    {
        return (x != FALSE_ROW && x != TRUE_ROW && tr->high == TRUE_ROW && tr->low == FALSE_ROW && tr->topVar == x);
    }
    else
    {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Returns the ID of the top variable of the passed node
 */
BDD_ID Manager::topVar(BDD_ID f)
{
    TableRow *tr = getData(f);
    if (tr)
    {
        return tr->topVar;
    }
    else
    {
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
BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    if (f > return_lastID() || x > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID - True cofactor doesnt exist");
    }

    if (!isVariable(x) && !isConstant(x))
    {
        throw std::runtime_error("Not a variable or constant - True cofactor doesnt exist");
    }

    TableRow *row = getData(f);
    if (row->topVar == x)
    {
        return row->high;
    }
    else if (f == FALSE_ROW || f == TRUE_ROW)
    {
        return f;
    }

    BDD_ID lowBranch = coFactorTrue(row->low, x);
    BDD_ID highBranch = coFactorTrue(row->high, x);

    return ite(row->topVar, highBranch, lowBranch);
}

/**
 * Returns the false cofactor of the node with respect to the variable passed
 *
 * @param   f : BDD_ID of the node to be checked
 *          x : variable with respect to
 * @return  ID : returns the false - cofactor of the function
 */
BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    if (f > return_lastID() || x > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID - False cofactor doesnt exist");
    }

    if (!isVariable(x) && !isConstant(x))
    {
        throw std::runtime_error("Not a variable or constant - True cofactor doesnt exist");
    }

    TableRow *row = getData(f);
    if (row->topVar == x)
    {
        return row->low;
    }
    else if (f == FALSE_ROW || f == TRUE_ROW)
    {
        return f;
    }

    BDD_ID lowBranch = coFactorFalse(row->low, x);
    BDD_ID highBranch = coFactorFalse(row->high, x);

    return ite(row->topVar, highBranch, lowBranch);
}

/**
 * Returns the true co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorTrue(BDD_ID f)
{
    if (f > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID");
    }
    return getData(f)->high;
}

/**
 * Returns the false co factor of the node with respect to the node's top variable
 */
BDD_ID Manager::coFactorFalse(BDD_ID f)
{
    if (f > return_lastID())
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
    BDD_ID topVariable;
    BDD_ID high, low;
    BDD_ID Cp_ID;
    TableRow new_row_data;
    size_t check_row_data_ID, CPT_Id;
    CPTableRow new_cpt_row = {0, i, t, e};
    string temp_label = "";

    if (i > return_lastID() || t > return_lastID() || e > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for ite operation");
    }

    /*Terminal cases*/
    if (i == 1)
        return t; // ite(1,t,e)
    if (i == 0)
        return e; // ite(0,t,e)
    if (t == e)
        return t; // ite(i,t,t)
    if (t == 1 && e == 0)
        return i; // ite(i,1,0)
    TableRow *data = getData(i);
    if (t == 0 && e == 1) // ite(i,0,1) should return neg(i)
    {
        if (i != FALSE_ROW && i != TRUE_ROW && data->high == TRUE_ROW && data->low == FALSE_ROW && data->topVar == i)
        {
            /*Check for the redundancy*/
            check_row_data_ID = unique_table.getRowByData({data->low, data->high, data->topVar});
            if (check_row_data_ID != -1)
            {
                return check_row_data_ID;
            }
            else
            {
                new_row_data = {"", data->low, data->high, data->topVar};
                check_row_data_ID = unique_table.addRow(&new_row_data);
                new_cpt_row.id = check_row_data_ID;
                computed_table.addRowCPTable(&new_cpt_row);
                return check_row_data_ID;
            }
        }
    }

    // Standard triplets
    if (i == t)
    {
        return ite(i, 1, e);
    }
    if (i == e)
    {
        return ite(i, t, 0);
    }

    CPT_Id = computed_table.getCPRowByHash({i, t, e});
    if (CPT_Id != -1)
    {
        return CPT_Id;
    }

    else
    {

        topVariable = data->topVar;
        if (!isConstant(t))
        {
            topVariable = min(topVariable, topVar(t));
            if (!isConstant(e))
            {
                topVariable = min(topVariable, topVar(e));
            }
        }
        else if (!isConstant(e))
        {
            topVariable = min(topVariable, topVar(e));
        }

        high = ite(coFactorTrue(i, topVariable), coFactorTrue(t, topVariable), coFactorTrue(e, topVariable));
        low = ite(coFactorFalse(i, topVariable), coFactorFalse(t, topVariable), coFactorFalse(e, topVariable));

        if (high == low)
        {
            return high;
        }

        check_row_data_ID = unique_table.getRowByData({high, low, topVariable});
        if (check_row_data_ID != -1)
        {
            new_cpt_row.id = check_row_data_ID;
            computed_table.addRowCPTable(&new_cpt_row);
            return check_row_data_ID;
        }
        else
        {
            new_row_data = {"", high, low, topVariable};
            Cp_ID = unique_table.addRow(&new_row_data);
            new_cpt_row.id = Cp_ID;
            computed_table.addRowCPTable(&new_cpt_row);
            return Cp_ID;
        }
    }
}

/**
 *  Performs negation on the given node and returns the row ID of result
 */
BDD_ID Manager::neg(BDD_ID a)
{
    if (!(a > return_lastID()))
    {
        return ite(a, FALSE_ROW, TRUE_ROW);
    }
    else
    {
        throw std::runtime_error("Row with this id does not exist.");
    }
};

/**
 * Performs and operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::and2(BDD_ID a, BDD_ID b) // ite(a,b,0)
{
    if (a > return_lastID() || b > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for and operation");
    }
    return ite(a, b, 0);
}

/**
 * Performs or operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::or2(BDD_ID a, BDD_ID b) // ite(a,1,b)
{
    if (a > return_lastID() || b > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for or operation");
    }
    return ite(a, 1, b);
}

/**
 * Performs xor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::xor2(BDD_ID a, BDD_ID b) // ite(a,neg_b,b)
{
    if (a > return_lastID() || b > return_lastID())
    {
        throw std::runtime_error("Invalid BDD_ID given for xor operation");
    }
    return ite(a, neg(b), b);
}

/**
 * Performs nand operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::nand2(BDD_ID a, BDD_ID b) // ite(a,b,0)
{
    if (!(a > return_lastID() || b > return_lastID()))
    {
        return ite(a, neg(b), TRUE_ROW);
    }
    else
    {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
}

/**
 * Performs nor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{
    if (!(a > return_lastID() || b > return_lastID()))
    {
        return ite(a, FALSE_ROW, neg(b));
    }
    else
    {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
};

/**
 * Performs exclusive nor operation on the given nodes and returns the row ID of result
 */
BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{
    if (!(a > return_lastID() || b > return_lastID()))
    {
        return ite(a, b, neg(b));
    }
    else
    {
        throw std::runtime_error("Rows with these ids do not exist.");
    }
};

/**
 * Returns the label of the top variable of the given node
 */
string Manager::getTopVarName(const BDD_ID &root)
{
    TableRow *topVariable = getData(root);
    if (topVariable == nullptr)
    {
        throw std::runtime_error("Invalid BDD_ID given");
    }

    return getData(topVariable->topVar)->label;
}

/**
 * Returns the size of the Unique table
 */
size_t Manager::uniqueTableSize()
{
    return unique_table.tableSize();
}

/**
 * It fills the set nodes_of_root with all nodes which are reachable from root.
 */
void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    findNodesOrVars(root, nodes_of_root, true);
};

/**
 * It fills the set nodes_of_root with all variables which are reachable from root.
 */
void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{
    findNodesOrVars(root, vars_of_root, false);
};

/**
 * It fills the set nodes_of_root with all variables or nodes which are reachable from root based on the node/variable condition.
 */
void Manager::findNodesOrVars(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root, bool node = true)
{
    TableRow *tr = getData(root);
    if (tr)
    {
        addToSet(nodes_of_root, tr->id, node);
        if ((!node && isVariable(tr->topVar)))
        {
            addToSet(nodes_of_root, tr->topVar, node);
        }

        if (addToSet(nodes_of_root, tr->high, node))
        {
            if (!isConstant(tr->high))
            {
                findNodesOrVars(tr->high, nodes_of_root, node);
            }
        };

        if (addToSet(nodes_of_root, tr->low, node))
        {
            if (!isConstant(tr->low))
            {
                findNodesOrVars(tr->low, nodes_of_root, node);
            }
        };
    }
    else
    {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

bool Manager::addToSet(std::set<BDD_ID> &nodes_of_root, BDD_ID id, bool node = true)
{
    if (node || (!node && isVariable(id)))
    {
        return (nodes_of_root.insert(id).second);
    }
    else
    {
        return false;
    }
}

/**
 * Prints Unique table
 */
void Manager::print_table()
{
    unique_table.displayTable();
    computed_table.displayTable();
}

/**
 * Returns the last assigned BDD
 */
int Manager::return_lastID()
{
    return unique_table.last_id - 1;
}

/**
 * Gets the data from the Row
 */
TableRow *Manager::getData(BDD_ID f)
{
    TableRow *tr = unique_table.getRowById(f);
    if (tr)
    {
        return tr;
    }
    else
    {
        throw std::runtime_error("Row with this id does not exist.");
    }
}

/**
 * Creates the dot file and copies the data into it
 */
void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{
    string graph_styling, node_styling, constant_styling, style;
    ofstream file;
    file.open(filepath);

    if (!file)
    {
        throw std::runtime_error("Invalid filepath");
        return;
    }

    graph_styling = "\tgraph [ranksep=0.5, nodesep=0.5];\n";
    node_styling = "\tnode [style=filled, fillcolor=lightblue, color=darkblue, shape=circle, fixedsize=true, height=0.55];\n";
    constant_styling = "\tFalse,True\t[shape=square, fillcolor=white, color=black];\n";
    style = graph_styling + node_styling + constant_styling;

    file << "graph BDD {\n";
    file << style;
    file << cleanString(graphString(root));
    file << "}";

    file.close();
}

/**
 * Pushes all the connections into the string recursively
 */
string Manager::graphString(BDD_ID id)
{
    string return_string = "";
    TableRow *tr = getData(id);

    if (tr)
    {
        if (!isConstant(tr->id))
        {
            return_string += getTopVarName(tr->id) + "--" + getTopVarName(tr->low) + "\t[style = dotted];\n";
            return_string += getTopVarName(tr->id) + "--" + getTopVarName(tr->high) + ";\n";

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
    stringstream ss(graph_string);
    string line;
    string out = "";
    while (std::getline(ss, line))
    {
        if (out.find(line) == std::string::npos)
        {
            out += "\t" + line + "\n";
        }
    }
    return out;
}
