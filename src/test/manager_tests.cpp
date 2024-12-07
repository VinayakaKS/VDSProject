//
// Created by tobias on 22.11.16
//

#include "Tests.h"

// Test fixture
class ManagerTest : public ::testing::Test {
protected:
    static ClassProject::Manager* obj;

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

const ClassProject::BDD_ID FASLE_ID = 0;
const ClassProject::BDD_ID TRUE_ID = 1;
const ClassProject::BDD_ID A = 2;
const ClassProject::BDD_ID B = 3;
const ClassProject::BDD_ID C = 4;
const ClassProject::BDD_ID D = 5;
const ClassProject::BDD_ID A_OR_B = 6;
const ClassProject::BDD_ID C_AND_D = 7;

// Example test case for the Manager module
TEST_F(ManagerTest, False) {
    EXPECT_EQ( obj->False(), FASLE_ID );
}

TEST_F(ManagerTest, TRUE) {
    EXPECT_EQ( obj->True(), TRUE_ID );
}

//         virtual BDD_ID createVar(const std::string &label) override {  // S
//             return -1;
//         };

//         virtual bool isConstant(BDD_ID f) override {  // S
//             return -1;
//         };

//         virtual bool isVariable(BDD_ID x) override {  // S
//             return -1;
//         };

//         virtual BDD_ID topVar(BDD_ID f) override {  // S
//             return -1;
//         };

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
TEST_F(ManagerTest, iteException) {
    ClassProject::BDD_ID Invalid_id = 9999;
    EXPECT_ANY_THROW( obj->ite(Invalid_id , FASLE_ID , FASLE_ID));
}

// Terminal Case 1 Test
TEST_F(ManagerTest, iteTerminal1) {
    EXPECT_EQ( obj->ite(TRUE_ID , B , C) , B);
}

// Terminal Case 2 Test
TEST_F(ManagerTest, iteTerminal2) {  
    EXPECT_EQ( obj->ite(FASLE_ID , B , C) , C);
}

// Terminal Case 3 Test
TEST_F(ManagerTest, iteTerminal3) {  
    EXPECT_EQ( obj->ite(B , C , C) , C);
}

// Terminal Case 4 Test
TEST_F(ManagerTest, iteTerminal4) {  
    EXPECT_EQ( obj->ite(B , TRUE_ID , FASLE_ID) , B);
}

// Terminal Case 5 Test
TEST_F(ManagerTest, iteTerminal5) {  
    EXPECT_EQ( obj->ite(B , FASLE_ID , FASLE_ID) , FASLE_ID);
}

// Terminal Case 6 Test
TEST_F(ManagerTest, iteTerminal6) {  
    EXPECT_EQ( obj->ite(B , TRUE_ID , TRUE_ID) , TRUE_ID);
}

// A OR B Case 7 Test
TEST_F(ManagerTest, iteAorB) {  
    EXPECT_EQ( obj->ite(A , TRUE_ID , B) , A_OR_B);
}

// C and D Case 7 Test
TEST_F(ManagerTest, iteCandD) {  
    EXPECT_EQ( obj->ite(C , D , FASLE_ID) , C_AND_D);
}



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
//         };
