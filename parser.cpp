#include <string>
#include <cctype>

#include "lexer.hpp"
#include "parser.hpp"

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
bool Parser::is_match(token_type type){
    if(peek().type==type) {
        advance();
        return true;
    }
    return false;
}
