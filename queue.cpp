#include "queue.h"


void queue::push(Job *job) {
    auto node = new qnode(job);
    if (empty()) 
        head = node;
    else 
        tail->next = node;
    tail = node;
    size++;

}

void queue::push(Job *job,sem_t *sem,int val) {
    auto node = new qnode(job,sem,val);
    if (empty())
        head = node;
    else
        tail->next = node;
    tail = node;
    size++;

}

Job *queue::pop() {
    if (empty()) 
        return nullptr;
    auto tmp = &head;
    while(true) {
        if((*tmp)->is_ready()) break;
        tmp=&((*tmp)->next);
        if(!(*tmp)) tmp = &head;
    }
    auto temp=(*tmp)->job;
    *tmp=(*tmp)->next;
    size--;
    if (empty())
        tail = nullptr;
    return temp;
}

bool queue::empty() {
    return size == 0;
}

queue::queue() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

qnode::qnode(Job *job) {
    this->job = job;
    this->next = nullptr;
    sem= nullptr;
    val=-1;
}

qnode::qnode(Job *job, sem_t *sem, int val) {
    this->job=job;
    this->next= nullptr;
    this->sem=sem;
    this->val=val;
}

bool qnode::is_ready() {
    if(!sem) return true;
    int value;
    sem_getvalue(sem,&value);
    return value==val;
}
