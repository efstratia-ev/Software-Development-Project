#include <cstdio>
#include "SQLInfo.h"

set::set(int a, int c) {
    array=a;
    column=c;
}

set::set() {}

int set::getArray(){
    return array;
}

void set::setArray(int a) {
    set::array = a;
}

void set::setColumn(int c) {
    set::column = c;
}

int set::getColumn(){
    return column;
}


Priority_Queue::Priority_Queue() {
    head=NULL;
    used_arrays=NULL;
    size=0;
}


void Priority_Queue::Push(Predicate *p){
    head=new Priority_Queue_Node(p,head);
    size++;

}
Predicate* Priority_Queue::Priority_Queue::Pop(){
    Predicate *predicate=head->getPredicate();
    head=head->get_next();
    size--;
    if(!predicate->is_filter()){
        int array1=IsUsedArray(predicate->getArray1()->getArray());
        int array2=IsUsedArray(predicate->getArray2()->getArray());
        if(!IsUsedArray(array1)) used_arrays= new List_Int(array1,used_arrays);
        if(!IsUsedArray(array2)) used_arrays= new List_Int(array2,used_arrays);
        if(size>1) Rearrange();
    }
    return predicate;
}

void Priority_Queue::Rearrange(){
    Priority_Queue_Node *current=head,*previous=NULL;
    Priority_Queue_Node * tmp;

    while(current!=NULL){
        if(current->getPredicate()->is_filter()){
            if(AreUsedArray(current->getPredicate()->getArray1()->getArray(),current->getPredicate()->getArray2()->getArray())) {
                tmp = current->get_next();
                current->setNext(head);
                previous->setNext(tmp);
                head=current;
            }
        }
        else {
            previous=current;
            current=current->get_next();
        }
    }
}

int Priority_Queue::InitRearrange(){
    int filters=0;
    Priority_Queue_Node *current=head,*previous=NULL;
    Priority_Queue_Node * tmp;
    Priority_Queue_Node * next_tmp;
    if(head->getPredicate()->is_filter()) filters++;
    while(current!=NULL){
        if(current->getPredicate()->is_filter() && current!=head){
            filters++;
            next_tmp=current->get_next();
            tmp = current->get_next();
            current->setNext(head);
            previous->setNext(tmp);
            head=current;
            current=next_tmp;
        }
        else {
            previous=current;
            current=current->get_next();
        }
    }
    return filters;
}


bool Priority_Queue::IsEmpty(){
    return (this->size==0) ;
}

Priority_Queue::~Priority_Queue(){
    List_Int * temp=used_arrays->getNext();
    while(temp!=NULL){
        delete used_arrays;
        used_arrays=temp;
    }
}

bool Priority_Queue::AreUsedArray(int target1,int target2){
    List_Int * temp=used_arrays;
    int count=0;
    while(temp!=NULL){
        if(temp->getData()==target1) count++;
        else if (temp->getData()==target2) count++;
        temp=temp->getNext();
    }
    return count==2 ;
}

bool Priority_Queue::IsUsedArray(int target) {
    List_Int *temp = used_arrays;
    while (temp != NULL) {
        if (temp->getData() == target) return true;
        temp = temp->getNext();
    }
    return false;
}

List_Int *List_Int::getNext(){
    return next;
}

List_Int::List_Int(int d,List_Int * n){
    data=d;
    next=n;
}

int List_Int::getData(){
    return data;
}