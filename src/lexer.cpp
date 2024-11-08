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
    Decimal,
    End
};

// A token is something that has a value associated with a token type
struct Token {
    TokenType type;
    string value;
};

string tokenToString(Token t) {
    switch (t.type) {
        case TokenType::Number: return "Number";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Mult: return "Mult";
        case TokenType::Div: return "Div";
        case TokenType::Cell: return "Cell";
        case TokenType::LeftParen: return "LeftParen";
        case TokenType::RightParen: return "RightParen";
        case TokenType::Decimal: return "Decimal";
        case TokenType::End: return "End";
    }
}


// A lexer is something that tokenizes a given string input. We need the input, and a position pointer
class Lexer {
    private:
        string input;
        int pos;

        bool is_cell(char c, char after) {
            return isalpha(c) && isdigit(after); // We identify it by something like A1, B2, etc., where prev is A-Z, and c is 0-9.
        }

        Token parseCell(string cell) {
            pos += 2; // We have to skip the next character
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

        string parseDecimal(char current_char) {
            string decimal;
            while (isdigit(current_char) || current_char == '.') {
                decimal += current_char;
                pos++;
                current_char = input[pos];
            }
            return decimal;
        }

        bool isDecimal (char current_char) {
            string decimal;
            int pos_copy = pos;
            while (isdigit(current_char) || current_char == '.') {
                decimal += current_char;
                pos_copy++;
                current_char = input[pos_copy];
            }
            if (decimal.find('.') != std::string::npos) {
                return true;
            }
            return false;
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
                } else if (is_cell(current_char, input[pos + 1])) {
                    string cell = string(1, current_char) + string(1, input[pos + 1]); // Index error here, we should not try to access out of bounds
                    return parseCell(cell);
                } else if (isDecimal(current_char)) {
                    // If the current char is a digit, we have to construct a string of digits
                    return Token{TokenType::Decimal, parseDecimal(current_char)};
                } else if (isdigit(current_char)) {
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

void print_lexer() {
    Lexer lexer("3.2+2");
    Token token = lexer.getNextToken();
    vector<Token> tokens;
    while (token.type != TokenType::End) {
        cout << "Token type: " << tokenToString(token) << " Token value: " << token.value << endl;
        tokens.push_back(token);
        token = lexer.getNextToken();
    }
}

int main() {
    print_lexer();
}