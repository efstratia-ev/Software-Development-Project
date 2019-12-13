#include <iostream>
#include "Priority_Queue.h"

Priority_Queue::Priority_Queue() {
    head=nullptr;
    used_arrays=nullptr;
    filtered_arrays=nullptr;
    size=0;
    last_joined[0].setArray(-1);
    last_joined[1].setArray(-1);
}


void Priority_Queue::Push(Predicate *p){
    head=new Priority_Queue_Node(p,head);
    size++;

}
Predicate* Priority_Queue::Pop(){
    Predicate *predicate=head->getPredicate();
    Priority_Queue_Node *temp=head;
    head=head->get_next();
    delete temp;
    size--;
    if(size==0) return predicate;
    if(!predicate->is_filter()){
        int array1=predicate->getArray1()->getArray();
        int array2=predicate->getArray2()->getArray();
        if(!IsUsedArray(array1)) used_arrays= new List_Int(array1,used_arrays);
        if(!IsUsedArray(array2)) used_arrays= new List_Int(array2,used_arrays);
        Rearrange();
        last_joined[0].copy(predicate->getArray1());
        last_joined[1].copy(predicate->getArray2());
    }
    else {
        int array=predicate->getArray1()->getArray();
        if(!IsFilteredArray(array)) filtered_arrays= new List_Int(array,filtered_arrays);
        Rearrange();
    }
    return predicate;
}

void Priority_Queue::Rearrange(){
    Priority_Queue_Node *current=head,*previous=nullptr;
    Priority_Queue_Node * tmp,*priority_node= nullptr;
    int priority_code=0;
    if(current->getPredicate()->is_filter()){
        return;
    }
    if(size==1){
        if (AreUsedArray(current->getPredicate()->getArray1()->getArray(),current->getPredicate()->getArray2()->getArray())){
            current->getPredicate()->setfilter(true);
        }
        if(current->getPredicate()->getArray1()->equal(last_joined[0]) || current->getPredicate()->getArray1()->equal(last_joined[1])){
            current->getPredicate()->setSorted(true);
        }
        if(current->getPredicate()->getArray2()->equal(last_joined[0]) || current->getPredicate()->getArray2()->equal(last_joined[1])){
            current->getPredicate()->setSorted(true);
        }
        return;
    }
    current=head;
    while(current){
        if (AreUsedArray(current->getPredicate()->getArray1()->getArray(),current->getPredicate()->getArray2()->getArray())){
            current->getPredicate()->setfilter(true);
            if(current==head) priority_node=previous;
            break;
        }
        else if ((priority_code<4 && IsUsedArray(current->getPredicate()->getArray1()->getArray()))){
            priority_code=3;
            priority_node=previous;
            if(current->getPredicate()->getArray1()->equal(last_joined[0]) || current->getPredicate()->getArray1()->equal(last_joined[1])){
                priority_code=4;
                current->getPredicate()->setSorted(true);
            }
        }
        else if ((priority_code<4 && IsUsedArray(current->getPredicate()->getArray2()->getArray()))){
            priority_code=3;
            priority_node=previous;
            if(current->getPredicate()->getArray2()->equal(last_joined[0]) || current->getPredicate()->getArray2()->equal(last_joined[1])){
                priority_code=4;
                current->getPredicate()->setSorted(true);
            }
        }
        else if (priority_code<2 && AreFilteredArray(current->getPredicate()->getArray1()->getArray(),current->getPredicate()->getArray2()->getArray())){
            priority_code=2;
            priority_node=previous;
        }
        else if ((priority_code<1 && (IsFilteredArray(current->getPredicate()->getArray1()->getArray()) || IsFilteredArray(current->getPredicate()->getArray2()->getArray())))){
            priority_code=1;
            priority_node=previous;
        }
        previous=current;
        current=current->get_next();
    }
    if (!priority_node) return;
    previous=priority_node;
    current=previous->get_next();
    tmp = current->get_next();
    current->setNext(head);
    previous->setNext(tmp);
    head = current;
}

int Priority_Queue::InitRearrange(){
    int filters=0;
    Priority_Queue_Node *current=head,*previous=nullptr;
    Priority_Queue_Node * tmp;
    Priority_Queue_Node * next_tmp;
    if(head->getPredicate()->is_filter()) filters++;
    while(current){
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
   delete used_arrays;
   delete filtered_arrays;
}

bool Priority_Queue::AreUsedArray(int target1,int target2){
    List_Int * temp=used_arrays;
    int count=0;
    while(temp){
        if(temp->getData()==target1 || temp->getData()==target2) count++;
        temp=temp->getNext();
    }
    return count==2 ;
}


bool Priority_Queue::IsUsedArray(int target) {
    List_Int *temp = used_arrays;
    while (temp) {
        if (temp->getData() == target) return true;
        temp = temp->getNext();
    }
    return false;
}

bool Priority_Queue::AreFilteredArray(int target1,int target2){
    List_Int * temp=filtered_arrays;
    while(temp){
        if(temp->getData()==target1 || temp->getData()==target2) {
            return true;
        }
        temp=temp->getNext();
    }
    return false;
}

bool Priority_Queue::IsFilteredArray(int target){
    List_Int *temp = filtered_arrays;
    while (temp) {
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

List_Int::~List_Int() {
    delete next;
}

Priority_Queue_Node::Priority_Queue_Node(Predicate *p, Priority_Queue_Node *n) {
    predicate=p;
    next=n;
}

Priority_Queue_Node *Priority_Queue_Node::get_next() {
    return next;
}

Predicate *Priority_Queue_Node::getPredicate() {
    return predicate;
}

void Priority_Queue_Node::setNext(Priority_Queue_Node *n) {
    Priority_Queue_Node::next = n;
}
