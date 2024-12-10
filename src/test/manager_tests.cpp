//
// Created by tobias on 22.11.16
//

#include "Tests.h"
#include <set>

// Test fixture
class ManagerTest : public ::testing::Test {
protected:
    static ClassProject::Manager* obj;
    ClassProject::Manager* temp_obj;

    void SetUp() override {
        temp_obj = new ClassProject::Manager();  // Create object for each test case
    }

    void TearDown() override {
        delete temp_obj;  // Manually delete unique object after each test case
        temp_obj = nullptr;
    }

    static void SetUpTestSuite() {
        std::cout << "Setting up test suite\n";
        obj = new ClassProject::Manager(); // Initialize once for all tests
        std::cout << "obj initialized: " << obj << "\n";
    }

    static void TearDownTestSuite() {
        std::cout << "Tearing down test suite\n";
        delete obj;
        obj = nullptr;
    }
};

ClassProject::Manager* ManagerTest::obj = nullptr;

const ClassProject::BDD_ID FALSE_ID = 0;
const string FALSE_NAME = "FALSE";
const ClassProject::BDD_ID TRUE_ID = 1;
const string TRUE_NAME = "TRUE";
const ClassProject::BDD_ID A = 2;
const string A_NAME = "a";
const ClassProject::BDD_ID B = 3;
const string B_NAME = "b";
const ClassProject::BDD_ID C = 4;
const string C_NAME = "c";
const ClassProject::BDD_ID AandB = 4;
const ClassProject::BDD_ID D = 5;
const string D_NAME = "d";
const ClassProject::BDD_ID A_OR_B = 6;
const string A_OR_B_NAME = "a+b";
const ClassProject::BDD_ID C_AND_D = 7;
const string C_AND_D_NAME = "c*d";
const ClassProject::BDD_ID G = 8;
const ClassProject::BDD_ID F = 9;
const ClassProject::BDD_ID Invalid = 9999;
ClassProject::set <ClassProject::BDD_ID> non_empty_set_nodes = {10, 20};
//test to create a table to test my values


// checks the id for true label
TEST_F(ManagerTest, TRUE) {
    EXPECT_EQ( obj->True(), TRUE_ID );
}
// checks the id for true label
TEST_F(ManagerTest, FALSE) {
    EXPECT_EQ( obj->False(), FALSE_ID );
}
// createVar()_S
//valid label inputs for creating a ID
TEST_F(ManagerTest, Createvar_validlabels_id) {
    EXPECT_ANY_THROW( obj->createVar(" "));
    EXPECT_ANY_THROW( obj->createVar("+"));
    EXPECT_ANY_THROW( obj->createVar("^"));
};
//created var has a  valid BDD_ID
TEST_F(ManagerTest, Createvar_invalid_id) {
    EXPECT_EQ( obj->createVar("a"), A );
    EXPECT_EQ( obj->createVar("b"), B );
    EXPECT_EQ( obj->createVar("c"), C );
    EXPECT_EQ( obj->createVar("d"), D );
};
//same labels are refered with same BDDID
TEST_F(ManagerTest, Createvar_SameIDforsameLabel)
{
    EXPECT_ANY_THROW(obj->createVar("a"));
};
//BDDID is different for non-identical labels and incremented
TEST_F(ManagerTest, Createvar_UniQueidforlabel) {
    const ClassProject::BDD_ID id2 = temp_obj->createVar("b");
    const ClassProject::BDD_ID id3 = temp_obj->createVar("a+b");
    EXPECT_NE(id2, id3);  // Ensure the IDs are different
    EXPECT_EQ( id3, id2+1); // Ensure the incrementation
};
//uniqueTableSize()
TEST_F(ManagerTest,uniqueTableSize)
{
    EXPECT_NE( obj->uniqueTableSize(),9999999999+1);// check for terminal case
    EXPECT_EQ( temp_obj->uniqueTableSize(), 2);// check for the terminal case
    EXPECT_EQ( obj->uniqueTableSize(),6 );// check for the correct size
};

//isConstant_S
TEST_F(ManagerTest, isConstant_invalidinput) {
    EXPECT_ANY_THROW( obj->isConstant(Invalid));
}
TEST_F(ManagerTest, isConstant_validreturn) {
    EXPECT_EQ( obj->isConstant(TRUE_ID),true);
    EXPECT_EQ( obj->isConstant(FALSE_ID),true);
    EXPECT_EQ( obj->isConstant(A),false);
}
//end of isConstant

// //isVariable_
TEST_F(ManagerTest, isVariable_invalidinput) {
    EXPECT_ANY_THROW( obj->isVariable(Invalid));
}
TEST_F(ManagerTest, isVariable_validreturn) {
    obj->print_table();
    EXPECT_EQ( obj->isVariable(TRUE_ID),false);
    EXPECT_EQ( obj->isVariable(FALSE_ID),false);
    EXPECT_EQ( obj->isVariable(A),true);
};
// //end of isVariable

// //topVar()
//valid input
TEST_F(ManagerTest, topvar_invalidinput) {
    EXPECT_ANY_THROW( obj->topVar(Invalid));
};
//returns a valid BDD_ID
TEST_F(ManagerTest, topvar_validreturn) {
    EXPECT_NE( obj->topVar(TRUE_ID),Invalid);
    EXPECT_EQ( obj->topVar(TRUE_ID),TRUE_ID);
    EXPECT_EQ( obj->topVar(FALSE_ID),FALSE_ID);
    EXPECT_EQ( obj->topVar(A),A);
};
//end of topvar

//////////  START OF neg  ////////////

TEST_F(ManagerTest, negInvalidInput) {
    EXPECT_ANY_THROW( temp_obj->neg(Invalid) );
}

TEST_F(ManagerTest, negWorks)
{
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj->neg(A);
    EXPECT_EQ( temp_obj->getData(new_id)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A ); // points to the root variable
};

//////////  END OF neg  ////////////


//////////  START OF nand2  ////////////
TEST_F(ManagerTest, nand2InvalidInput) {
    EXPECT_ANY_THROW( temp_obj->nand2(Invalid, Invalid) );
}

TEST_F(ManagerTest, nand2Works)
{
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj-> nand2(A, B);
    EXPECT_EQ( temp_obj->getData(new_id - 1)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->topVar , B );
    EXPECT_EQ( temp_obj->getData(new_id)->high , (new_id - 1) );
    EXPECT_EQ( temp_obj->getData(new_id)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A );
};
//////////  END OF nand2  ////////////


//////////  START OF NOR ////////////
TEST_F(ManagerTest, nor2InvalidInput) {
    EXPECT_ANY_THROW( temp_obj->nor2(Invalid, Invalid) );
}

TEST_F(ManagerTest, nor2Works)
{
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj-> nor2(A, B);
    EXPECT_EQ( temp_obj->getData(new_id - 1)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->topVar , B );
    EXPECT_EQ( temp_obj->getData(new_id)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id)->low , (new_id - 1) );
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A );
};
//////////  END OF nor2  ////////////

//////////  START OF xnor2   ////////////
TEST_F(ManagerTest, xonr2InvalidInput) {
    EXPECT_ANY_THROW( temp_obj->xor2(Invalid , Invalid) );
}

TEST_F(ManagerTest, xnor2Works) {
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj->xnor2(A , B);
    EXPECT_EQ( temp_obj->getData(new_id - 1)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->topVar , B );
    EXPECT_EQ( temp_obj->getData(new_id)->high , B );
    EXPECT_EQ( temp_obj->getData(new_id)->low ,(new_id - 1));
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A );
}
//////////  END OF nor2   ////////////


//////////  START OF findNodes  ////////////
TEST_F(ManagerTest, findNodes_InvalidInput) {
    EXPECT_ANY_THROW( temp_obj->findNodes(Invalid , non_empty_set_nodes) );
}

TEST_F(ManagerTest, findNodes_work)
{
    temp_obj-> and2(or2(A, B), and2(C, D));
    ClassProject:: set <ClassProject::BDD_ID> nodes_of_root;
    temp_obj-> findNodes(F,nodes_of_root);
    EXPECT_NE(nodes_of_root.find(G), nodes_of_root.end());// should contain id8
    EXPECT_NE(nodes_of_root.find(C_AND_D), nodes_of_root.end()); // should contain id7
    EXPECT_NE(nodes_of_root.find(D), nodes_of_root.end()); // should contain id5
    EXPECT_NE(nodes_of_root.find(F), nodes_of_root.end());// should contain itself
    EXPECT_EQ(nodes_of_root.size(), 6);
}

//////////  END OF findNodes  ////////////

//////////  START OF findVars////////////
TEST_F(ManagerTest, findVars_InvalidInput) {
    EXPECT_ANY_THROW( temp_obj->findNodes(Invalid , non_empty_set_nodes) );
}

TEST_F(ManagerTest, findVars_work)
{
    temp_obj-> and2(or2(A, B), and2(C, D));
    ClassProject:: set <ClassProject::BDD_ID> vars_of_root;
    temp_obj-> findNodes(F,vars_of_root);
    EXPECT_NE(vars_of_root.find(A), vars_of_root.end());// should contain id2
    EXPECT_NE(vars_of_root.find(B), vars_of_root.end()); // should contain id3
    EXPECT_NE(vars_of_root.find(C), vars_of_root.end()); // should contain id4
    EXPECT_NE(vars_of_root.find(D), vars_of_root.end());// should contain id5
    EXPECT_EQ(vars_of_root.find(TRUE_ID), vars_of_root.end());// should not contain True_id
    EXPECT_EQ(vars_of_root.find(FALSE_ID), vars_of_root.end());// should not contain False_id
    EXPECT_EQ(vars_of_root.size(), 4);
}
//////////  START OF ITE   ////////////
//Throws Exception when calling ite with an invalid BDD_ID
TEST_F(ManagerTest, iteException) {
    EXPECT_ANY_THROW( obj->ite(Invalid , FALSE_ID , FALSE_ID));
}

// Terminal Case 1 Test
TEST_F(ManagerTest, iteTerminal1) {
    obj->print_table();
    EXPECT_EQ( obj->ite(TRUE_ID , B , C) , B);
}

// Terminal Case 2 Test
TEST_F(ManagerTest, iteTerminal2) {  
    EXPECT_EQ( obj->ite(FALSE_ID , B , C) , C);
}

// Terminal Case 3 Test
TEST_F(ManagerTest, iteTerminal3) {  
    EXPECT_EQ( obj->ite(B , C , C) , C);
}

// Terminal Case 4 Test
TEST_F(ManagerTest, iteTerminal4) {  
    EXPECT_EQ( obj->ite(B , TRUE_ID , FALSE_ID) , B);
}

// Terminal Case 5 Test
TEST_F(ManagerTest, iteTerminal5) {  
    EXPECT_EQ( obj->ite(B , FALSE_ID , FALSE_ID) , FALSE_ID);
}

// Terminal Case 6 Test
TEST_F(ManagerTest, iteTerminal6) {  
    EXPECT_EQ( obj->ite(B , TRUE_ID , TRUE_ID) , TRUE_ID);
}

// A OR B Case 7 Test
TEST_F(ManagerTest, iteAorB) {  
    obj->print_table();
    EXPECT_EQ( obj->ite(A , TRUE_ID , B) , A_OR_B);
}

// C and D Case 7 Test
TEST_F(ManagerTest, iteCandD) {  
    EXPECT_EQ( obj->ite(C , D , FALSE_ID) , C_AND_D);
    obj->print_table();
}
//////////  END OF ITE   ////////////



//////////  START OF COFACTOR TRUE 2 Parameters   ////////////
TEST_F(ManagerTest, coFactorTrueInvalidId) {  
    EXPECT_ANY_THROW( obj->coFactorTrue(A_OR_B , Invalid));
}

TEST_F(ManagerTest, coFactorTrueNotVariable) {  
    EXPECT_ANY_THROW( obj->coFactorTrue(A_OR_B , C_AND_D));
}

TEST_F(ManagerTest, coFactorTrueTopVariable) { 
    obj->print_table() ;
    EXPECT_EQ( obj->coFactorTrue(C_AND_D , C) , D);
}

TEST_F(ManagerTest, coFactorTrueNotTopVariable) {  
    EXPECT_EQ( obj->coFactorTrue(C_AND_D , A) , C_AND_D);
}
//////////  END OF COFACTOR TRUE 2 Parameters   ////////////


//////////  START OF COFACTOR FALSE 2 Parameters   ////////////
TEST_F(ManagerTest, coFactorFalseInvalidId) {  
    EXPECT_ANY_THROW( obj->coFactorFalse(A_OR_B , Invalid));
}

TEST_F(ManagerTest, coFactorFalseNotVariable) {  
    EXPECT_ANY_THROW( obj->coFactorFalse(A_OR_B , C_AND_D));
}

TEST_F(ManagerTest, coFactorFalseTopVariable) {  
    EXPECT_EQ( obj->coFactorFalse(A_OR_B , A) , B);
}

TEST_F(ManagerTest, coFactorFalseNotTopVariable) {  
    EXPECT_EQ( obj->coFactorFalse(A_OR_B , C) , A_OR_B);
}
//////////  END OF COFACTOR FALSE 2 Parameters   ////////////


//////////  START OF COFACTOR TRUE 1  Parameter   ////////////
TEST_F(ManagerTest, coFactorTrueInvalidIdTop) {  
    EXPECT_ANY_THROW( obj->coFactorTrue(Invalid));
}

TEST_F(ManagerTest, coFactorTrueTop) {  
    EXPECT_EQ( obj->coFactorTrue(A_OR_B) , TRUE_ID);
}
//////////  END OF COFACTOR TRUE 1  Parameter   ////////////


//////////  START OF COFACTOR FALSE 1  Parameter   ////////////
TEST_F(ManagerTest, coFactorFalseInvalidIdTop) {  
    EXPECT_ANY_THROW( obj->coFactorFalse(Invalid));
}

TEST_F(ManagerTest, coFactorFalseTop) {  
    EXPECT_EQ( obj->coFactorFalse(A_OR_B) , B);
}
//////////  END OF COFACTOR FALSE 1  Parameter   ////////////


//////////  Final Test for Top Var after introducing functions with ite   ////////////
TEST_F(ManagerTest, topVarFunction) {  
    EXPECT_EQ( obj->topVar(A_OR_B),A);
}
//////////  END OF TOPVAR  ////////////


//////////  START OF and2   ////////////
TEST_F(ManagerTest, and2InvalidInput) {  
    EXPECT_ANY_THROW( temp_obj->and2(Invalid , Invalid) );
}

TEST_F(ManagerTest, and2Works) {  
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj->and2(A , B);
    TableRow* data = temp_obj->getData(new_id);
    EXPECT_EQ(data->high , B );
    EXPECT_EQ( data->low , FALSE_ID );
    EXPECT_EQ( data->topVar , A );
}
//////////  END OF and2   ////////////


//////////  START OF or2   ////////////
TEST_F(ManagerTest, or2InvalidInput) {  
    EXPECT_ANY_THROW( temp_obj->or2(Invalid , Invalid) );
}

TEST_F(ManagerTest, or2Works) {  
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj->or2(A , B);
    EXPECT_EQ( temp_obj->getData(new_id)->high , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id)->low , B );
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A );
}
//////////  END OF or2   ////////////

//////////  START OF xor2   ////////////
TEST_F(ManagerTest, xor2InvalidInput) {  
    EXPECT_ANY_THROW( temp_obj->xor2(Invalid , Invalid) );
}

TEST_F(ManagerTest, xor2Works) {  
    temp_obj->createVar("a");
    temp_obj->createVar("b");
    ClassProject::BDD_ID new_id = temp_obj->xor2(A , B);
    EXPECT_EQ( temp_obj->getData(new_id - 1)->high , FALSE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->low , TRUE_ID );
    EXPECT_EQ( temp_obj->getData(new_id - 1)->topVar , B );
    EXPECT_EQ( temp_obj->getData(new_id)->high , (new_id - 1) );
    EXPECT_EQ( temp_obj->getData(new_id)->low , B );
    EXPECT_EQ( temp_obj->getData(new_id)->topVar , A );
}
//////////  END OF xor2   ////////////

// TEST_F(ManagerTest, getTopVarName_valid) {
//     EXPECT_ANY_THROW(temp_obj->getTopVarName(Invalid));
// }

// TEST_F(ManagerTest, getTopVarName_works){
//     temp_obj-> and2(A,B);
//     EXPECT_EQ(temp_obj->getTopVarName(AandB), A_NAME);
//     EXPECT_EQ(temp_obj->getTopVarName(TRUE_ID), TRUE_ID);
// }





//////////  START OF getTopVarName   ////////////
TEST_F(ManagerTest, getTopVarNameInvalidInput) {  
    EXPECT_ANY_THROW( obj->getTopVarName(Invalid) );
}

TEST_F(ManagerTest, getTopVarNameofConstant) {  
    EXPECT_EQ( obj->getTopVarName(FALSE_ID), "FALSE" );
    EXPECT_EQ( obj->getTopVarName(TRUE_ID), "TRUE" );
}

TEST_F(ManagerTest, getTopVarNameofVariable) {  
    EXPECT_EQ( obj->getTopVarName(A), "a" );
    EXPECT_EQ( obj->getTopVarName(B), "b" );
}

TEST_F(ManagerTest, getTopVarNameofNode) {  
    obj->print_table();
    EXPECT_EQ( obj->getTopVarName(A_OR_B), "a" );
    EXPECT_EQ( obj->getTopVarName(C_AND_D), "c" );
}
//////////  END OF getTopVarName   ////////////


