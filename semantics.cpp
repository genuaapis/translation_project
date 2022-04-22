#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "parser.hpp"
#include "semantics.hpp"

void traversal(parse_node* node, std::vector<std::string>& identifier_stack, std::vector<int>& var_count, bool& error_flag)
{
  if (node != NULL)
  {
    switch (node->type)
    {
      case node_type::block:
        var_count.push_back(0);
        for (auto& n : (node->children))
          traversal(n, identifier_stack, var_count, error_flag);
        while(var_count.back() != 0)
        {
          pop(identifier_stack);
          --(var_count.back());
        }
        var_count.pop_back();
        break;
      case node_type::vars:
        for (auto& t : (node->tokens))
        {
          int result {find((t->instance), identifier_stack)};
          if (result < 0)
          {
            push((t->instance), identifier_stack);
            ++(var_count.back());
          }
          else
          {
            std::cout << "Error: multiple definition of identifier '" << t->instance << "' line " << t->line_num << ".\n";
            error_flag = true;
          }
        }
      default:
        for (auto& t : (node->tokens))
        {
          if ((t->id) == token_ID::identifier)
          {
            int result {find((t->instance), identifier_stack)};
            if (result < 0)
            {
              std::cout << "Error: undefined identifier '" << t->instance << "' line " << t->line_num << ".\n";
              error_flag = true;
            }
          }
        }
        for (auto& n : (node->children))
          traversal(n, identifier_stack, var_count, error_flag);
    }
  }
}

void push(std::string s, std::vector<std::string>& identifier_stack)
{
  identifier_stack.push_back(s);
}

void pop(std::vector<std::string>& identifier_stack)
{
  identifier_stack.pop_back();
}

int find(std::string s, std::vector<std::string>& identifier_stack)
{
  int index {-1};
  for (int i {identifier_stack.size() - 1}; i >= 0; --i)
  {
    if (identifier_stack.at(i) == s)
      index = ((identifier_stack.size() - 1) - i);
  }
  return index;
}
