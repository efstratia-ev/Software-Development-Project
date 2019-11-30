#ifndef LIST
#define LIST
#include <cinttypes>

struct rowids{
    uint64_t rowid1;
    uint64_t rowid2;
    rowids(uint64_t a,uint64_t b){
        rowid1=a;
        rowid2=b;
    }
};
class listNode{
    char *buffer;
    char *current;
    listNode *next;
    uint64_t count;
    uint64_t max;
public:
    listNode();
    bool isFull();
    uint64_t getcount();
    void add(uint64_t rowID1,uint64_t rowID2);
    void add(uint64_t rowID);
    listNode *createNext();
    void print();
    listNode *getnext();
    ~listNode();
    void restart_current();
    rowids* pop();
    uint64_t pop_element();
};

class list{
    uint64_t size;
    listNode *start;
    listNode *current;
public:
    list();
    void add(uint64_t rowID1,uint64_t rowID2);
    void print();
    void printSize();
    ~list();
    uint64_t get_size();
    void restart_current();
    rowids* pop();
    uint64_t pop_element();

    void add(uint64_t rowID);
};

#endif