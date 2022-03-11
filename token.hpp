#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class token_ID
{
  identifier, num_literal, op, delimiter,
  start_key, stop_key, repeat_key, until_key, whole_key, label_key, quit_key, listen_key,
  yell_key, main_key, portal_key, if_key, then_key, pick_key, declare_key, assign_key, proc_key,
  error, eof
};

struct token
{
  token_ID id;
  std::string instance;
  int line_num;
  int char_count;
};

#endif
