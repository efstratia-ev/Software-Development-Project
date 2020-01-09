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

Job *queue::pop() {
    if (empty()) 
        return nullptr;
    auto tmp = head;
    head = head->next;
    size--;
    if (empty()) 
        tail = nullptr;
    return tmp->job;
}

bool queue::empty() {
    return size == 0;
}