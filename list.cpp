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
    max=(1024*1024)/(2* sizeof(uint64_t));  //max size of results in the buffer
    count=0;   //current number of results in the buffer
    current=buffer;
    next=NULL;
}

void listNode::add(uint64_t rowID1, uint64_t rowID2) {  //add new result
    count++;
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


listNode::~listNode() {
    free(buffer);
    if(next) delete next;
}

list::list() {
    start=new listNode();
    current=start;
}

void list::add(uint64_t rowID1, uint64_t rowID2) {
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
    int count=0;
    while(current){
        count+=current->getcount();
        current=current->getnext();
    }
    cout<<"Number of results is :"<<count<<"\n";
}


list::~list() {
    delete start;
}
