#include <vector>
#include <string>
#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"

parse_node* _goto(std::vector<token>::iterator& current_token)
{
  parse_node* node = new parse_node;
  node->type = node_type::_goto;
  if (current_token->id == token_ID::identifier)
    node->tokens.push_back(current_token);
  else
  {
    node->type = node_type::error;
    std::cout << "Error: " /*FINISH THIS*/
  }
  ++current_token;
  return node;
}
