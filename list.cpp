#include <cstdlib>
#include <cstring>
#include <iostream>
#include "list.h"
using namespace std;
listNode::listNode(bool t) {
    buffer=(char*)malloc(1024*1024);
    if(buffer==nullptr){
        std::cout<<"error"<<std::endl;
    }
    max=(1024*1024)/(sizeof(uint64_t));  //max size of results in the buffer
    count=0;   //current number of results in the buffer
    current=buffer;
    next=nullptr;
    tuples=t;
}

void listNode::add(uint64_t rowID1, uint64_t rowID2) {  //add new result
    count+=1;
    memcpy(current,&rowID1, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    memcpy(current,&rowID2, sizeof(uint64_t));
    if(count!=max/2) current+= sizeof(uint64_t);
}

bool listNode::isFull() {
    if(tuples) return count==(max/2);
    return count==max;
}


listNode *listNode::createNext() {  //new list node
    next=new listNode(tuples);
    return next;
}

listNode *listNode::getnext() {
    return next;
}

uint64_t listNode::getcount(){
    return count;

}

rowids* listNode::pop() {
    if(count==0) return nullptr;
    count-=1;
    uint64_t temp1,temp2;
    memcpy(&temp1,current, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    memcpy(&temp2,current, sizeof(uint64_t));
    if(count!=max/2) current+= sizeof(uint64_t);
    auto *first=new rowids(temp1,temp2);
    return first;
}

void listNode::add(uint64_t rowID) {
    count++;
    memcpy(current,&rowID, sizeof(uint64_t));
    if(count!=max) current+= sizeof(uint64_t);
}

uint64_t listNode::pop_element() {
    count--;
    uint64_t temp;
    memcpy(&temp,current, sizeof(uint64_t));
    if(count!=max) current+= sizeof(uint64_t);
    return temp;
}

void listNode::restart_current() {
    current=buffer;
    if(next) next->restart_current();
}

listNode::~listNode() {
    free(buffer);
    delete next;
}

list::list() {
    size=0;
    start=nullptr;
    current=start;
}

void list::add(uint64_t rowID1, uint64_t rowID2) {
    if(!start){
        start=new listNode(true);
        current=start;
    }
    size++;
    if(current->isFull())
        current=current->createNext();  //new list node
    current->add(rowID1,rowID2);  //add the result
}


list::~list() {
    delete start;
}

uint64_t list::get_size() {
    return size;
}

void list::restart_current() {
    if(size==0) return;
    current=start;
    start->restart_current();
}

rowids *list::pop() {
    rowids *temp=current->pop();
    if(temp) return temp;
    current=current->getnext();
    if(!current) return nullptr;
    return current->pop();
}

uint64_t list::pop_element() {
    if(current->getcount()>0){
        return current->pop_element();
    }
    current=current->getnext();
    return current->pop_element();
}

void list::add(uint64_t rowID) {
    if(!start){
        start=new listNode(false);
        current=start;
    }
    size++;
    if(current->isFull())
        current=current->createNext();  //new list node
    current->add(rowID);  //add the result
}
