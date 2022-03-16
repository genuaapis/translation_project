#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "token.hpp"

enum class node_type
{
  program, block, vars, expr, n, a, m, r, stats, m_stat, stat, in, out,
  _if, loop1, loop2, assign, ro, label, _goto, error
};

struct parse_node
{
  node_type type;
  std::vector<token*> tokens;
  std::vector<parse_node*> children;
}

parse_node* _goto(std::vector<token>::iterator&);

#endif
