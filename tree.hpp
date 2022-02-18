#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "node.hpp"

node* build_tree(std::istream*);
void in_order(node*, std::ostream*, int*);
void pre_order(node*, std::ostream*, int*);
void post_order(node*, std::ostream*, int*);

#endif
