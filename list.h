#ifndef LIST
#define LIST
#include <cinttypes>
class listNode{
    char *buffer;
    char *current;
    listNode *next;
    uint64_t count;
    uint64_t max;
public:
    listNode();
    bool isFull();
    void add(uint64_t rowID1,uint64_t rowID2);
    listNode *createNext();
    void print();
    listNode *getnext();
    ~listNode();
};

class list{
    listNode *start;
    listNode *current;
public:
    list();
    void add(uint64_t rowID1,uint64_t rowID2);
    void print();
    ~list();
};

#endif