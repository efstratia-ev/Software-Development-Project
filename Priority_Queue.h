#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H


#include "SQLInfo.h"


class Priority_Queue_Node{
    Predicate *predicate;
    Priority_Queue_Node * next;
public:
    Priority_Queue_Node(Predicate *p,Priority_Queue_Node *n);
    Priority_Queue_Node* get_next();
    Predicate *getPredicate();
    void setNext(Priority_Queue_Node *next);
    ~Priority_Queue_Node();
    void setPredicateNULL();
    void setPredicate(Predicate *predicate);
};

class List_Int{
    int data;
    List_Int * next;
public:
    List_Int(int d,List_Int * n);
    List_Int *getNext() ;
    ~List_Int();
    int getData();
};


class Priority_Queue{
    int size;
    Priority_Queue_Node * head;
    List_Int * used_arrays;  //needs delete ..
    List_Int * filtered_arrays; //needs delete
    set last_joined[2];
public:
    Priority_Queue();
    void Push(Predicate *p);
    Predicate *Pop();
    void Rearrange();
    int InitRearrange();
    bool IsEmpty();
    bool IsUsedArray(int);
    bool AreUsedArray(int,int);
    bool AreFilteredArray(int,int);
    bool IsFilteredArray(int);
    Predicate * getPredicateI(int i);
    void DeletePQ();
    void RearrangeStats(int * indices,int size);

    ~Priority_Queue();
};

#endif
