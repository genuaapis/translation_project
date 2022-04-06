#include <iostream>
#include <fstream>
#include "token.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "test_parser.hpp"

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
  if (parse_tree->type != node_type::error)
  {
    std::ofstream* os = new std::ofstream();
    os->open("parser_output.txt");
    int real_depth {0};
    int* depth {&real_depth};
    print(parse_tree, os, depth);
    os->close();
  }

  return 0;
}
