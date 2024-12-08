//
// Created by tobias on 22.11.16
//

#include "Tests.h"

// Test fixture
class ManagerTest : public ::testing::Test {
protected:
    static ClassProject::Manager* obj;
    ClassProject::Manager* temp_obj;

    void SetUp() override {
        if (temp_obj == nullptr) {
            temp_obj = new ClassProject::Manager();  // Create object for each test case
        }
    }

    void TearDown() override {
        if (temp_obj != nullptr) {
            delete temp_obj;  // Manually delete unique object after each test case
            temp_obj = nullptr;
        }
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
const ClassProject::BDD_ID TRUE_ID = 1;
const ClassProject::BDD_ID A = 2;
const ClassProject::BDD_ID B = 3;
const ClassProject::BDD_ID C = 4;
const ClassProject::BDD_ID AandB = 4;
const ClassProject::BDD_ID D = 5;
const ClassProject::BDD_ID A_OR_B = 6;
const ClassProject::BDD_ID C_AND_D = 7;
const ClassProject::BDD_ID Invalid = 9999;
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
};
//same labels are refered with same BDDID
TEST_F(ManagerTest, Createvar_SameIDforsameLabel)
{
    EXPECT_ANY_THROW(obj->createVar("a"));
};
//BDDID is different for non-identical labels and incremented
TEST_F(ManagerTest, Createvar_UniQueidforlabel) {
    const ClassProject::BDD_ID id2 = obj->createVar("b");
    const ClassProject::BDD_ID id3 = obj->createVar("a+b");
    EXPECT_NE(id2, id3);  // Ensure the IDs are different
    EXPECT_EQ( id3, id2+1); // Ensure the incrementation
};
//uniqueTableSize()
TEST_F(ManagerTest,uniqueTableSize)
{
    EXPECT_NE( obj->uniqueTableSize(),9999999999+1);// check for terminal case
    EXPECT_EQ( temp_obj->uniqueTableSize(), 2);// check for the terminal case
    EXPECT_EQ( obj->uniqueTableSize(),5 );// check for the correct size
};
//check deletetable
/*TEST_F(ManagerTest, table_deletion)
{
    obj->delete_table();
    EXPECT_EQ(obj->uniqueTableSize(), 2);
}*/
// end of delete table


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

//neg()_s
/*TEST_F(ManagerTest,negation_check)
{
    EXPECT_ANY_THROW( obj->neg(A));
}*/


//         virtual BDD_ID neg(BDD_ID a) override {  // S
//             return -1;
//         };

//         virtual BDD_ID nand2(BDD_ID a, BDD_ID b) override {  // S
//             return -1;
//         };

//         virtual BDD_ID nor2(BDD_ID a, BDD_ID b) override {  // S
//             return -1;
//         };

//         virtual BDD_ID xnor2(BDD_ID a, BDD_ID b) override {  // S
//             return -1;
//         };

//         virtual void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override {  // S
//         };

//         virtual void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override {  // S
//         };


//         virtual size_t uniqueTableSize() override {  // S
//             return -1;
//         };


//         virtual BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override {  // N
//             return -1;
//         };


// Throws Exception when calling ite with an invalid BDD_ID
// TEST_F(ManagerTest, iteException) {
//     ClassProject::BDD_ID Invalid_id = 9999;
//     EXPECT_ANY_THROW( obj->ite(Invalid_id , FALSE_ID , FALSE_ID));
// }

// // Terminal Case 1 Test
// TEST_F(ManagerTest, iteTerminal1) {
//     EXPECT_EQ( obj->ite(TRUE_ID , B , C) , B);
// }

// // Terminal Case 2 Test
// TEST_F(ManagerTest, iteTerminal2) {  
//     EXPECT_EQ( obj->ite(FALSE_ID , B , C) , C);
// }

// // Terminal Case 3 Test
// TEST_F(ManagerTest, iteTerminal3) {  
//     EXPECT_EQ( obj->ite(B , C , C) , C);
// }

// // Terminal Case 4 Test
// TEST_F(ManagerTest, iteTerminal4) {  
//     EXPECT_EQ( obj->ite(B , TRUE_ID , FALSE_ID) , B);
// }

// // Terminal Case 5 Test
// TEST_F(ManagerTest, iteTerminal5) {  
//     EXPECT_EQ( obj->ite(B , FALSE_ID , FALSE_ID) , FALSE_ID);
// }

// // Terminal Case 6 Test
// TEST_F(ManagerTest, iteTerminal6) {  
//     EXPECT_EQ( obj->ite(B , TRUE_ID , TRUE_ID) , TRUE_ID);
// }

// // A OR B Case 7 Test
// TEST_F(ManagerTest, iteAorB) {  
//     EXPECT_EQ( obj->ite(A , TRUE_ID , B) , A_OR_B);
// }

// // C and D Case 7 Test
// TEST_F(ManagerTest, iteCandD) {  
//     EXPECT_EQ( obj->ite(C , D , FALSE_ID) , C_AND_D);
// }



//         virtual BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override {  // N
//             return -1;
//         };

//         virtual BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override {  // N
//             return -1;
//         };

//         virtual BDD_ID coFactorTrue(BDD_ID f) override {  // N
//             return -1;
//         };

//         virtual BDD_ID coFactorFalse(BDD_ID f) override {  // N
//             return -1;
//         };

//         virtual BDD_ID and2(BDD_ID a, BDD_ID b) override {  // N
//             return -1;
//         };

//         virtual BDD_ID or2(BDD_ID a, BDD_ID b) override { // N
//             return -1;
//         };

//         virtual BDD_ID xor2(BDD_ID a, BDD_ID b) override { // N
//             return -1;
//         };


//         virtual std::string getTopVarName(const BDD_ID &root) override {  // N
//             return "-1";
//         };

//         virtual void visualizeBDD(std::string filepath, BDD_ID &root) override {
//         }; not included

