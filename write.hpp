#pragma once
#include "parser.hpp"
#include "lexer.hpp"
#include <fstream>




char tokenTypeToChar(token_type type);
void printTree(Node* node, int i);
void write_in_file(Node* node);
void write_in_file_recursive(Node* node);