/* Main file for testing. */

#include <iostream>
#include <fstream>
#include "test_scanner.hpp"

int main(int argc, char* argv[])
{
  std::istream* is = &std::cin;
  if (argc == 2)
    is = new std::ifstream(argv[1]);
  else if (argc > 2)
    std::cout << "Invalid number of arguments. Terminating.\n";
  test_scanner(is);
  return 0;
}
