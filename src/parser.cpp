#include <vector>
#include <stack>
#include <iostream>
#include <cctype>
#include <algorithm>
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

// A function that converts a token to its string equivalent.
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

// A lexer is something that tokenizes a given string input. Used mainly for arithemtical expressions and cell references.
class Lexer {
    private:
        string input;
        int pos;

        // A cell is identified by somehing like A1, B2, etc.
        bool is_cell(char c, char after) {
            return isalpha(c) && isdigit(after);
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
        Lexer(string input) : input(input), pos(0) {}

        // Yields the next token using the pos pointer
        Token getNextToken() {
            
            while (pos < input.size()) {
                char current_char = input[pos];
                
                if (isspace(current_char) || current_char == '=') {
                    // We skip whitespace and the equals sign (beginning of a formula)
                    pos++;
                    continue;
                } else if (is_cell(current_char, input[pos + 1])) {
                    // If we get a cell reference, we have to parse it into a Celltype Token
                    string cell = string(1, current_char) + string(1, input[pos + 1]);
                    return parseCell(cell);
                } else if (isDecimal(current_char)) {
                    return Token{TokenType::Decimal, parseDecimal(current_char)};
                } else if (isdigit(current_char)) {
                    return Token{TokenType::Number, parseNumber(current_char)};
                }

                // Operator logic
                switch (current_char) {
                    case '+': pos++; return Token{TokenType::Plus, "+"};
                    case '-': pos++; return Token{TokenType::Minus, "-"};
                    case '*': pos++; return Token{TokenType::Mult, "*"};
                    case '/': pos++; return Token{TokenType::Div, "/"};
                    case '(': pos++; return Token{TokenType::LeftParen, "("};
                    case ')': pos++; return Token{TokenType::RightParen, ")"};
                }
            }

            // End marker for usage in outside functions
            return Token{TokenType::End, ""};
        }
        string getInput() {
            return input;
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

// Parser implements the shunting yard algorithm to parse arithmetical expressions in RPN.
class Parser {
    private:
        vector<Token> tokens;
        stack<Token> operators;
        stack<Token> output;

        // Shunting yard precedences, higher numbers indicate higher precedence
        int precedence(Token t) {
            if (t.type == TokenType::Plus || t.type == TokenType::Minus) {
                return 1;
            } else if (t.type == TokenType::Mult || t.type == TokenType::Div) {
                return 2;
            }
            return 0;
        }
    public:
        Parser(vector<Token> tokens) : tokens(tokens) {}

        // Applies shunting yard algorithm, returning a stack of tokens in RPN.
        stack<Token> shunting_yard() {
            for (Token& token : tokens) {
                // We push all numbers to the output stack
                if (token.type == TokenType::Number || token.type == TokenType::Decimal) {
                    output.push(token);
                } else if(token.type == TokenType::LeftParen) {
                    operators.push(token);
                } else if (token.type == TokenType::RightParen) {
                    while (operators.top().type != TokenType::LeftParen && !operators.empty()) {
                        output.push(operators.top());
                        operators.pop();
                    }
                    operators.pop(); // Pop the left parenthesis
                } else if (token.type == TokenType::Plus || token.type == TokenType::Minus || token.type == TokenType::Mult || token.type == TokenType::Div) {
                    while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                        output.push(operators.top());
                        operators.pop();
                    }
                    operators.push(token);
                }

            }
            while (!operators.empty()) {
                output.push(operators.top());
                operators.pop();
            }

            // We return reversed output for easier evaluation in the future
            stack<Token> reversedOutput;
            while (!output.empty()) {
                reversedOutput.push(output.top());
                output.pop();
            }
            return reversedOutput;
        }
};

void print_stack(stack<Token> s) {
    vector<string> output;
    while (!s.empty()) {
        output.push_back(s.top().value + " ");
        s.pop();
    }
    reverse(output.begin(), output.end());
    for (int i = 0; i < output.size(); i++) {
        cout << output[i];
    }
}

double calculate (double value, double value2, string op) {
    if (op == "+") {
        return value + value2;
    } else if (op == "-") {
        return value - value2;
    } else if (op == "*") {
        return value * value2;
    } else if (op == "/") {
        return value / value2;
    }
}

bool isOperator(Token t) {
    return t.type == TokenType::Plus || t.type == TokenType::Minus || t.type == TokenType::Mult || t.type == TokenType::Div;
}

bool isLiteral(Token t) {
    return t.type == TokenType::Number || t.type == TokenType::Decimal;
}

// A function that evaluates a reverse polish notation expression
double evaluate_rpn(stack<Token> s) {

    stack<double> evaluationStack;

    while (!s.empty()) {
        Token token = s.top();
        s.pop();

        // If we get an operator, we want to apply it.
        if (isOperator(token)) {
            // Our operands are stored on the evaluation stack
            double value2 = evaluationStack.top();
            evaluationStack.pop();
            double value1 = evaluationStack.top();
            evaluationStack.pop();

            // Apply the operator
            double result = calculate(value1, value2, token.value);
            evaluationStack.push(result); // Will always be an operand

        } else if (isLiteral(token)) {    
            // Operands are pushed to the evaluation stack, evaluated when we encounter an operator.
            evaluationStack.push(stod(token.value));
        }
    }
    return evaluationStack.top();
}

int main() {
    Lexer lexer("3.2+(2-2.2)*6");
    Token token = lexer.getNextToken();
    vector<Token> tokens;
    while (token.type != TokenType::End) {
        tokens.push_back(token);
        token = lexer.getNextToken();
    }

    Parser parser(tokens);
    stack<Token> output = parser.shunting_yard();
    // print_stack(output);
    cout << lexer.getInput() << " = " << evaluate_rpn(output);
}