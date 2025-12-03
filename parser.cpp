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
