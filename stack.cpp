#include "stack.h"

stack_node::stack_node(radix *h, stack_node *nd) {
    Radix=h;
    next=nd;
}

radix *stack_node::getRadix() const {
    return Radix;
}

stack_node *stack_node::getNext() const {
    return next;
}

stack::stack() {
    Stack=NULL;
}

void stack::push(radix *H) {
    Stack=new stack_node(H,Stack);
}

radix *stack::pop() {
    stack_node *temp=Stack;
    Stack=Stack->getNext();
    radix *Radix=temp->getRadix();
    delete temp;
    return Radix;
}

bool stack::notEmpty() {
    return !(Stack==NULL);
}
