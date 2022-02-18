#include <string>
#include <iostream>
#include <algorithm>
#include "node.hpp"
#include "tree.hpp"

node::node(std::string s)
{
  last_letter = s.back();
  word = s;
  l_node = nullptr;
  m_node = nullptr;
  r_node = nullptr;
}

void node::add_string(std::string s)
{
  if (s.back() == last_letter)
  {
    if (m_node == nullptr)
      m_node = new node(s);
    else
      m_node->add_string(s);
  }
  else if (s.back() < last_letter)
  {
    if (l_node == nullptr)
      l_node = new node(s);
    else
      l_node->add_string(s);
  }
  else if (s.back() > last_letter)
  {
    if (r_node == nullptr)
      r_node = new node(s);
    else
      r_node->add_string(s);
  }
}

node* build_tree(std::istream* is)
{
  node* tree = nullptr;
  std::string s;
  (*is) >> s;
  tree = new node(s);
  while (*is >> s)
    tree->add_string(s);

  return tree;
}

void in_order(node* n, std::ostream* os, int* depth)
{
  ++(*depth);

  if (n->l_node != nullptr)
    in_order(n->l_node, os, depth);

  for (int i {0}; i < *depth; ++i)
    *os << "  ";
  *os << n->last_letter << ':';
  *os << n->word;
  *os << '\n';

  if (n->m_node != nullptr)
    in_order(n->m_node, os, depth);

  if (n->r_node != nullptr)
    in_order(n->r_node, os, depth);

  --(*depth);
}

void pre_order(node* n, std::ostream* os, int* depth)
{
  ++(*depth);

  for (int i {0}; i < *depth; ++i)
    *os << "  ";
  *os << n->last_letter << ':';
  *os << n->word;
  *os << '\n';

  if (n->l_node != nullptr)
    pre_order(n->l_node, os, depth);

  if (n->m_node != nullptr)
    pre_order(n->m_node, os, depth);

  if (n->r_node != nullptr)
    pre_order(n->r_node, os, depth);

  --(*depth);
}

void post_order(node* n, std::ostream* os, int* depth)
{
  ++(*depth);

  if (n->l_node != nullptr)
    post_order(n->l_node, os, depth);

  if (n->m_node != nullptr)
    post_order(n->m_node, os, depth);

  if (n->r_node != nullptr)
    post_order(n->r_node, os, depth);

  for (int i {0}; i < *depth; ++i)
    *os << "  ";
  *os << n->last_letter << ':';
  *os << n->word;
  *os << '\n';

  --(*depth);
}
