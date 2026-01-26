#pragma once
#include <vector>
#include "lexer.hpp"

struct Node {
    virtual ~Node() = default;//virtual ca sa se stearga calumea node urile
};

struct NumberNode:Node {
    int value;                // the numeric value
    NumberNode(int v) : value(v) {}  ;// constructor to set the value
    /*
    echivalent cu:
    NumberNode(int v){
    value=v;
    }
    */
};
struct VariableNode:Node{
    std::string name;
    VariableNode(const std::string& n) : name(n){};
};
struct UnaryNode:Node {//asa e numele ca se refera la un operatr unu singur unary
    token_type op;      // operator pentru -
    Node* child;  // se aplica la exrepresia asta 
    // Constructor: initializeaza child si op
    UnaryNode(token_type o, Node* c) : op(o), child(c){};
};
struct BinaryNode:Node{
    token_type op;
    Node* left;
    Node* right;
    BinaryNode(token_type o,Node* a,Node* b) : op(o),  left(a),right(b){};
};


// clasa de parser a trebuit sa o mut sper sa mearga plsplsplspls
class Parser{
public:
    Parser(const std::vector<token>& tokens);
    void parse();
    Node* expr();
    Node* term();
    Node* factor();

private:
    std::vector<token> tokens;
    size_t current;

    token peek();
    token advance();
    bool is_match(token_type type);
    token previous();
    bool is_end();

    token consume(token_type type);

};



