#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> // For smart pointers

using namespace std;

const size_t LIMIT = 9999999999;
const size_t HIGH = 1;
const size_t LOW = 0;

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

// Table class
class DynamicTable {
    vector<TableRow> UniqueTable;             // Stores rows in Unique table
    unordered_map<size_t, size_t> idMap;         // Maps id to index in `UniqueTable` for fast lookup
    unordered_map<string, size_t> labelMap;         // Maps id to index in `UniqueTable` for fast lookup

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
            cout<< "Row added - "<< last_id<< " "<<row_data->label <<endl;
            idMap[last_id++] = UniqueTable.size() - 1;
            labelMap[row_data->label] = UniqueTable.size() - 1;
            return (last_id - 1);
        }

        // Get a row by id
        TableRow* getRowById(size_t id) {
            if (idMap.find(id) != idMap.end()) {
                return &UniqueTable[idMap[id]];
            }
            return nullptr;
        }

        // Get a row by label
        TableRow* getRowByLabel(string label) {
            if (labelMap.find(label) != labelMap.end()) {
                return &UniqueTable[labelMap[label]];
            }
            return nullptr;
        }

        // Get a row by data
        TableRow* getRowByData(size_t high , size_t low , size_t topVar) {
            for (const auto& row : UniqueTable) {
                if (row.high == high && row.low == low && row.topVar == topVar) {
                    // Found the row, use it
                    return &UniqueTable[row.id];
                }
            }
            
            return nullptr;
        }


        // Display the table
        void displayTable() const {
            std::cout << endl << "BDD_ID\tLabel\t\t\tHigh\tLow\tTopVar\n";
            for (const auto &row : UniqueTable) {
                std::cout << row.id << "\t" << row.label << "\t\t\t" << row.high << "\t" << row.low << "\t" << row.topVar << "\n";
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
            idMap.clear();                // Clears the idMap
            labelMap.clear();             // Clears the labelMap
            last_id = 0;                  // Reset the last_id counter
            cout << "Table has been deleted." << endl;
        }
};

