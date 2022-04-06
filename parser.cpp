#include <vector>
#include <string>
#include <map>
#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"

extern std::map<token_ID, std::string> string_map;
extern std::map<std::string, token_ID> token_map;

std::map<node_type, std::string> node_map {{node_type::program, "program"}, {node_type::block, "block"},
                                           {node_type::vars, "vars"}, {node_type::expr, "expr"},
                                           {node_type::n, "n"}, {node_type::z, "z"},
                                           {node_type::a, "a"}, {node_type::m, "m"},
                                           {node_type::r, "r"}, {node_type::stats, "stats"},
                                           {node_type::m_stat, "m_stat"}, {node_type::stat, "stat"},
                                           {node_type::in, "in"}, {node_type::out, "out"},
                                           {node_type::_if, "if"}, {node_type::loop, "loop"},
                                           {node_type::loop1, "loop1"}, {node_type::loop2, "loop2"},
                                           {node_type::assign, "assign"}, {node_type::ro, "ro"},
                                           {node_type::label, "label"}, {node_type::_goto, "goto"},
                                           {node_type::error, "error"}};

parse_node* parser(std::vector<token>& tokens)
{
  std::vector<token>::iterator token_i {tokens.begin()};
  parse_node* parse_root = new parse_node;
  parse_root = program(token_i);
  return parse_root;
}

parse_node* program(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::program;
  node->children.push_back(vars(current_token));
  if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
    node->type = node_type::error;
  else
  {
    if (current_token->id == token_ID::main_key)
    {
      ++current_token;
      node->children.push_back(block(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
      else
      {
        if (current_token->id != token_ID::eof)
        {
          node->type = node_type::error;
          std::cout << "Error line " << current_token->line_num << ": expected end of file, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
        }
      }
    }
    else
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected 'main', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
    }
  }
  return node;
}

parse_node* block(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::block;
  if (current_token->instance == "{")
  {
    ++current_token;
    node->children.push_back(vars(current_token));
    if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
      node->type = node_type::error;
    else
    {
      node->children.push_back(stats(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
      else
      {
        if (current_token->instance == "}")
          ++current_token;
        else
        {
          node->type = node_type::error;
          std::cout << "Error line " << current_token->line_num << ": expected '}', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
        }
      }
    }
  }
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected '{', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
  }
  return node;
}

parse_node* vars(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::vars;
  if (current_token->id == token_ID::declare_key)
  {
    ++current_token;
    if (current_token->id == token_ID::identifier)
    {
      node->tokens.push_back(current_token);
      ++current_token;
      if (current_token->instance == ":=")
      {
        ++current_token;
        if (current_token->id == token_ID::whole_key)
        {
          ++current_token;
          if (current_token->instance == ";")
          {
            ++current_token;
            node->children.push_back(vars(current_token));
            if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
              node->type = node_type::error;
          }
          else
          {
            node->type = node_type::error;
            std::cout << "Error line " << current_token->line_num << ": expected ';', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
          }
        }
        else
        {
          node->type = node_type::error;
          std::cout << "Error line " << current_token->line_num << ": expected 'whole', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
        }
      }
      else
      {
        node->type = node_type::error;
        std::cout << "Error line " << current_token->line_num << ": expected ':=', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
      }
    }
    else
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected identifier, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
    }
  }
  else
    node = NULL;
  return node;
}

parse_node* expr(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::expr;
  node->children.push_back(n(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else
  {
    if (current_token->instance == "-")
    {
      ++current_token;
      node->children.push_back(expr(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
  }
  return node;
}

parse_node* n(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::n;
  node->children.push_back(a(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else
  {
    node->children.push_back(z(current_token));
    if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
      node->type = node_type::error;
  }
  return node;
}

parse_node* z(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::z;
  if ((current_token->instance == "/") || (current_token->instance == "+"))
  {
    node->tokens.push_back(current_token);
    ++current_token;
    node->children.push_back(a(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
    else
    {
      node->children.push_back(z(current_token));
      if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
        node->type = node_type::error;
    }
  }
  else
    node = NULL;
  return node;
}

parse_node* a(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::a;
  node->children.push_back(m(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else if (current_token->instance == "*")
  {
    ++current_token;
    node->children.push_back(a(current_token));
  }
  return node;
}

parse_node* m(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::m;
  if (current_token->instance == "%")
  {
    ++current_token;
    node->children.push_back(m(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
  }
  else
  {
    node->children.push_back(r(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
  }
  return node;
}

parse_node* r(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::r;
  if (current_token->instance == "(")
  {
    ++current_token;
    node->children.push_back(expr(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
    else if (current_token->instance != ")")
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected ')', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
    }
  }
  else if ((current_token->id == token_ID::identifier) || (current_token->id == token_ID::num_literal))
    node->tokens.push_back(current_token);
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected '(', identifier, or numeric literal, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
  }
  ++current_token;
  return node;
}

parse_node* stats(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::stats;
  node->children.push_back(stat(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else
  {
    node->children.push_back(m_stat(current_token));
    if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
      node->type = node_type::error;
  }
  return node;
}

parse_node* m_stat(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::m_stat;
  if (current_token->instance == "}")
    node = NULL;
  else
  {
    node->children.push_back(stat(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
    else
    {
      node->children.push_back(m_stat(current_token));
      if ((node->children.back() != NULL) && ((node->children.back())->type == node_type::error))
        node->type = node_type::error;
    }
  }
  return node;
}

parse_node* stat(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::stat;
  if (current_token->instance == "{")
  {
    node->children.push_back(block(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
  }
  else
  {
    if (current_token->id == token_ID::listen_key)
    {
      ++current_token;
      node->children.push_back(in(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::yell_key)
    {
      ++current_token;
      node->children.push_back(out(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::if_key)
    {
      ++current_token;
      node->children.push_back(_if(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::repeat_key)
    {
      ++current_token;
      node->children.push_back(loop(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::assign_key)
    {
      ++current_token;
      node->children.push_back(assign(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::label_key)
    {
      ++current_token;
      node->children.push_back(label(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else if (current_token->id == token_ID::portal_key)
    {
      ++current_token;
      node->children.push_back(_goto(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected statement, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
    }
    if (current_token->instance != ";")
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected ';', found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
    }
    ++current_token;
  }
  return node;
}

parse_node* in(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::in;
  if (current_token->id == token_ID::identifier)
    node->tokens.push_back(current_token);
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected identifier, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
  }
  ++current_token;
  return node;
}

parse_node* out(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::out;
  node->children.push_back(expr(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  return node;
}

parse_node* _if(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::_if;
  if (current_token->instance == "[")
  {
    ++current_token;
    node->children.push_back(expr(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
    else
    {
      node->children.push_back(ro(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
      else
      {
        node->children.push_back(expr(current_token));
        if ((node->children.back())->type == node_type::error)
          node->type = node_type::error;
        else
        {
          if (current_token->instance == "]")
          {
            ++current_token;
            if (current_token->id == token_ID::then_key)
            {
              ++current_token;
              node->children.push_back(stat(current_token));
              if ((node->children.back())->type == node_type::error)
                node->type = node_type::error;
            }
            else
            {
              std::cout << "Error line " << current_token->line_num << ": expected 'then', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
              node->type = node_type::error;
            }
          }
          else
          {
            std::cout << "Error line " << current_token->line_num << ": expected ']', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
            node->type = node_type::error;
          }
        }
      }
    }
  }
  else
  {
    std::cout << "Error line " << current_token->line_num << ": expected '[', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
    node->type = node_type::error;
  }
  return node;
}

parse_node* loop(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::loop;
  if (current_token->instance == "[")
  {
    ++current_token;
    node->children.push_back(loop1(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
  }
  else
  {
    node->children.push_back(loop2(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
  }
  return node;
}

parse_node* loop1(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::loop1;
  node->children.push_back(expr(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else
  {
    node->children.push_back(ro(current_token));
    if ((node->children.back())->type == node_type::error)
      node->type = node_type::error;
    else
    {
      node->children.push_back(expr(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
      else
      {
        if (current_token->instance == "]")
        {
          ++current_token;
          node->children.push_back(stat(current_token));
          if ((node->children.back())->type == node_type::error)
            node->type = node_type::error;
        }
        else
        {
          std::cout << "Error line " << current_token->line_num << ": expected ']', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
          node->type = node_type::error;
        }
      }
    }
  }
  return node;
}

parse_node* loop2(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::loop2;
  node->children.push_back(stat(current_token));
  if ((node->children.back())->type == node_type::error)
    node->type = node_type::error;
  else if (current_token->id == token_ID::until_key)
  {
    ++current_token;
    if (current_token->instance == "[")
    {
      ++current_token;
      node->children.push_back(expr(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
      else
      {
        node->children.push_back(ro(current_token));
        if ((node->children.back())->type == node_type::error)
          node->type = node_type::error;
        else
        {
          node->children.push_back(expr(current_token));
          if ((node->children.back())->type == node_type::error)
            node->type = node_type::error;
          else
          {
            if (current_token->instance != "]")
            {
              std::cout << "Error line " << current_token->line_num << ": expected ']', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
              node->type = node_type::error;
            }
          }
        }
      }
    }
    else
    {
      std::cout << "Error line " << current_token->line_num << ": expected '[', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
      node->type = node_type::error;
    }
  }
  else
  {
    std::cout << "Error line " << current_token->line_num << ": expected 'until', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
    node->type = node_type::error;
  }
  ++current_token;
  return node;
}

parse_node* assign(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::assign;
  if (current_token->id == token_ID::identifier)
  {
    node->tokens.push_back(current_token);
    ++current_token;
    if (current_token->instance == "=")
    {
      ++current_token; /*verify*/
      node->children.push_back(expr(current_token));
      if ((node->children.back())->type == node_type::error)
        node->type = node_type::error;
    }
    else
    {
      std::cout << "Error line " << current_token->line_num << ": expected '=', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
      node->type = node_type::error;
    }
  }
  else
  {
    std::cout << "Error line " << current_token->line_num << ": expected identifier, found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
    node->type = node_type::error;
  }
  return node;
}

parse_node* ro(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::ro;
  if ((current_token->instance == ">=") || (current_token->instance == "<=") ||
      (current_token->instance == "==") || (current_token->instance == "!="))
    node->tokens.push_back(current_token);
  else if (current_token->instance == ".")
  {
    ++current_token;
    if (current_token->instance == ".")
    {
      ++current_token;
      if (current_token->instance == ".")
        node->tokens.push_back(current_token);
      else
      {
        node->type = node_type::error;
        std::cout << "Error line " << current_token->line_num << ": expected '.', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
      }
    }
    else
    {
      node->type = node_type::error;
      std::cout << "Error line " << current_token->line_num << ": expected '.', found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
    }
  }
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected relational operator, found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
  }
  ++current_token;
  return node;
}

parse_node* label(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::label;
  if (current_token->id == token_ID::identifier)
    node->tokens.push_back(current_token);
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected identifier, found " << string_map.at(current_token->id) << ", '" << current_token->instance << "'.\n";
  }
  ++current_token;
  return node;
}

parse_node* _goto(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::_goto;
  if (current_token->id == token_ID::identifier)
    node->tokens.push_back(current_token);
  else
  {
    node->type = node_type::error;
    std::cout << "Error line " << current_token->line_num << ": expected identifier, found " << string_map.at(current_token->id) <<  ", '" << current_token->instance << "'.\n";
  }
  ++current_token;
  return node;
}
