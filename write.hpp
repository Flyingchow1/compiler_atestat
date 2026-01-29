#pragma once
#include "parser.hpp"
#include "lexer.hpp"


char tokenTypeToChar(token_type type);
void printTree(Node* node, int i);
void write_in_file(Node* node);