#ifndef SEMANTICS_HPP
#define SEMANTICS_HPP

#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "parser.hpp"

void traversal(std::ostream*, parse_node*, std::vector<std::string>&, std::vector<int>&, bool&);
void push(std::ostream*, std::string, std::vector<std::string>&);
void pop(std::ostream*, std::vector<std::string>&);
int find(std::ostream*, std::string, std::vector<std::string>&);
std::string new_var();
std::string new_label();

#endif
