#include <iostream>
#include "parser.hpp"
#include "lexer.hpp"
#include "write.hpp"
#include "unordered_map"

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

std::unordered_map<std::string, int> variables;
int stack_offset = 0; 
void gen_expr(ExprNode* node) {
    if (!node) return;

    if (auto n = dynamic_cast<NumberNode*>(node)) {
        file_output << "    mov rax, " << n->value << "\n";
    }
    else if (auto v = dynamic_cast<VariableNode*>(node)) {
        if (!variables.count(v->name)) {
            std::cerr << "Undefined variable: " << v->name << "\n";
            exit(1);
        }
        file_output << "    mov rax, [rbp-" << variables[v->name] << "]\n";
    }
    else if (auto u = dynamic_cast<UnaryNode*>(node)) {
        gen_expr(u->child);

        if (u->op == MINUS) {
            file_output << "    neg rax\n";
        }
    }
    else if (auto b = dynamic_cast<BinaryNode*>(node)) {
        gen_expr(b->left);
        file_output << "    push rax\n";

        gen_expr(b->right);
        file_output << "    pop rbx\n";

        switch (b->op) {
            case PLUS:
                file_output << "    add rax, rbx\n";
                break;
            case MINUS:
                file_output << "    sub rbx, rax\n";
                file_output << "    mov rax, rbx\n";
                break;
            case TIMES:
                file_output << "    imul rax, rbx\n";
                break;
            case SLASH:
                file_output << "    mov rdx, 0\n";
                file_output << "    mov rcx, rax\n";
                file_output << "    mov rax, rbx\n";
                file_output << "    idiv rcx\n";
                break;
        }
    }
}
void gen_stmt(LineNode* line) {
    if (!line) return;

    if (line->type == DECLAR) {
        auto d = dynamic_cast<DeclarNode*>(line->line);

        gen_expr(d->expr);

        stack_offset += 8;
        variables[d->var->name] = stack_offset;

        file_output << "    mov [rbp-" << stack_offset << "], rax\n";
    }
    else if (line->type == ECHO1) {
        auto e = dynamic_cast<EchoNode*>(line->line);

        gen_expr(e->expr);
        

        file_output << "    call print_int\n";
    }
}

void gen_program(ProgramNode* prog) {
file_output << "section .bss\n";
file_output << "buffer resb 32\n\n";

file_output << "section .data\n";
file_output << "minus_sign db '-'\n\n"; // used to print negative numbers

file_output << "section .text\n";
file_output << "global _start\n\n";

// ===== print_int =====
file_output << "print_int:\n";
file_output << "    cmp rax, 0\n";            // check if negative
file_output << "    jge .positive\n";        // jump if >= 0

file_output << "    mov rdx, 1\n";           // print '-' for negative numbers
file_output << "    mov rsi, minus_sign\n";
file_output << "    mov rdi, 1\n";           // stdout
file_output << "    mov rax, 1\n";           // syscall write
file_output << "    syscall\n";

file_output << "    neg rax\n";              // convert to positive

file_output << ".positive:\n";
file_output << "    mov rcx, buffer\n";
file_output << "    add rcx, 31\n";
file_output << "    mov byte [rcx], 10\n";   // newline
file_output << "    dec rcx\n";
file_output << "    mov rbx, 10\n";

file_output << ".convert:\n";
file_output << "    xor rdx, rdx\n";
file_output << "    div rbx\n";
file_output << "    add dl, '0'\n";
file_output << "    mov [rcx], dl\n";
file_output << "    dec rcx\n";
file_output << "    test rax, rax\n";
file_output << "    jnz .convert\n";

file_output << "    inc rcx\n";
file_output << "    mov rax, 1\n";
file_output << "    mov rdi, 1\n";            // stdout
file_output << "    mov rsi, rcx\n";          // buffer start
file_output << "    mov rdx, buffer\n";       // compute length
file_output << "    add rdx, 32\n";
file_output << "    sub rdx, rcx\n";
file_output << "    syscall\n";
file_output << "    ret\n\n";
    // ===== _start =====
file_output << "_start:\n";
file_output << "    push rbp\n";
file_output << "    mov rbp, rsp\n";
file_output << "    sub rsp, 1024\n";  

    for (LineNode* line : prog->lines) {
        gen_stmt(line);
    }

    // ===== exit =====
file_output << "    mov rax, 60\n";
file_output << "    xor rdi, rdi\n";
file_output << "    syscall\n";
}
void write_in_file(Node* node) {
    if (auto p = dynamic_cast<ProgramNode*>(node)) {
        gen_program(p);
    }
}
















/*
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
    /
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
*/