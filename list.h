class listNode{
    char *buffer;
    char *current;
    listNode *next;
    int count;
    int max;
public:
    listNode();
    bool isFull();
    void add(int rowID1,int rowID2);
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
    void add(int rowID1,int rowID2);
    void print();
    ~list();
};