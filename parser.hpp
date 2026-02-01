#pragma once
#include <vector>
#include "lexer.hpp"

struct Node {
    virtual ~Node() = default;//virtual ca sa se stearga calumea node urile
};
struct ExprNode:Node{
    virtual ~ExprNode()=default;

};

struct NumberNode:ExprNode {
    int value;                // the numeric value
    NumberNode(int v) : value(v) {}  ;// constructor to set the value
    /*
    echivalent cu:
    NumberNode(int v){
    value=v;
    }
    */
};
struct VariableNode:ExprNode{
    std::string name;
    VariableNode(const std::string& n) : name(n){};
};
struct UnaryNode:ExprNode {//asa e numele ca se refera la un operatr unu singur unary
    token_type op;      // operator pentru -
    ExprNode* child;  // se aplica la exrepresia asta 
    // Constructor: initializeaza child si op
    UnaryNode(token_type o, ExprNode* c) : op(o), child(c){};
};
struct BinaryNode:ExprNode{
    token_type op;
    ExprNode* left;
    ExprNode* right;
    BinaryNode(token_type o,ExprNode* a,ExprNode* b) : op(o),  left(a),right(b){};
};
struct DeclarNode:Node{
    VariableNode* var;
    ExprNode* expr;
    DeclarNode(VariableNode* v, ExprNode* e) : var(v), expr(e){}

};
struct LineNode:Node{
enum LineType{DECLAR,EXPR};
LineType type;
Node* line;
LineNode(LineType t, Node* s ) : type(t),line(s){};
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



