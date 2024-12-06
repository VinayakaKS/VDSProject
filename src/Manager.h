// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"
#include <iostream>
#include <table.cpp>

namespace ClassProject {
    class Manager : public ManagerInterface {
    
    public: 
        Manager() {
            unique_table.addRow("FALSE" , 0 , 0 , 0);
            unique_table.addRow("TRUE" , 1 , 1 , 1);
            unique_table.displayTable();
        };

        virtual BDD_ID createVar(const std::string &label) override {
            return -1;
        };

        virtual const BDD_ID &True() override {
            return TRUE_ROW;
        };

        virtual const BDD_ID &False() override {
            return FALSE_ROW;
        };

        virtual bool isConstant(BDD_ID f) override {
            return -1;
        };

        virtual bool isVariable(BDD_ID x) override {
            return -1;
        };

        virtual BDD_ID topVar(BDD_ID f) override {
            return -1;
        };

        virtual BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override {
            return -1;
        };

        virtual BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override {
            return -1;
        };

        virtual BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override {
            return -1;
        };

        virtual BDD_ID coFactorTrue(BDD_ID f) override {
            return -1;
        };

        virtual BDD_ID coFactorFalse(BDD_ID f) override {
            return -1;
        };

        virtual BDD_ID and2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

        virtual BDD_ID or2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

        virtual BDD_ID xor2(BDD_ID a, BDD_ID b) override {
            return -1;
        };

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

        virtual std::string getTopVarName(const BDD_ID &root) override {
            return "-1";
        };

        virtual void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override {
        };

        virtual void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override {
        };

        virtual size_t uniqueTableSize() override {
            return -1;
        };

        virtual void visualizeBDD(std::string filepath, BDD_ID &root) override {
        };
    

    private:
        DynamicTable unique_table;
        const BDD_ID FALSE_ROW = 99999;
        const BDD_ID TRUE_ROW = 99999;
    };
}

#endif
