#include <iostream>
#include <map>
#include "parser.hpp"
#include "test_parser.hpp"

extern std::map<node_type, std::string> node_map;

void print(parse_node* n, std::ostream* os, int* depth)
{
  ++(*depth);
  if (n != NULL)
  {
    for (int i {0}; i < *depth; ++i)
      *os << "  ";
    *os << "Type: " << node_map.at(n->type);
    for (auto& i : (n->tokens))
      *os << " Token: " << i->instance;
    *os << '\n';

    for (auto& i : (n->children))
    print(i, os, depth);
  }

  --(*depth);
}
