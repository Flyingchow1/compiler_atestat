#include <iostream>
#include "parser.hpp"
#include "lexer.hpp"



// PRINTING PARSER
char tokenTypeToChar(token_type i) {
    switch(i) {
        case PLUS: return '+';
        case MINUS: return '-';
        case TIMES: return '*';
        case SLASH: return '/';
        default: return '?';
    }
}
void printTree(Node* node, int i = 0) {
    if (!node) return; // daca 

    std::string padding(i*4, '_'); // 4 spatii pe chestie
    if (auto n = dynamic_cast<NumberNode*>(node)) {     // dynamic cast verifica daca ceva (n) este un altceva <NumberNode>
        std::cout << padding <<"NUMBER"<< n->value << "\n";          //auto e ca si cum ai scrie NumberNode* n= dar e mai bun din ceva motiv
    }//                                                      ideea e ca facem asa : hei ce e asta?
    else if (auto v = dynamic_cast<VariableNode*>(node)) {//                       este varabila -> print spatii si varabila
        std::cout << padding <<"VARIABLE"<< v->name << "\n";//                                 daca este altceva decat id si num atunci print si 
    }//                                                                             creste padding u care este in incremente de 4
    else if (auto u = dynamic_cast<UnaryNode*>(node)) {
        std::cout << padding <<"UNARY"<< tokenTypeToChar(u->op) << "\n";
        printTree(u->child, i + 1);
    }
    else if (auto b = dynamic_cast<BinaryNode*>(node)) {
        std::cout << padding <<"BINARY"<< tokenTypeToChar(b->op) << "\n";
        printTree(b->left, i + 1);
        printTree(b->right, i + 1);
    }
}
/////////////////////////////////////////////////////////
                    //CONVERTING
void write_in_file(Node* node) {
    if (!node) return;

    if (auto n = dynamic_cast<NumberNode*>(node)) {  
    std::cout << "    mov rax, " << n->value << "\n";
    }
   /* else if (auto v = dynamic_cast<VariableNode*>(node)) {               
       
    }                                               
    else if (auto u = dynamic_cast<UnaryNode*>(node)) {
    
    }
    */
    else if (auto b = dynamic_cast<BinaryNode*>(node)) {
        write_in_file(b->left);
        std::cout << "    push rax\n";
        write_in_file(b->right);
        std::cout << "    pop rbx\n";//scoti din rbx

        // combini cele 2 
        switch (b->value) {
            case PLUS: 
                std::cout << "    add rax, rbx\n"; 
                break;
            case MINUS: 
                std::cout << "    sub rbx, rax\n"; 
                std::cout << "    mov rax, rbx\n"; 
                break;
            case TIMES: 
                std::cout << "    imul rax, rbx\n"; 
                break;
            case SLASH: 
                std::cout << "    mov rdx, 0\n";    // clear RDX for division
                std::cout << "    mov rcx, rax\n";  // divisor
                std::cout << "    mov rax, rbx\n";  // dividend
                std::cout << "    idiv rcx\n";     // RAX = RAX / RCX
                break;
        }
    }
}



/////////////////////////////////////////////////////////

int main() {


    std::string test_lexer;
    std::getline(std::cin, test_lexer); // citesc toata linia
    Lexer lex(test_lexer);
    std::vector<token> tokens;
        std::cout<< "TEST LEXER\n";
    //int i=0;
std::cout<<"test\n";
    token t;
    do {
        t = lex.getToken();

        std::cout<<lex.token_to_string(t.type)<<"        "<<t.value <<std::endl;
        tokens.push_back(t);//tokens[i++]=t;

    } while(t.type != END_OF_FILE);



    std::cout<<"\n\n\n\n\n\n    PARSER TEST:\n";
    Parser parser(tokens);
    Node* root=parser.expr();
    printTree(root);
    
    
    std::cout<<"\n\n\n";
    std::cout<<"FILE TEST\n\n";
    write_in_file(root);
   
   

}
