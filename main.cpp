#include <iostream>
#include <fstream>
#include <string>
#include "node.hpp"
#include "tree.hpp"

int main(int argc, char* argv[])
{
  std::istream* is = &std::cin;
  std::string base {"output"};
  bool in_file = false;
  if (argc == 2)
  {
    std::string file_name(argv[1]);
    base = file_name;
    file_name += ".sp2022";
    is = new std::ifstream(file_name);
    in_file = true;
    if(is->fail())
    {
      std::cout << "Error: could not open specified file.\n";
      return -1;
    }
    in_file = true;  in_file = true;
  }
  else if (argc > 2)
  {
    std::cout << "Invalid number of arguments. Terminating.\n";
    return -1;
  }

  node* tree = build_tree(is);
  std::string log_name {base};
  std::ofstream* os = new std::ofstream();
  int depth = -1;

  log_name += ".pre_order";
  os->open(log_name, std::ios::trunc);
  pre_order(tree, os, &depth);
  os->close();
  log_name = base;

  log_name += ".in_order";
  os->open(log_name, std::ios::trunc);
  in_order(tree, os, &depth);
  os->close();
  log_name = base;

  log_name += ".post_order";
  os->open(log_name, std::ios::trunc);
  post_order(tree, os, &depth);
  os->close();
  log_name = base;

  if (in_file)
    delete is;
  delete os;

  return 0;
}
