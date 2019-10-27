#ifndef STACK_H
#define STACK_H

#include <cstdio>
#include "radix.h"

class radix;

class stack_node {
    radix *Radix;
    stack_node *next;
public:
    stack_node(radix *h,stack_node *nd);

    radix *getRadix() const;

    stack_node *getNext() const;

};

class stack{
    stack_node *Stack;
public:
    stack();
    void push(radix *H);
    radix * pop();
    bool notEmpty();
};
#endif //STACK_H
