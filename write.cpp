#include <iostream>
#include "parser.hpp"
#include "lexer.hpp"
#include "write.hpp"

std::ofstream file_output ("test.asm");

char tokenTypeToChar(token_type i) {
    switch(i) {
        case PLUS: return '+';
        case MINUS: return '-';
        case TIMES: return '*';
        case SLASH: return '/';
        case EQUAL: return '=';
        
        default: return '?';
    }
}
/*
void printTree(Node* node, int i) {
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
}*/
void printTree(Node* node, int depth = 0) {
    if (!node) return;
    std::string padding(depth * 4, '_'); // 4 underscores per depth level
    if (auto n = dynamic_cast<NumberNode*>(node)) {
        std::cout << padding << "NUMBER " << n->value << "\n";
    }
    else if (auto v = dynamic_cast<VariableNode*>(node)) {
        std::cout << padding << "VARIABLE " << v->name << "\n";
    }
    else if (auto u = dynamic_cast<UnaryNode*>(node)) {
        std::cout << padding << "UNARY " << tokenTypeToChar(u->op) << "\n";
        printTree(u->child, depth + 1);
    }
    else if (auto b = dynamic_cast<BinaryNode*>(node)) {
        std::cout << padding << "BINARY " << tokenTypeToChar(b->op) << "\n";
        printTree(b->left, depth + 1);
        printTree(b->right, depth + 1);
    }
    else if (auto d = dynamic_cast<DeclarNode*>(node)) {
        std::cout << padding << "DECLARATION\n";
        printTree(d->var, depth + 1);
        printTree(d->expr, depth + 1);
    }
    else if (auto e = dynamic_cast<EchoNode*>(node)) {
        std::cout << padding << "ECHO\n";
        printTree(e->expr, depth + 1);
    }
    else if (auto l = dynamic_cast<LineNode*>(node)) {
        std::cout << padding << "LINE " 
                  << (l->type == DECLAR ? "DECLAR" : "ECHO") 
                  << "\n";
        printTree(l->line, depth + 1);
    }
    else if (auto p = dynamic_cast<ProgramNode*>(node)) {
        std::cout << padding << "PROGRAM\n";
        for (auto line : p->lines)
            printTree(line, depth + 1);
    }
    else {
        std::cout << padding << "UNKNOWN NODE\n";
    }
}


/////////////////////////////////////////////////////       T        E      S       T       E       //////////////////////////////////////////////


void write_in_file_recursive(Node* node) {
    if (!node) return;

    if (auto n = dynamic_cast<NumberNode*>(node)) {  
    std::cout << "    mov rax, " << n->value << "\n";
    file_output << "    mov rax, " << n->value << "\n";
    }
   /* else if (auto v = dynamic_cast<VariableNode*>(node)) {               
       
    }                                               
    else if (auto u = dynamic_cast<UnaryNode*>(node)) {
    
    }
    */
    else if (auto b = dynamic_cast<BinaryNode*>(node)) {
        
        write_in_file_recursive(b->left);
        std::cout << "    push rax\n";
        write_in_file_recursive(b->right);
        std::cout << "    pop rbx\n";//scoti din rbx

        // combini cele 2 
        switch (b->op) {
            case PLUS: 
                std::cout << "    add rax, rbx\n"; 
                file_output << "    add rax, rbx\n"; 
                break;
            case MINUS: 
                std::cout << "    sub rbx, rax\n"; 
                std::cout << "    mov rax, rbx\n"; 
                file_output<< "    sub rbx, rax\n"; 
                file_output<< "    mov rax, rbx\n"; 
                break;
            case TIMES: 
                std::cout << "    imul rax, rbx\n"; 
                file_output << "    imul rax, rbx\n";
                break;
            case SLASH: 
                std::cout << "    mov rdx, 0\n";    // clear RDX for division
                std::cout << "    mov rcx, rax\n";  // divisor
                std::cout << "    mov rax, rbx\n";  // dividend
                std::cout << "    idiv rcx\n";     // RAX = RAX / RCX
                file_output << "    mov rdx, 0\n";    // clear RDX for division
                file_output<< "    mov rcx, rax\n";  // divisor
                file_output << "    mov rax, rbx\n";  // dividend
                file_output << "    idiv rcx\n";     // RAX = RAX / RCX
                break;
        }
    }
}
void write_in_file(Node* node){
    


    file_output<<"section .text\n"<<"global _start\n\n"<<"_start:\n";
    write_in_file_recursive(node);
    file_output<<"mov rdi, rax\n"<<"mov rax, 60\n"<<"syscall";

}
