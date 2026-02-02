#include <iostream>
#include "parser.hpp"
#include "lexer.hpp"
#include "write.hpp"



int main() {


    std::string test_lexer;
    std::getline(std::cin, test_lexer); // citesc toata linia
    Lexer lex(test_lexer);
    std::vector<token> tokens;
    std::cout<< "TEST LEXER\n";//    TEMP
std::cout<<"test\n";// TEMP
    token t;
    do {
        t = lex.getToken();

        std::cout<<lex.token_to_string(t.type)<<"        "<<t.value <<std::endl;      //TEMP 
        tokens.push_back(t);//tokens[i++]=t;

    } while(t.type != END_OF_FILE);


    //VERIFIC PARSE TREE U
    std::cout<<"\n\n\n\n\n\n    PARSER TEST:\n";       // TEMP
    Parser parser(tokens);
    ProgramNode* root=parser.program();
    printTree(root,0);
    
    //SCRIU IN FISIER/'CONSOLA ASEMBLY
    std::cout<<"\n\n\n";
    std::cout<<"FILE TEST\n\n";    
    

    //write_in_file(root);
    

}
