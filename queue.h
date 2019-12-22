#ifndef QUEUE
#define QUEUE
#include <iostream>
#include "job.h"

//using namespace std;


class qnode {
    public:
    Job *job;
    qnode *next;
    qnode(Job *job) {
        this->job = job;
        this->next = nullptr;
    }
};

class queue {
    qnode *head;
    qnode *tail;
    int size;
    public:
    queue() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    void push(Job *job); 
    Job *pop();
    bool empty();
    //getters and print for debug only
    int getSize() { return size; }
    qnode *getHead() { return head; }
    qnode *getTail() { return tail; }
    void print() {
        auto tmp = head;
        while (tmp != nullptr) {
            cout << tmp->job << endl;
            tmp = tmp->next;
        }
    }
};

#endif