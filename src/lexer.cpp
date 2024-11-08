#include <string>
#include <vector>
#include <iostream>
#include <cctype>
using namespace std;

enum class TokenType {
    Number,
    Plus,
    Minus,
    Mult,
    Div,
    Cell,
    LeftParen,
    RightParen,
    End
};

// A token is something that has a value associated with a token type
struct Token {
    TokenType type;
    string value;
};

// A lexer is something that tokenizes a given string input. We need the input, and a position pointer
class Lexer {
    private:
        string input;
        int pos;

        bool is_cell(char c, char prev) {
            return isdigit(c) && isalpha(prev); // We identify it by something like A1, B2, etc., where prev is A-Z, and c is 0-9.
        }

        Token parseCell(string cell) {
            return Token{TokenType::Cell, cell};
        }

        string parseNumber(char current_char) {
            string number;
            while (isdigit(current_char)) {
                number += current_char;
                pos++;
                current_char = input[pos];
            }
            return number;
        }

    public:
        Lexer(string input) : input(input), pos(0) {} // We start at 1 because the = operator underlies all formulas, so far we only want to parse formulas

        // We implement a getNextToken method
        Token getNextToken() {
            // We use a while loop to skip non-token characters
            while (pos < input.size()) {
                char current_char = input[pos];

                if (isspace(current_char)) {
                    pos++;
                    continue;
                } else if (current_char == '=') { // An equals sign indicates the beginning of a formula
                    pos++;
                    continue;
                // } else if (is_cell(current_char, input[pos - 1])) {
                //     string cell = string(1, current_char) + string(1, input[pos - 1]); // Index error here, we should not try to access out of bounds
                //     return parseCell(cell);
                } else if (isdigit(current_char)) {
                    // If the current char is a digit, we have to construct a string of digits
                    return Token{TokenType::Number, parseNumber(current_char)};
                }

                switch (current_char) {
                    case '+': pos++; return Token{TokenType::Plus, "+"};
                    case '-': pos++; return Token{TokenType::Minus, "-"};
                    case '*': pos++; return Token{TokenType::Mult, "*"};
                    case '/': pos++; return Token{TokenType::Div, "/"};
                    case '(': pos++; return Token{TokenType::LeftParen, "("};
                    case ')': pos++; return Token{TokenType::RightParen, ")"};
                }
            }
            return Token{TokenType::End, ""};
        }
};