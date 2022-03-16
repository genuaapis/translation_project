#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include "token.hpp"
#include "test_scanner.hpp"
#include "scanner.hpp"

extern std::map<token_ID, std::string> string_map;

void test_scanner(std::istream* is)
{
  std::vector<token> v = scanner(is);
  std::cout << std::endl;
  std::cout << std::right << std::setw(20) << "Type" << std::setw(20) << "Instance" << std::setw(20) << "Line Number" << std::setw(20) << "Character Number\n\n";
  for (auto&t : v)
    std::cout << std::right << std::setw(20) << string_map.at(t.id) << std::setw(20) << t.instance << std::setw(20) << t.line_num << std::setw(20) << t.char_count << "\n";
  std::cout << std::endl;
}
