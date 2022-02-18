#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>

class node
{
  public:
    char last_letter;
    node* l_node;
    node* m_node;
    node* r_node;
    std::string word;

    node(std::string);
    void add_string(std::string);
};

#endif
