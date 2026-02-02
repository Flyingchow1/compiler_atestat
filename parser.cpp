#include <string>
#include <cctype>

#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>


Parser::Parser(const std::vector<token>& tokens){
    current=0;
    this->tokens=tokens;

}

token Parser::peek(){
    return tokens[current];
}

bool Parser::is_end_of_file(){
    if ( peek().type==END_OF_FILE)
        return true;
    return false;
}
bool Parser::is_end_of_line(){
    if ( peek().type==END_OF_LINE)
        return true;
    return false;
}

token Parser::advance(){
    if(!is_end_of_file()){
        token old_token=tokens[current];
        current++;
        return old_token;
    }
    return tokens[current];
}
bool Parser::is_match(token_type type){// IS MATCH SI AVANSEAZA SINGUR
    if(peek().type==type) {
        advance();
        return true;
    }
    return false;
}
bool Parser::check(token_type type){
    if(peek().type==type){
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////
                    //ERORI
std::string token_to_string(token_type type){
    if(type == 0) return "NUM";
    else if(type == 1) return "ID";
    else if(type == 2) return "PLUS";
    else if(type == 3) return "MINUS";
    else if(type == 4) return "SLASH";
    else if(type == 5) return "LPAR";
    else if(type == 6) return "RPAR";
    else if(type == 7) return "TIMES";
    else if(type == 8) return "EOF";
    else {
        std::cerr << "eroare???"<<type<<std::endl;
        return "END_OF_FILE";
    }
}

///////////////////////////////////////////////////////////
token Parser::consume(token_type type) {
    if (tokens[current].type == type) {
        return advance();   // move to next token
    }
    else{
        std::cout<<"  EROARE CONSUME\n EXPECTED "<< token_to_string(type)<<"\n";
        exit(1);//TEMPORARY;
    }
};
token Parser::previous(){
    return tokens[current-1];
}


/*
program → line* EOF

line → declaration EOL| expr EOL
declaration → IDENT "=" expr   // decided by lookahead in parser
expr   → term ((+ | -) term)*
term   → factor ((* | /) factor)*

factor → NUMBER| IDENT| "(" expr ")"| "-" factor
*/

///ACTUAL PARSING

ProgramNode* Parser::program() {
    ProgramNode* prog = new ProgramNode();

    while (!is_end_of_file()) {
        if (is_match(ID)) {
            // parse a declaration
            DeclarNode* decl = declar();
            LineNode* ln = new LineNode(DECLAR, decl);
            prog->lines.push_back(ln);
        }
        else if (is_match(ECHO)) {
            EchoNode* node = echo(); 
            LineNode* ln = new LineNode(ECHO1, node);
            prog->lines.push_back(ln);
        }
        else if (is_match(END_OF_LINE)){

        }
        else {
            std::cout << "EROARE PROGRAM()\n";
            std::cout<< "poate ai uitat punct si virgua ;\n";
            std::cout<<"current este in program       "<<current<<"        "<<tokens[current].type<<"\n";
            return nullptr;

            //advance();
        }
    }
    return prog;
};
DeclarNode* Parser::declar(){
    token tok =previous();
    std::string value=tok.value;
    VariableNode* var = new VariableNode(value);
    if(is_match(EQUAL)){
        //pt ca cand am scris expr() am acolo un  rpevious
        //std::cout<<"current este in declar parser inainte de expr      "<<current<<"        "<<tokens[current].type<<"\n";
        ExprNode* equal2 = expr();
        //std::cout<<"current este in declar parser dupa expr funct     "<<current<<"        "<<tokens[current].type<<"\n";
        return new DeclarNode(var,equal2);
    }
    else {
        //std::cout<<"current este in declar parser dupa expr funct     "<<current<<"        "<<tokens[current].type<<"\n";
        std::cout<<"expected =, eroare declaratie \n";
        return nullptr;
    }
};
EchoNode* Parser::echo(){
    ExprNode* node=expr();
    return new EchoNode(node);
};


ExprNode* Parser::factor(){
    if(is_match(MINUS)){ //match si treci mai departe
        std::cout << "UNARY MINUS DETECTED\n";
        ExprNode* child = factor(); // dai parse la urmatoru factor de vine
        return new UnaryNode(MINUS,child); //il transformi in UnaryNode 
    }
    else if(is_match(NUM)){
        token tok = previous();  // previous ( ar fi o idee sa fac o fucntie previous)
        int value = std::stoi(tok.value); //transfom  string to  int
        return new NumberNode(value);
    }
    else if(is_match(ID)){
        token tok=previous();
        std::string value=tok.value;
        return new VariableNode(value);
    }
    else if(is_match(LPAR)){
        ExprNode* inside=expr();
        consume(RPAR);// OBLIG sa fie )
        return inside;
    }
    else{
        return nullptr;
        std::cout<<"\nceva nu merge bine \n";
    }
    

}


ExprNode* Parser::term(){
    ExprNode* left=factor();
    while( is_match(TIMES) || is_match(SLASH)){
        token op=previous();
        ExprNode* right =factor();
        left=new BinaryNode(op.type ,left ,right);// am stat 2 ore sa imi dau seama caaveam oridnea gresita(left op right )
    }
    return left;
    }

ExprNode* Parser::expr(){
    ExprNode* left=term();
    while(is_match(PLUS) || is_match(MINUS)){
        token op=previous();
        ExprNode* right= term();
        left= new BinaryNode( op.type , left , right);
    }
    return left;
}



