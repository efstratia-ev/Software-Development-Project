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
    max=(1024*1024)/(2* sizeof(int));  //max size of results in the buffer
    count=0;   //current number of results in the buffer
    current=buffer;
    next=NULL;
}

void listNode::add(int rowID1, int rowID2) {  //add new result
    count++;
    memcpy(current,&rowID1, sizeof(int));
    current+= sizeof(int);
    memcpy(current,&rowID2, sizeof(int));
    if(count!=max) current+= sizeof(int);
}

bool listNode::isFull() {
    return count==max;
}


listNode *listNode::createNext() {  //new list node
    next=new listNode();
    return next;
}

void listNode::print() { //print the results
    int temp;
    char *current_item=buffer;
    for(int i=0; i<count; i++){
        memcpy(&temp,current_item, sizeof(int));
        current_item+= sizeof(int);
        std::cout<<temp<<" ";
        memcpy(&temp,current_item, sizeof(int));
        current_item+= sizeof(int);
        std::cout<<temp<<std::endl;
    }
}

listNode *listNode::getnext() {
    return next;
}

listNode::~listNode() {
    free(buffer);
    if(next) delete next;
}

list::list() {
    start=new listNode();
    current=start;
}

void list::add(int rowID1, int rowID2) {
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

list::~list() {
    delete start;
}
