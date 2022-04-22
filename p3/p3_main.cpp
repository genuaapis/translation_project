#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "semantics.hpp"

int main(int argc, char* argv[])
{
  std::istream* is = &std::cin;
  if (argc == 2)
    is = new std::ifstream(argv[1]);
  else if (argc > 2)
  {
    std::cout << "Invalid number of arguments. Terminating.\n";
    return 1;
  }
  std::vector<token> scanner_tokens = scanner(is);
  parse_node* parse_tree = parser(scanner_tokens);
  std::vector<std::string> identifier_stack;
  std::vector<int> var_count;
  var_count.push_back(0);
  bool error_flag {false};
  traversal(parse_tree, identifier_stack, var_count, error_flag);

  return 0;
}
