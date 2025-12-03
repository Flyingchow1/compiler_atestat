#pragma once
#include <vector>
#include "lexer.hpp"

class Parser{
public:
    Parser(const std::vector<token>& tokens);
    void parse();

private:
    std::vector<token> tokens;
    size_t current;

    token peek();
    token advance();
    bool is_match(token_type type);
    bool is_end();
    void expr();
    void term();
    void factor();

};