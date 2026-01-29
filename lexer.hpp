#include <string>
#include <cctype>
#include <iostream>
#pragma once
enum token_type{
    NUM,
    ID,
    PLUS,
    MINUS,
    SLASH,
    LPAR,
    RPAR,
    TIMES,
    END_OF_FILE,
    END_OF_LINE
};
struct token {
    token_type type;
    std::string value;
};
class Lexer {
public:
    Lexer(const std::string& text);
    token getToken();
    std::string token_to_string(token_type type);
private:
    std::string text;
    size_t pos;
    char current;
    void advance();
    void skip_white_space();
    token number();
    token identifier();
};