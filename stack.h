#ifndef STACK_H
#define STACK_H

#include <cstdio>
#include "hashtable.h"

class hashtable;

class stack_node {
    hashtable *Hash;
    stack_node *next;
public:
    stack_node(hashtable *h,stack_node *nd);

    hashtable *getHash() const;

    stack_node *getNext() const;

};

class stack{
    stack_node *Stack;
public:
    stack();
    void push(hashtable *H);
    hashtable * pop();
    bool notEmpty();
};
#endif //STACK_H
