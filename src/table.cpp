#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> // For smart pointers

using namespace std;

const size_t LIMIT = 9999999999;
const size_t HIGH = 1;
const size_t LOW = 0;


struct CPTableRow{
    size_t id;
    size_t i;
    size_t t;
    size_t e;
    CPTableRow(size_t id = LIMIT, size_t i = LIMIT, size_t t = LIMIT, size_t e = LIMIT)
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
    vector<CPTableRow> UniqueTable;             // Stores rows in Unique table
    unordered_map<CPTkey,size_t, CPTHash> rowDataMap;

public:
    void addRowCPTable(CPTableRow *row_data) {
        // if(row_data->i == LIMIT) {
        //     row_data->i = HIGH;
        // }

        // if(row_data->t == LIMIT) {
        //     row_data->t = HIGH;
        // }

        // if(row_data->e == LIMIT) {
        //     row_data->e = LOW;
        // }

        // CPTableRow data = {row_data->id,row_data->i,row_data->t,row_data->e};
        UniqueTable.push_back(*row_data);
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
        for (const auto &row : UniqueTable) {
            std::cout << row.id << "\t\t"<< row.i << "\t" << row.t << "\t" << row.e << "\n";
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

    TableRow(size_t id = LIMIT, const string &label="", size_t high = LIMIT, size_t low = LIMIT, size_t topVar = LIMIT)
        : id(id), label(label), high(high), low(low), topVar(topVar) {}
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
    // unordered_map<size_t, size_t> idMap;             // Maps id to index in `UniqueTable` for fast lookup
    unordered_map<string, size_t> labelMap;          // Maps id to index in `UniqueTable` for fast lookup
    unordered_map<UTkey,size_t,UTHash> rowMap;

    public:
        size_t last_id = 0;
        // Add a row to the table
        size_t addRow(TableRow *row_data) {
            if(row_data->topVar == LIMIT) {
                row_data->topVar = last_id;
            }

            if(row_data->high == LIMIT) {
                row_data->high = HIGH;
            }

            if(row_data->low == LIMIT) {
                row_data->low = LOW;
            }

            TableRow data = {last_id, row_data->label,row_data->high,row_data->low,row_data->topVar};
            UniqueTable.push_back(data);
            // cout<<"Added row "<< last_id<<endl;
            // idMap[last_id++] = UniqueTable.size() - 1;
            last_id++;
            labelMap[row_data->label] = UniqueTable.size() - 1;
            rowMap[{row_data->high,row_data->low,row_data->topVar}] = UniqueTable.size() - 1;
            return (last_id - 1);
        }
    size_t addRow_Computed(TableRow *row_data) {
            if(row_data->topVar == LIMIT) {
                row_data->topVar = last_id;
            }

            if(row_data->high == LIMIT) {
                row_data->high = HIGH;
            }

            if(row_data->low == LIMIT) {
                row_data->low = LOW;
            }
            UniqueTable.push_back(*row_data);
            // cout<< "Row added - "<< row_data->id << " "<<row_data->label <<endl;
            // idMap[row_data->id] = UniqueTable.size() - 1;
            labelMap[row_data->label] = UniqueTable.size() - 1;
            return row_data->id;
        }
        // Get a row by id
        TableRow* getRowById(size_t id) {
            if(id < last_id) {
                return &UniqueTable[id];
            } else {
                return nullptr;
            }
            // if (idMap.find(id) != idMap.end()) {
            //     return &UniqueTable[idMap[id]];
            // }
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
            // idMap.clear();                // Clears the idMap
            labelMap.clear();             // Clears the labelMap
            last_id = 0;                  // Reset the last_id counter
            cout << "Table has been deleted." << endl;
        }
};

