#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> // For smart pointers

using namespace std;
// Row structure
struct TableRow {
    size_t id;                       // Unique identifier
    string label;                    // Label for the row
    size_t high;                     // Points to the high node
    size_t low;                      // Points to low side node
    size_t topVar;                   // Top variable

    TableRow(size_t id, const string &label, size_t high, size_t low, size_t topVar)
        : id(id), label(label), high(high), low(low), topVar(topVar) {}
};

// Table class
class DynamicTable {
    vector<TableRow> UniqueTable;             // Stores rows in Unique table
    unordered_map<int, size_t> idMap;         // Maps id to index in `UniqueTable` for fast lookup
    size_t last_id = 0;

    public:
        // Add a row to the table 
        //TODO: size_t is unsigned long => -1 is not a correct implementation
        void addRow(TableRow *row_data) {
            row_data->topVar = last_id;
            TableRow data = {last_id, row_data->label,row_data->high,row_data->low,row_data->topVar};
            // UniqueTable.emplace_back(last_id, row_data->label, row_data->high, row_data->low, row_data->topVar);
            UniqueTable.push_back(data);
            idMap[last_id++] = UniqueTable.size() - 1;
        }

        // Get a row by id
        TableRow* getRowById(size_t id) {
            if (idMap.find(id) != idMap.end()) {
                return &UniqueTable[idMap[id]];
            }
            return nullptr;
        }

        // Display the table
        void displayTable() const {
            std::cout << "BDD_ID\tLabel\tHigh\tLow\tTopVar\n";
            for (const auto &row : UniqueTable) {
                std::cout << row.id << "\t" << row.label << "\t" << row.high << "\t" << row.low << "\t" << row.topVar << "\n";
            }
        }
};

