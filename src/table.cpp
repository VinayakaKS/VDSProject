#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory> // For smart pointers

// Row structure
struct TableRow {
    size_t id;                       // Unique identifier
    std::string label;            // Label for the row
    size_t high;                     // Points to another row's id
    size_t low;                      // Points to another row's id
    size_t topVar;                   // Points to another row's id

    TableRow(size_t id, const std::string& label, size_t high = -1, size_t low = -1, size_t topVar = -1)
        : id(id), label(label), high(high), low(low), topVar(topVar) {}
};

// Table class
class DynamicTable {
    std::vector<TableRow> rows;             // Stores rows
    std::unordered_map<int, size_t> idMap;  // Maps id to index in `rows` for fast lookup
    std::size_t last_id = 0;

    public:
        // Add a row to the table
        void addRow(const std::string& label, int high = -1, int low = -1, int topVar = -1) {
            if(topVar == -1) {
                topVar = last_id;
            }

            rows.emplace_back(last_id, label, high, low, topVar);
            idMap[last_id++] = rows.size() - 1;
        }

        // Get a row by id
        TableRow* getRowById(int id) {
            if (idMap.find(id) != idMap.end()) {
                return &rows[idMap[id]];
            }
            return nullptr;
        }

        // Display the table
        void displayTable() const {
            std::cout << "ID\tLabel\tHigh\tLow\tTopVar\n";
            for (const auto& row : rows) {
                std::cout << row.id << "\t" << row.label << "\t" << row.high << "\t" << row.low << "\t" << row.topVar << "\n";
            }
        }
};

