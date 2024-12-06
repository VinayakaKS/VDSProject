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
