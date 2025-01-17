#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> // For smart pointers

using namespace std;

const size_t HIGH = 1;
const size_t LOW = 0;


struct CPTableRow{
    size_t id;
    size_t i;
    size_t t;
    size_t e;
    CPTableRow(size_t id , size_t i , size_t t , size_t e )
        : id(id),i(i), t(t), e(e) {}
};

struct CPTkey{
    size_t i;
    size_t t;
    size_t e;

    // Define equality operator - Avoids the collision
    bool operator==(const CPTkey& other) const {
        return i == other.i && t == other.t && e == other.e;
    }
};

// Define a hash function for CPTableRow
struct CPTHash {
    
    std::size_t operator()(const CPTkey& row) const {
        return std::hash<size_t>()(row.i) ^ 
               (std::hash<size_t>()(row.t) << 1) ^ 
               (std::hash<size_t>()(row.e) << 2);
    }
};



// Computed Table class
class DynTable{
    unordered_map<CPTkey,size_t, CPTHash> rowDataMap;

    public:
        void addRowCPTable(CPTableRow *row_data) {
            rowDataMap[{row_data->i,row_data->t,row_data->e}] = row_data->id;
        }

        size_t getCPRowByHash(CPTkey data) {
            auto it = rowDataMap.find(data);
            if ( it != rowDataMap.end()) {
                return it->second;
            }
            return -1;
        }

        // Display the table
        void displayTable() const {
            cout<<"\nComputed Table"<<endl;
            std::cout << endl << "BDD_ID\t\tI\tT\tE\n";
            for (const auto &row : rowDataMap) {
                std::cout << row.second << "\t\t"<< row.first.i << "\t" << row.first.t << "\t" << row.first.e << "\n";
            }
        }
};

// Row structure
struct TableRow {
    size_t id;                       // Unique identifier
    string label;                    // Label for the row
    size_t high;                     // Points to the high node
    size_t low;                      // Points to low side node
    size_t topVar;                   // Top variable

    TableRow(const string &label="", size_t high = HIGH, size_t low = LOW, size_t topVar = LOW )
        :id(LOW) , label(label), high(high), low(low), topVar(topVar) {}
};

struct UTkey{
    size_t high;
    size_t low;
    size_t topVar;

    // Define equality operator - Avoids the collision
    bool operator==(const UTkey& other) const {
        return high == other.high && low == other.low && topVar == other.topVar;
    }
};

// Define a hash function for CPTableRow
struct UTHash {
    
    std::size_t operator()(const UTkey& row) const {
        return std::hash<size_t>()(row.high) ^ 
               (std::hash<size_t>()(row.low) << 1) ^ 
               (std::hash<size_t>()(row.topVar) << 2);
    }
};

// Unique Table class
class DynamicTable {
    vector<TableRow> UniqueTable;                    // Stores rows in Unique table
    unordered_map<string, size_t> labelMap;          // Maps id to label in `UniqueTable` for fast lookup
    unordered_map<UTkey,size_t,UTHash> rowMap;

    public:
        size_t last_id = 0;
        // Add a row to the table
        size_t addRow(TableRow *row_data) {
            size_t new_id = last_id;

            if(row_data->topVar == LOW) {
                row_data->topVar = new_id;
            }

            row_data->id = new_id;
            UniqueTable.push_back(*row_data);
            labelMap[row_data->label] = new_id;
            rowMap[{row_data->high,row_data->low,row_data->topVar}] = new_id;
            last_id++;
            return new_id;
        }

        // Get a row by id
        TableRow* getRowById(size_t id) {
            if(id < last_id) {
                return &UniqueTable[id];
            } else {
                return nullptr;
            }
        }

        // Get a row by label
        TableRow* getRowByLabel(string label) {
            if (labelMap.find(label) != labelMap.end()) {
                return &UniqueTable[labelMap[label]];
            }
            return nullptr;
        }

        // Get a row by data
        size_t getRowByData(UTkey data) {
            auto it = rowMap.find(data);
            if(it != rowMap.end()){
                return it->second;
            }
            return -1;
        }

        // Display the table
        void displayTable() const {
            cout<<"\nUnique Table"<<endl;
            std::cout << endl << "BDD_ID\tLabel\tHigh\tLow\tTopVar\n";
            for (const auto &row : UniqueTable) {
                std::cout << row.id << "\t" << row.label << "\t" << row.high << "\t" << row.low << "\t" << row.topVar << "\n";
            }
        }

        // Display the table
        size_t tableSize() {
            return UniqueTable.size();
        }

        // Delete the table
        void deleteTable()
        {
            // Clear all data structures
            UniqueTable.clear();          // Clears the vector containing table rows
            labelMap.clear();             // Clears the labelMap
            last_id = 0;                  // Reset the last_id counter
            cout << "Table has been deleted." << endl;
        }
};

