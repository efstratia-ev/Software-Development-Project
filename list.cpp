#include <cstdlib>
#include <cstring>
#include <iostream>
#include "list.h"
using namespace std;
listNode::listNode() {
    buffer=(char*)malloc(1024*1024);
    if(buffer==NULL){
        std::cout<<"error"<<std::endl;
    }
    max=(1024*1024)/( sizeof(uint64_t));  //max size of results in the buffer
    count=0;   //current number of results in the buffer
    current=buffer;
    next=NULL;
}

void listNode::add(uint64_t rowID1, uint64_t rowID2) {  //add new result
    count+=2;
    memcpy(current,&rowID1, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    memcpy(current,&rowID2, sizeof(uint64_t));
    if(count!=max) current+= sizeof(uint64_t);
}

bool listNode::isFull() {
    return count==max;
}


listNode *listNode::createNext() {  //new list node
    next=new listNode();
    return next;
}

void listNode::print() { //print the results
    uint64_t temp;
    char *current_item=buffer;
    for(uint64_t i=0; i<count; i++){
        memcpy(&temp,current_item, sizeof(uint64_t));
        current_item+= sizeof(uint64_t);
        std::cout<<temp<<",";
        memcpy(&temp,current_item, sizeof(uint64_t));
        current_item+= sizeof(uint64_t);
        std::cout<<temp<<std::endl;
    }
}

listNode *listNode::getnext() {
    return next;
}

uint64_t listNode::getcount(){
    return count;

}


/*listNode::~listNode() {
    free(buffer);
    if(next) delete next;
}*/

rowids* listNode::pop() {
    if(count==0) return NULL;
    count-=2;
    uint64_t temp1,temp2;
    memcpy(&temp1,current, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    memcpy(&temp2,current, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    rowids *first=new rowids(temp1,temp2);
    return first;
}

void listNode::add(uint64_t rowID) {
    count++;
    memcpy(current,&rowID, sizeof(uint64_t));
    if(count!=max) current+= sizeof(uint64_t);
}

uint64_t listNode::pop_element() {
    if(count==0) return NULL;
    count--;
    uint64_t temp;
    memcpy(&temp,current, sizeof(uint64_t));
    current+= sizeof(uint64_t);
    return temp;
}

void listNode::restart_current() {
    current=buffer;
}

list::list() {
    size=0;
    start=new listNode();
    current=start;
}

void list::add(uint64_t rowID1, uint64_t rowID2) {
    size++;
    if(current->isFull())
        current=current->createNext();  //new list node
    current->add(rowID1,rowID2);  //add the result
}

void list::print(){
    current=start;
    while(current){
        current->print();
        current=current->getnext();
    }
}

void list::printSize(){
    current=start;
    uint64_t count=0;
    while(current){
        count+=current->getcount();
        current=current->getnext();
    }
    cout<<"Number of results is :"<<count<<"\n";
}


list::~list() {
    delete start;
}

uint64_t list::get_size() {
    return size;
}

void list::restart_current() {
    current=start;
    while(current){
        current->restart_current();
        current=current->getnext();
    }
    current=start;
}

rowids *list::pop() {
    rowids *temp=current->pop();
    if(temp) return temp;
    current=current->getnext();
    return current->pop();
}

uint64_t list::pop_element() {
    uint64_t  temp=current->pop_element();
    if(temp) return temp;
    current=current->getnext();
    return current->pop_element();
}

void list::add(uint64_t rowID) {
    size++;
    if(current->isFull())
        current=current->createNext();  //new list node
    current->add(rowID);  //add the result
}
