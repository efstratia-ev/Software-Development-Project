#ifndef QUEUE
#define QUEUE
#include <iostream>
#include "job.h"

//using namespace std;


class qnode {
    public:
    Job *job;
    qnode *next;
    sem_t *sem;
    int val;
    qnode(Job *job);
    qnode(Job *job,sem_t *sem,int val);
    bool is_ready();
};

class queue {
    qnode *head;
    qnode *tail;
    int size;
    public:
    queue();
    void push(Job *job);
    void push(Job *job,sem_t *sem,int val);
    void push(queue *q);
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