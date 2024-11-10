#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <typeinfo>
#include "parser.cpp"
using namespace std;

class Cell {
    private:
        string value; // Can be an integer or a string
    public:
        Cell(string value) : value(value) {
            // cout << "Cell created! Value: " << value << endl;
        }

        friend ostream& operator<<(ostream& os, const Cell& c) {
            os << c.value;
            return os;
        }

        string get_value() {
            return value;
        }

        string set_value(string new_value) {
            value = new_value;
            return value;
        }
};

// A table is composed of rows and columns. Each value should be a cell
class Table {
    private:
        vector<vector<Cell>> table; // The table has to be accessible by column(A-Z) row (1-10) combinations. When you index A1, you take the value at index 0 of row 0.

        // A-Z
        int get_column_index(char col) {
            return col - 'A';
        }

    public:
        Table() {
            cout << "Table created!" << endl;
        }

        void add_row(vector<Cell> row) {
            table.push_back(row);
        }


        // We assume the table is mxn
        void print_table() {
            for (size_t i = 0; i < table.size(); i++) {
                for (size_t j = 0; j < table[i].size(); j++) {
                    string padded_val = table[i][j].get_value();

                    // We pad the values for table alignment
                    while (padded_val.size() < 20) {
                        padded_val += " ";
                    }
                    cout << padded_val << " | ";
                }
                cout << endl;
            }
        }

        // Return a cell reference at a given row and column (A1, B2, etc)
        Cell& get_cell(int row, char col) {
            col = get_column_index(col);
            row -= 1; // 0-indexed
            if ((size_t) row >= table.size() || row < 0 || (size_t) col >= table[row].size() || col < 0) {
                throw("Invalid cell reference!"); // i have no idea what this does
            }
            return table[row][col];
        }
};

class FileReader {
    private:
        vector<string> file_data;
        ifstream file;

        // Split a string by a delimiter
        vector<string> split_string(const string& s, char delimiter) {
            vector<string> tokens;
            string token;
            stringstream ss(s);
            while (getline(ss, token, delimiter)) {
                tokens.push_back(token);
            }
            return tokens;
        }

    public:
        FileReader(string file_name) : file(file_name) {
            if (!file.is_open()) {
                cout << "File failed to open!" << endl;
            }
        }

        void read_file() {
            string line;
            while (getline(file, line) && !line.empty()) {
                file_data.push_back(line);
            }
        }

        void print_file() {
            for (size_t i = 0; i < file_data.size(); i++) {
                cout << "Line " << i + 1 << ": " << file_data[i] << endl;
            }
        }

        Table create_table() {
            Table t;

            for (size_t i = 0; i < file_data.size(); i++) {
                vector<Cell> row;
                // We need to split at the comma delimeter
                vector<string> delimeted_string = split_string(file_data[i], ',');

                // Create cell objects from each csv value
                for (const string& s : delimeted_string) {
                    if (s[0] == '=') {
                        // We have a formula of type int(op)int(op)int.... Note, int can be references in the table.

                    } else {
                        // We have a regular value
                        Cell c(s);
                        row.push_back(c);
                    }
                }
                t.add_row(row);
            }
            return t;
        }
};

// A function that replaces cell references with their respective values
void parse_cells(vector<Token>& tokens, Table& table) {
    for (Token& token : tokens) {
        if (token.type == TokenType::Cell) {
            try
            {
                string cell_reference = token.value; // Each cell is A1 or B2, etc
                Cell c = table.get_cell(cell_reference[1] - '0', cell_reference[0]); // Convert row to an integer
                token.value = c.get_value();
                if (token.value.find('.') != string::npos) {
                    token.type = TokenType::Decimal;
                } else {
                    token.type = TokenType::Number;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }
    }
}

int main() {
    FileReader reader("data.csv");
    reader.read_file();
    Table table = reader.create_table();
    table.print_table();
    // cout << t.get_cell(1, 'C');

    Lexer lexer("B2 + B3");
    vector<Token> tokens = lexer.tokenize();

    // We need to go over each token, if it is a cell, replace it with the respective table value
    parse_cells(tokens, table);
    Parser parser(tokens);
    stack<Token> output = parser.shunting_yard();
    // print_stack(output);
    cout << lexer.getInput() << " = " << evaluate_rpn(output);
}