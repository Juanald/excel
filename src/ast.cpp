#include <iostream>
#include <sstream>
using namespace std;

class ASTNode {
    public:
        virtual double evaluate() const = 0;
        virtual ~ASTNode() {} // Destructor
};

// A class that represents a node with just a number
class NumberNode : public ASTNode {
    private:
        int value;
    public:
        explicit NumberNode(int value) : value(value) {}
        double evaluate() const override {
            return (double) value;
        }
};

// An add node consists of a left and right node, as well as an operator
class AddNode : public ASTNode {
    private:
        ASTNode* left;
        ASTNode* right;
        char op;
    public:
        double evaluate() const override {
            if (op == '+') {
                return (double) left->evaluate() + (double) right->evaluate();
            } else if (op == '-') {
                return (double) left->evaluate() - (double) right->evaluate();
            } else {
                throw invalid_argument("Invalid operator!");
            }
        }

        ~AddNode() override {
            delete left;
            delete right;
        }
};

class MulNode : public ASTNode {
    private:
        ASTNode* left;
        ASTNode* right;
        char op;
    public:
        double evaluate() const override {
            if (op == '*') {
                return (double) left->evaluate() * (double) right->evaluate();
            } else if (op == '/') {
                return (double) left->evaluate() / (double) right->evaluate();
            } else {
                throw invalid_argument("Invalid operator!");
            }
        }

        ~MulNode() override {
            delete left;
            delete right;
        }
};

class Unarynode : public ASTNode {
    private:
        ASTNode* child;
        char op;
    public:
        double evaluate() const override {
            if (op == '-') {
                return -child->evaluate();
            } else {
                throw invalid_argument("Invalid unary operator!");
            }
        }
};
