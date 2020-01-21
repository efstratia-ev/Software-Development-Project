#include "queue.h"

void queue::pushJoin(Job *job) {
    if (empty()){
        auto node = new qnode(job);
        head = node;
        tail= node;
    }
    else if(head->job->add((JoinJob*)job)){
        return;
    }
    else{
        auto node = new qnode(job);
        node->next=head;
        head = node;
    }
    size++;
}

void queue::push(Job *job) {
    auto node = new qnode(job);
    if (empty()) 
        tail= node;
    else 
        node->next=head;
    head = node;
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
    if(head->is_ready()){
        if(tail==head) tail= nullptr;
        auto temp=head->job;
        auto tmp=head;
        head=tmp->next;
        delete tmp;
        size--;
        return temp;
    }
    auto tmp = head;
    while(tmp->next){
        if(tmp->next->is_ready()){
            break;
        }
        tmp=tmp->next;
    }
    if(!tmp->next) return nullptr;
    if(tmp->next==tail) tail=tmp;
    auto temp=tmp->next->job;
    auto del=tmp->next;
    tmp->next=del->next;
    delete del;
    size--;
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

void queue::push(queue *q) {
    if (empty()) {
        head = q->head;
        tail = q->tail;
    }
    else{
        tail->next = q->head;
        tail=q->tail;
    }
    size+=q->size;
    delete q;
}

int queue::getSize() { return size; }

qnode *queue::getHead() { return head; }

qnode *queue::getTail() { return tail; }

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

qnode::~qnode() {
    delete sem;
}
