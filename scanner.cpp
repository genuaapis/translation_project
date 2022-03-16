/* Implementation of scanner and auxiliary functions. */

#include <vector>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <exception>
#include "token.hpp"
#include "scanner.hpp"

extern std::map<token_ID, std::string> string_map;
extern std::map<std::string, token_ID> token_map;

std::vector<token> scanner(std::istream* is)
{
  std::vector<token> token_list;
  bool in_comment {false};
  std::map<char, int> column_map = create_column_map();
  std::vector<std::vector<int>> fsa = create_fsa();
  std::vector<std::string> data = filter(is);

  int line {0},
      state {0};

  for (auto& s : data)
  {
    ++line;
    state = 0;
    int char_count{0};
    token t;
    std::string text;
    /* FSA driver */
    for (auto& c : s)
    {
      try {state = (fsa.at(state)).at(column_map.at(c));}
      catch (std::exception& e) {state = -1;}
      if (state < 0)
      {
        t.id = token_ID::error;
        t.instance = c;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        goto error_escape;
      }
      else if (state == 1)
      {
        t.id = token_ID::identifier;
        t.line_num = line;
        t.char_count = char_count;
        text += c;
        if (keyword_filter(token_list, t, text))
        {
          state = 0;
          text = "";
        }
      }
      else if (state == 2)
      {
        t.id = token_ID::num_literal;
        t.line_num = line;
        t.char_count = char_count;
        text += c;
      }
      else if ((state == 4) || (state == 5))
      {
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        text = c;
      }
      else if ((state == 1000) || (state == 2000) || (state == 3000))
      {
        t.instance = text;
        token_list.push_back(t);
        t.id = token_ID::identifier;
        t.line_num = line;
        t.char_count = char_count;
        text = c;
        state = 1;
      }
      else if (state == 300)
      {
        text = c;
        t.instance = text;
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if (state == 700)
      {
        text = c;
        t.instance = text;
        t.id = token_ID::delimiter;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if ((state == 1300) || (state == 2300) || (state == 3300))
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.instance = text;
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if ((state == 1400) || (state == 2400) || (state == 3400) || (state == 4600))
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        state = 4;
      }
      else if ((state == 1500) || (state == 2500) || (state == 3500) || (state == 4500))
      {
        t.instance = text;
        token_list.push_back(t);
      }
      else if ((state == 1700) || (state == 2700) || (state == 3700))
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.instance = text;
        t.id = token_ID::delimiter;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if ((state == 1800) || (state == 2800) || (state == 3800) || (state == 4800))
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        state = 5;
      }
      else if (state == 4000)
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.id = token_ID::identifier;
        t.line_num = line;
        t.char_count = char_count;
        state = 1;
      }
      else if (state == 4200)
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.id = token_ID::num_literal;
        t.line_num = line;
        t.char_count = char_count;
        state = 2;
      }
      else if (state == 4300)
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.instance = text;
        t.id = token_ID::op;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if ((state == 4400) || (state == 5400))
      {
        text += c;
        t.instance = text;
        token_list.push_back(t);
        text = "";
        state = 0;
      }
      else if (state == 4700)
      {
        t.instance = text;
        token_list.push_back(t);
        text = c;
        t.instance = text;
        t.id = token_ID::delimiter;
        t.line_num = line;
        t.char_count = char_count;
        token_list.push_back(t);
        text = "";
        state = 0;
      }

      ++char_count;
    }
  }

  error_escape:

  /* A method for correcting character count */
  for (auto& t : token_list)
  {
    t.char_count -= (t.instance.length() - 1);
    if (t.char_count == -1)
      ++t.char_count;
  }

  token eof_tok;
  eof_tok.id = token_ID::eof;
  eof_tok.instance = "EOF";
  eof_tok.line_num = data.size() + 1;
  eof_tok.char_count = 0;
  token_list.push_back(eof_tok);

  return token_list;
}

/* Filters for whitespace and comments */
std::vector<std::string> filter(std::istream* is)
{
  std::vector<std::string> filtered_input;
  std::string line;
  bool comment = false;
  while (getline(*is, line))
  {
    line = std::regex_replace(line, std::regex("[[:s:]]"), std::string(""));

    std::size_t comment_loc = line.find("##");
    while (comment_loc != std::string::npos)
    {
      if (!comment)
      {
        comment_loc = line.find("##", ++comment_loc);
        if (comment_loc != std::string::npos)
          line = std::regex_replace(line, std::regex("##[^##]*##"), std::string(""));
        else
        {
          line = std::regex_replace(line, std::regex("##.*"), std::string(""));
          comment = true;
          break;
        }
      }
      else
      {
        line = std::regex_replace(line, std::regex("[^##]*##"), std::string(""), std::regex_constants::format_first_only);
        comment = false;
      }
      comment_loc = line.find("##", ++comment_loc);
    }

    line += '\0';
    filtered_input.push_back(line);
  }

  return filtered_input;
}

std::map<char, int> create_column_map()
{
  std::map<char, int> m {{'_', 0},
                         {'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}, {'e', 1},
                         {'f', 1}, {'g', 1}, {'h', 1}, {'i', 1}, {'j', 1},
                         {'k', 1}, {'l', 1}, {'m', 1}, {'n', 1}, {'o', 1},
                         {'p', 1}, {'q', 1}, {'r', 1}, {'s', 1}, {'t', 1},
                         {'u', 1}, {'v', 1}, {'w', 1}, {'x', 1}, {'y', 1},
                         {'z', 1},
                         {'A', 9}, {'B', 9}, {'C', 9}, {'D', 9}, {'E', 9},
                         {'F', 9}, {'G', 9}, {'H', 9}, {'I', 9}, {'J', 9},
                         {'K', 9}, {'L', 9}, {'M', 9}, {'N', 9}, {'O', 9},
                         {'P', 9}, {'Q', 9}, {'R', 9}, {'S', 9}, {'T', 9},
                         {'U', 9}, {'V', 9}, {'W', 9}, {'X', 9}, {'Y', 9},
                         {'Z', 9},
                         {'0', 2}, {'1', 2}, {'2', 2}, {'3', 2}, {'4', 2},
                         {'5', 2}, {'6', 2}, {'7', 2}, {'8', 2}, {'9', 2},
                         {'+', 3}, {'-', 3}, {'*', 3}, {'/', 3}, {'%', 3},
                         {';', 3}, {',', 3}, {'.', 3},
                         {'=', 4},
                         {'\0', 5},
                         {':', 6},
                         {'(', 7}, {')', 7}, {'{', 7}, {'}', 7}, {'[', 7},
                         {']', 7},
                         {'<', 8}, {'>', 8}, {'!', 8}};
  return m;
}


/* FSA table */
std::vector<std::vector<int>> create_fsa()
{
  std::vector<std::vector<int>> v;
  v.push_back({1,    1,    2,    300,  4,    500,  4,    700,  5,    -2});
  v.push_back({1000, 1,    1,    1300, 1400, 1500, 1400, 1700, 1800,  1});
  v.push_back({2000, 2000, 2,    2300, 2400, 2500, 2400, 2700, 2800, -2});
  v.push_back({3000, 3000, 3200, 3300, 3400, 3500, 3400, 3700, 3800, -2});
  v.push_back({4000, 4000, 4200, 4300, 4400, 4500, 4600, 4700, 4800, -2});
  v.push_back({-1,   -1,   -1,   -1,   5400, -1,   -1,   -1,   -1,   -2});
  return v;
}

/* Filters for keywords in identifiers */
bool keyword_filter(std::vector<token>& v, token t, std::string text)
{
  bool match {false};
  if ((token_map.find(text) != token_map.end()) && (text != "delimiter") && (text != "operator") && (text != "identifier") && (text != "error"))
  {
    t.id = token_map.at(text);
    t.instance = text;
    v.push_back(t);
    match = true;
  }
  return match;
}
