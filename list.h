class listNode{
    char *buffer;
    char *current;
    listNode *next;
    int64_t count;
    int64_t max;
public:
    listNode();
    bool isFull();
    void add(int64_t rowID1,int64_t rowID2);
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
    void add(int64_t rowID1,int64_t rowID2);
    void print();
    ~list();
};