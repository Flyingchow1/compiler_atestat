#include <iostream>
#include <string>
#include <cctype>   

#include "lexer.hpp"
//tes
Lexer::Lexer(const std::string& input) {
    text = input;
    pos = 0;
    if (text.empty())
        current = '\0';    // verific sa nu fie gol
    else
        current = text[pos];
}

void Lexer::advance(){//merg inainte
    pos++;
    if(text.size()>pos)
        current=text[pos];
    else
        current='\0';
}

void Lexer::skip_white_space(){// ignor spatiile
    while (current == ' ' || current == '\t' || current == '\n') {
        advance();
    }
}

token Lexer::number(){//verific daca e numar
    std::string num_str;
    while(isdigit(current)){
        num_str+=current;
        advance();
    }
    token result;
    result.type=NUM;
    result.value=num_str;
    return result;
}

token Lexer::identifier(){
    std::string id_str;
    if(isalpha(current))// teoretic nu e necesar pt ca chemama functia doar cand este o litera in fata
        while(isalpha(current) || isdigit(current)){
            id_str+=current;
            advance();
        }
    token result;
    result.type=ID;
    result.value=id_str;
    return result;
}

token Lexer::getToken(){
    skip_white_space();
    if(isalpha(current))
        return identifier();
    else if(isdigit(current))
            return number();
    else if(current=='+'){
        token result;
        result.type=PLUS;
        result.value = std::string(1, current);
        advance();
        return result;
    }
    else if(current=='-'){
        token result;
        result.type=MINUS;
        result.value = std::string(1, current);
        advance();
        return result;
    }
    else if(current=='('){
        token result;
        result.type=LPAR;
        result.value = std::string(1, current);
        advance();
        return result;
    }
    else if(current==')'){
        token result;
        result.type=RPAR;
        result.value = std::string(1, current);
        advance();
        return result;
    }
    else if(current=='/'){
        token result;
        result.type=SLASH;
        result.value = std::string(1, current);
        advance();
        return result;
    }
    else if(current=='\0'){
        token result;
        result.type=END_OF_FILE;
        result.value = "";
        return result;
    }
    else{
    std::cerr << "Lexer error: unexpected character '" << current << "'" << std::endl;
    advance();
    return getToken();
    }
}
std::string Lexer::token_to_string(token_type type){
    if(type == 0) return "NUM";
    else if(type == 1) return "ID";
    else if(type == 2) return "PLUS";
    else if(type == 3) return "MINUS";
    else if(type == 4) return "SLASH";
    else if(type == 5) return "LPAR";
    else if(type == 6) return "RPAR";
    else if(type == 7) return "EOF";
    else {
        std::cerr << "eroare???"<<std::endl;
        return "END_OF_FILE";
    }
}
