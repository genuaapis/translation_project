#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>
#include <iostream>
#include "token.hpp"

std::vector<std::string> filter(std::istream*);
std::vector<std::vector<int>> create_fsa();
std::map<char, int> create_column_map();
std::vector<token> scanner(std::istream*);
bool keyword_filter(std::vector<token>&, token, std::string);

#endif
