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

bool Parser::is_end(){
    if ( peek().type==END_OF_FILE)
        return true;
    return false;
}

token Parser::advance(){
    if(!is_end()){
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

Node* Parser::factor(){
    if(is_match(MINUS)){ //match si treci mai departe
        std::cout << "UNARY MINUS DETECTED\n";
        Node* child = factor(); // dai parse la urmatoru factor de vine
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
        Node* inside=expr();
        consume(RPAR);// OBLIG sa fie )
        return inside;
    }
    else{
        return nullptr;
        std::cout<<"\nceva nu merge bine \n";
    }
    

}


Node* Parser::term(){
    Node* left=factor();
    while( is_match(TIMES) || is_match(SLASH)){
        token op=previous();
        Node* right =factor();
        left=new BinaryNode(op.type ,left ,right);// am stat 2 ore sa imi dau seama caaveam oridnea gresita(left op right )
    }
    return left;
    }

Node* Parser::expr(){
    Node* left=term();
    while(is_match(PLUS) || is_match(MINUS)){
        token op=previous();
        Node* right= term();
        left= new BinaryNode( op.type , left , right);
    }
    return left;


    

}

