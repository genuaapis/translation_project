#ifndef SEMANTICS_HPP
#define SEMANTICS_HPP

#include <vector>
#include <string>
#include "token.hpp"
#include "parser.hpp"

void traversal(parse_node*, std::vector<std::string>&, std::vector<int>&, bool&);
void push(std::string, std::vector<std::string>&);
void pop(std::vector<std::string>&);
int find(std::string, std::vector<std::string>&);

#endif
