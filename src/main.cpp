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
    manager.print_table();

    std::cout << "Nothing implemented, yet" << std::endl;
}
