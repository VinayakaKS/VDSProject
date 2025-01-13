// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"
#include <iostream>
#include <table.cpp>
#include <regex>
namespace ClassProject {
    
    class Manager : public ManagerInterface {
    
    public:
        Manager() {
            TableRow true_node = {1,"True",1,1,1};
            TableRow false_node = {0,"False",0,0,0};
            unique_table.addRow(&false_node);
            unique_table.addRow(&true_node);
        }
        
        virtual BDD_ID createVar(const std::string &label);

        virtual const BDD_ID &True() override {
            return TRUE_ROW;
        };

        virtual const BDD_ID &False() override {
            return FALSE_ROW;
        };

        virtual bool isConstant(BDD_ID f);

        virtual bool isVariable(BDD_ID x);

        virtual BDD_ID topVar(BDD_ID f);

        virtual BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e);

        virtual BDD_ID coFactorTrue(BDD_ID f, BDD_ID x);

        virtual BDD_ID coFactorFalse(BDD_ID f, BDD_ID x);

        virtual BDD_ID coFactorTrue(BDD_ID f);

        virtual BDD_ID coFactorFalse(BDD_ID f);

        virtual BDD_ID and2(BDD_ID a, BDD_ID b);

        virtual BDD_ID or2(BDD_ID a, BDD_ID b);

        virtual BDD_ID xor2(BDD_ID a, BDD_ID b);

        virtual BDD_ID neg(BDD_ID a);

        virtual BDD_ID nand2(BDD_ID a, BDD_ID b) ;

        virtual BDD_ID nor2(BDD_ID a, BDD_ID b);

        virtual BDD_ID xnor2(BDD_ID a, BDD_ID b);

        virtual std::string getTopVarName(const BDD_ID &root);

        virtual void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);

        virtual void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);

        virtual size_t uniqueTableSize();

        void visualizeBDD(std::string filepath, BDD_ID &root);
        
        string graphString(BDD_ID id);
        
        string cleanString(string graph_string);

        void print_table();
        
        int return_lastID();

        TableRow* getData(BDD_ID f);

        void findNodesOrVars(const BDD_ID &root, set<BDD_ID> &nodes_of_root , bool node);

        void addToSet(set<BDD_ID> &nodes_of_root , BDD_ID id , bool node);

        DynTable computed_table;
        DynamicTable unique_table;


    private:
        const BDD_ID FALSE_ROW = 0;
        const BDD_ID TRUE_ROW = 1;
    };
}



#endif
