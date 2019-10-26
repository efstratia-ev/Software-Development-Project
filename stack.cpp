#include "stack.h"

stack_node::stack_node(hashtable *h, stack_node *nd) {
    Hash=h;
    next=nd;
}

hashtable *stack_node::getHash() const {
    return Hash;
}

stack_node *stack_node::getNext() const {
    return next;
}

stack::stack() {
    Stack=NULL;
}

void stack::push(hashtable *H) {
    Stack=new stack_node(H,Stack);
}

hashtable *stack::pop() {
    stack_node *temp=Stack;
    Stack=Stack->getNext();
    hashtable *Hash=temp->getHash();
    delete temp;
    return Hash;
}

bool stack::notEmpty() {
    return !(Stack==NULL);
}
