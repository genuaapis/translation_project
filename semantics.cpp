#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "parser.hpp"
#include "semantics.hpp"

int num_temp_vars {0},
    num_labels {0};
bool brpos;

void traversal(std::ostream* os, parse_node* node, std::vector<std::string>& identifier_stack, std::vector<int>& var_count, bool& error_flag)
{
  if (node != NULL)
  {
    for (auto& t : (node->tokens))
    {
      if ((t->id) == token_ID::identifier)
      {
        int result {find(os, (t->instance), identifier_stack)};
        if ((result < 0) && (node->type != node_type::vars))
        {
          std::cout << "Error: undefined identifier '" << t->instance << "' line " << t->line_num << ".\n";
          error_flag = true;
        }
      }
    }

    switch (node->type)
    {
      case node_type::program:
      {
        for (auto& n : node->children)
          traversal(os, n, identifier_stack, var_count, error_flag);
        *os << "STOP\n";
        for (int i {0}; i <= num_temp_vars; ++i)
          *os << "T" << i << " 0\n";
        for (int i {0}; i <= num_labels; ++i)
          *os << "L" << i << " 0\n";
        break;
      }
      case node_type::block:
      {
        var_count.push_back(0);
        for (auto& n : (node->children))
          traversal(os, n, identifier_stack, var_count, error_flag);
        while(var_count.back() != 0)
        {
          pop(os, identifier_stack);
          --(var_count.back());
        }
        var_count.pop_back();
        break;
      }
      case node_type::vars:
      {
        for (auto& t : (node->tokens))
        {
          int result {find(os, (t->instance), identifier_stack)};
          if (result < 0)
          {
            push(os, (t->instance), identifier_stack);
            ++(var_count.back());
          }
          else
          {
            std::cout << "Error: multiple definition of identifier '" << t->instance << "' line " << t->line_num << ".\n";
            error_flag = true;
          }
        }
        for (auto& n : (node->children))
          traversal(os, n, identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::expr:
      {
        if ((node->children).back()->type == node_type::expr)
        {
          traversal(os, (node->children).back(), identifier_stack, var_count, error_flag);
          std::string temp_var = new_var();
          *os << "STORE " << temp_var << "\n";
          traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
          *os << "SUB " << temp_var << "\n";
        }
        else
          traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::n:
      {
        for (auto& n : node->children)
          traversal(os, n, identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::z:
      {
        if (((node->children).size() > 1) )//(node->children).back()->type == node_type::z)
        {
          traversal(os, (node->children).back(), identifier_stack, var_count, error_flag);
          std::string temp_var = new_var();
          *os << "STORE " << temp_var << "\n";
          traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
          if ((node->tokens).front()->instance == "/")
            *os << "DIV " << temp_var << "\n";
          else if ((node->tokens).front()->instance == "+")
            *os << "ADD " << temp_var << "\n";
        }
        else
          traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::a:
      {

        traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        if ((node->children).back()->type == node_type::a)
        {
          std::string temp_var = new_var();
          *os << "STORE " << temp_var << "\n";
          traversal(os, (node->children).back(), identifier_stack, var_count, error_flag);
          *os << "MULT " << temp_var << "\n";
        }
        break;
      }
      case node_type::m:
      {
        traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        if ((node->children).front()->type == node_type::m)
          *os << "MULT -1\n";
        break;
      }
      case node_type::r:
      {
        if ((node->children).empty())
        {
          if ((node->tokens).front()->id == token_ID::num_literal)
            *os << "LOAD " << (node->tokens).front()->instance << "\n";
          else
            *os << "STACKR " << find(os, (node->tokens).front()->instance, identifier_stack) << "\n";
        }
        else
          traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::stats:
      case node_type::m_stat:
      case node_type::stat:
      {
        for (auto& n : node->children)
          traversal(os, n, identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::in:
      {
        std::string temp_var = new_var();
        *os << "READ " << temp_var << "\n";
        *os << "LOAD " << temp_var << "\n";
        *os << "STACKW " << find(os, (node->tokens).front()->instance, identifier_stack) << "\n";
        break;
      }
      case node_type::out:
      {
        traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        std::string temp_var = new_var();
        *os << "STORE " << temp_var << "\n";
        *os << "WRITE " << temp_var << "\n";
        break;
      }
      case node_type::_if:
      {
        std::string temp_var = new_var();
        traversal(os, (node->children).at(0), identifier_stack, var_count, error_flag);
        *os << "STORE " << temp_var << "\n";
        traversal(os, (node->children).at(2), identifier_stack, var_count, error_flag);
        if ((node->children).at(1)->tokens.front()->instance != ".")
          *os << "SUB " << temp_var << "\n";
        else
          *os << "MULT " << temp_var << "\n";
        traversal(os, (node->children).at(1), identifier_stack, var_count, error_flag);
        std::string label_stop = new_label();
        *os << label_stop << "\n";
        if ((node->children).at(1)->tokens.front()->instance == "==")
        {
          traversal(os, (node->children).at(1), identifier_stack, var_count, error_flag);
          *os << label_stop << "\n";
        }
        traversal(os, (node->children).at(3), identifier_stack, var_count, error_flag);
        *os << label_stop << ": NOOP\n";
        break;
      }
      case node_type::loop:
      {
        traversal(os, (node->children).front(), identifier_stack, var_count, error_flag);
        break;
      }
      case node_type::loop1:
      {
        std::string temp_var = new_var();
        std::string label_start = new_label();
        *os << label_start << ": ";
        traversal(os, (node->children).at(0), identifier_stack, var_count, error_flag);
        *os << "STORE " << temp_var << "\n";
        traversal(os, (node->children).at(2), identifier_stack, var_count, error_flag);
        if ((node->children).at(1)->tokens.front()->instance != ".")
          *os << "SUB " << temp_var << "\n";
        else
          *os << "MULT " << temp_var << "\n";
        traversal(os, (node->children).at(1), identifier_stack, var_count, error_flag);
        std::string label_stop = new_label();
        *os << label_stop << "\n";
        if ((node->children).at(1)->tokens.front()->instance == "==")
        {
          traversal(os, (node->children).at(1), identifier_stack, var_count, error_flag);
          *os << label_stop << "\n";
        }
        traversal(os, (node->children).at(3), identifier_stack, var_count, error_flag);
        *os << "BR " << label_start << "\n";
        *os << label_stop << ": NOOP\n";
        break;
      }
      case node_type::loop2:
      {
        std::string label_start = new_label();
        *os << label_start << ": ";
        traversal(os, (node->children).at(0), identifier_stack, var_count, error_flag);
        std::string temp_var = new_var();
        traversal(os, (node->children).at(1), identifier_stack, var_count, error_flag);
        *os << "STORE " << temp_var << "\n";
        traversal(os, (node->children).at(3), identifier_stack, var_count, error_flag);
        if ((node->children).at(2)->tokens.front()->instance != ".")
          *os << "SUB " << temp_var << "\n";
        else
          *os << "MULT " << temp_var << "\n";
        traversal(os, (node->children).at(2), identifier_stack, var_count, error_flag);
        *os << label_start << "\n";
        if ((node->children).at(2)->tokens.front()->instance == "==")
        {
          traversal(os, (node->children).at(2), identifier_stack, var_count, error_flag);
          *os << label_start << "\n";
        }
        break;
      }
      case node_type::ro:
      {
        if ((node->tokens).front()->instance == ">=")
          *os << "BRPOS ";
        else if ((node->tokens).front()->instance == "<=")
          *os << "BRNEG ";
        else if ((node->tokens).front()->instance == "==")
        {
          if (brpos == false)
          *os << "BRPOS ";
          else
            *os << "BRNEG ";
          brpos = !brpos;
        }
        else if ((node->tokens).front()->instance == "!=")
          *os << "BRZERO ";
        else if ((node->tokens).front()->instance == ".")
          *os << "BRNEG ";
        break;
      }
      case node_type::assign:
      {
        traversal(os, (node->children).back(), identifier_stack, var_count, error_flag);
        *os << "STACKW " << find(os, (node->tokens).front()->instance, identifier_stack) << "\n";
        break;
      }
      case node_type::label:
      {
        *os << (node->tokens).front()->instance << ": NOOP\n";
        break;
      }
      case node_type::_goto:
      {
        //check for label?
        *os << "BR " << (node->tokens).front()->instance << "\n";
        break;
      }
      //default:

        /*for (auto& n : (node->children))
          traversal(os, n, identifier_stack, var_count, error_flag);*/
    }
  }
}

void push(std::ostream* os, std::string s, std::vector<std::string>& identifier_stack)
{
  identifier_stack.push_back(s);
  *os << "PUSH\n";
}

void pop(std::ostream* os, std::vector<std::string>& identifier_stack)
{
  identifier_stack.pop_back();
  *os << "POP\n";
}

int find(std::ostream* os, std::string s, std::vector<std::string>& identifier_stack)
{
  int index {-1};
  for (int i {identifier_stack.size() - 1}; i >= 0; --i)
  {
    if (identifier_stack.at(i) == s)
      index = ((identifier_stack.size() - 1) - i);
  }
  return index;
}

std::string new_var()
{
  std::string t_var("T");
  t_var += std::to_string(num_temp_vars++);
  return t_var;
}

std::string new_label()
{
  std::string l_var("L");
  l_var += std::to_string(num_labels++);
  return l_var;
}
