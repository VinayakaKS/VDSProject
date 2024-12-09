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
            TableRow true_node = {1,"TRUE",1,1,1};
            TableRow false_node = {0,"FALSE",0,0,0};
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

        virtual BDD_ID neg(BDD_ID a) override {
            return -1;
        };

        virtual BDD_ID nand2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

        virtual BDD_ID nor2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

        virtual BDD_ID xnor2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

        virtual std::string getTopVarName(const BDD_ID &root);

        virtual void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override {
        };

        virtual void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override {
        };

        virtual size_t uniqueTableSize();

        virtual void visualizeBDD(std::string filepath, BDD_ID &root) override {
        };

        void print_table() {
            unique_table.displayTable();
        }

        int return_lastID(){
            return unique_table.last_id-1;
        }

        TableRow* getData(BDD_ID f) {
            TableRow* tr = unique_table.getRowById(f); 
            if(tr) {
                return tr;
            } else {
                throw std::runtime_error("Row with this id does not exist.");
            }
        }

    private:
        DynamicTable unique_table;
        const BDD_ID FALSE_ROW = 0;
        const BDD_ID TRUE_ROW = 1;
    };
}



#endif
