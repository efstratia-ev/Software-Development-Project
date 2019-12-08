#include <cstdio>
#include "SQLInfo.h"

set::set(int a, int c) {
    array=a;
    column=c;
}

set::set() {}

int set::getArray() const {
    return array;
}

void set::setArray(int array) {
    set::array = array;
}

void set::setColumn(int column) {
    set::column = column;
}

int set::getColumn() const {
    return column;
}


Priority_Queue::Priority_Queue() {
    this->head=NULL;
    this->size=0;
}


void Priority_Queue::Push(Predicate *p){
    head=new node(p,head);
    this->size++;

}
Predicate* Priority_Queue::Priority_Queue::Pop(){
    node * tmp = head->get_next();
    delete head;
    this->head = tmp;
    this->size--;
}

bool Priority_Queue::IsEmpty(){
    return (this->size==0) ;
}
