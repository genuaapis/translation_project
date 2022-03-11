/* Contains useful maps between token_ID and relevant string. */

#include <map>
#include <string>
#include "token.hpp"

std::map<token_ID, std::string> string_map {{token_ID::identifier, "identifier"}, {token_ID::num_literal, "numeric literal"},
                                           {token_ID::op, "operator"}, {token_ID::delimiter, "delimiter"},
                                           {token_ID::start_key, "start"}, {token_ID::stop_key, "stop"},
                                           {token_ID::repeat_key, "repeat"}, {token_ID::until_key, "until"},
                                           {token_ID::whole_key, "whole"}, {token_ID::label_key, "label"},
                                           {token_ID::quit_key, "quit"}, {token_ID::listen_key, "listen"},
                                           {token_ID::yell_key, "yell"}, {token_ID::main_key, "main"},
                                           {token_ID::portal_key, "portal"}, {token_ID::if_key, "if"},
                                           {token_ID::then_key, "then"}, {token_ID::pick_key, "pick"},
                                           {token_ID::declare_key, "declare"}, {token_ID::assign_key, "assign"},
                                           {token_ID::proc_key, "proc"}, {token_ID::error, "error"},
                                           {token_ID::eof, "EOF"}};

std::map<std::string, token_ID> token_map {{"identifier", token_ID::identifier}, {"numeric literal", token_ID::num_literal},
                                           {"operator", token_ID::op}, {"delimiter", token_ID::delimiter},
                                           {"start", token_ID::start_key}, {"stop", token_ID::stop_key},
                                           {"repeat", token_ID::repeat_key}, {"until", token_ID::until_key},
                                           {"whole", token_ID::whole_key}, {"label", token_ID::label_key},
                                           {"quit", token_ID::quit_key}, {"listen", token_ID::listen_key},
                                           {"yell", token_ID::yell_key}, {"main", token_ID::main_key},
                                           {"portal", token_ID::portal_key}, {"if", token_ID::if_key},
                                           {"then", token_ID::then_key}, {"pick", token_ID::pick_key},
                                           {"declare", token_ID::declare_key}, {"assign", token_ID::assign_key},
                                           {"proc", token_ID::proc_key}, {"error", token_ID::error},
                                           {"EOF", token_ID::eof}};
