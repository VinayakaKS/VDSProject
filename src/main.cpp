//
// Created by Carolina P. Nogueira 2016
//

#include <iostream>
#include <string>
#include "Manager.h"
ClassProject::Manager manager;

using namespace ClassProject;
int main(int argc, char* argv[])
{
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    manager.createVar("d");

    manager.or2(2,3);
    manager.and2(4,5);
    manager.and2(6,7);

    manager.print_table();

    BDD_ID id = manager.return_lastID(); 
    manager.visualizeBDD("../src/graph/BDD.dot",id);
}
