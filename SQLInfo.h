class set{
    int array;
    int column;
public:
    set();
    set(int a, int c);
    int getArray() const;
    int getColumn() const;
    void setArray(int a);
    void setColumn(int c);
};

class Predicate{
    set *array1;
public:
    Predicate(int a,int c){
        array1=new set(a,c);
    }
    virtual bool is_filter()=0;
    virtual set * getArray2()=0;
    set *getArray1() const {
        return array1;
    }
};

class comparison:public Predicate{
    char comp;
    int num;
public:
    comparison(int a,int c,char cmp,int n):Predicate(a,c){
        comp=cmp;
        num=n;
    }
    bool is_filter(){
        return true;
    }
    set * getArray2(){return NULL;}


};

class join:public Predicate{
    set *array2;
    bool isfilter;
public:
    join(int a1,int c1,int a2,int c2):Predicate(a1,c1){
        array2=new set(a2,c2);
        if(a1==a2) isfilter=true;
        else isfilter=false;
    }
    bool is_filter(){
        return isfilter;
    }

    set *getArray2() {
        return array2;
    }
};



class Priority_Queue_Node{
    Predicate *predicate;
    Priority_Queue_Node * next;
public:
    Priority_Queue_Node(Predicate *p,Priority_Queue_Node *n){
        predicate=p;
        next=n;
    }
    Priority_Queue_Node* get_next(){
        return next;
    }
    Predicate *getPredicate(){
        return predicate;
    }

    void setNext(Priority_Queue_Node *next) {
        Priority_Queue_Node::next = next;
    }
};

class List_Int{
    int data;
    List_Int * next;
public:
    List_Int(int d,List_Int * n);
    List_Int *getNext() const;
    int getData() const;
};


class Priority_Queue{
    int size;
    Priority_Queue_Node * head;
    List_Int * used_arrays;  //needs delete ..

public:
    Priority_Queue();
    void Push(Predicate *p);
    Predicate *Pop();
    void Rearrange();
    void InitRearrange();
    bool IsEmpty();
    bool IsUsedArray(int);
    bool AreUsedArray(int,int);

    ~Priority_Queue();
};



