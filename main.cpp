#include <iostream>

#include "lexer.hpp"


int main() {
    std::string test_lexer;
    std::getline(std::cin, test_lexer); // citesc toata linia
    Lexer lex(test_lexer);

    token t;
    do {
        t = lex.getToken();
        std::cout<<lex.token_to_string(t.type)<<"        "<<t.value <<std::endl;
    } while(t.type != END_OF_FILE);
}
