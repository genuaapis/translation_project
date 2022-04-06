#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "token.hpp"

enum class node_type
{
  program, block, vars, expr, n, z, a, m, r, stats, m_stat, stat, in, out,
  _if, loop, loop1, loop2, assign, ro, label, _goto, error
};

struct parse_node
{
  node_type type;
  std::vector<std::vector<token>::iterator> tokens;
  std::vector<parse_node*> children;
};

parse_node* parser(std::vector<token>&);
parse_node* program(std::vector<token>::iterator&);
parse_node* block(std::vector<token>::iterator&);
parse_node* vars(std::vector<token>::iterator&);
parse_node* expr(std::vector<token>::iterator&);
parse_node* n(std::vector<token>::iterator&);
parse_node* z(std::vector<token>::iterator&);
parse_node* a(std::vector<token>::iterator&);
parse_node* m(std::vector<token>::iterator&);
parse_node* r(std::vector<token>::iterator&);
parse_node* stats(std::vector<token>::iterator&);
parse_node* m_stat(std::vector<token>::iterator&);
parse_node* stat(std::vector<token>::iterator&);
parse_node* in(std::vector<token>::iterator&);
parse_node* out(std::vector<token>::iterator&);
parse_node* _if(std::vector<token>::iterator&);
parse_node* loop(std::vector<token>::iterator&);
parse_node* loop1(std::vector<token>::iterator&);
parse_node* loop2(std::vector<token>::iterator&);
parse_node* assign(std::vector<token>::iterator&);
parse_node* ro(std::vector<token>::iterator&);
parse_node* label(std::vector<token>::iterator&);
parse_node* _goto(std::vector<token>::iterator&);

#endif
