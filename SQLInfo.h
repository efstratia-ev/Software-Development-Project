#include <cstdint>

class set{
    int array;
    int column;
public:
    set();
    set(int a, int c);
    int getArray();
    int getColumn();
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
    virtual void setfilter(bool filter)=0;
    virtual bool is_comparison()=0;
    int get_array(){
        return array1->getArray();
    }
    int get_column(){
        return array1->getColumn();
    }
    virtual char get_comp()=0;
    virtual uint64_t get_value()=0;
    virtual int get_array2()=0;
    virtual int get_column2()=0;
    virtual set * getArray2()=0;
    set *getArray1(){
        return array1;
    }
};

class comparison:public Predicate{
    char comp;
    uint64_t num;
public:
    comparison(int a,int c,char cmp,int n):Predicate(a,c){
        comp=cmp;
        num=n;
    }
    bool is_filter(){
        return true;
    }
    bool is_comparison(){
        return true;
    }
    char get_comp(){
        return comp;
    }
    uint64_t get_value(){
        return num;
    }
    set * getArray2(){return NULL;}

    int get_array2(){
        return -1;
    }
    int get_column2(){
        return -1;
    }
    void setfilter(bool filter){}

};

class join:public Predicate{
    set *array2;
    bool isfilter;
public:
    join(int a1,int c1,int a2,int c2):Predicate(a1,c1){
        array2=new set(a2,c2);
        isfilter = false;
    }
    char get_comp(){
        return '\0';
    }
    bool is_filter(){
        return isfilter;
    }
    bool is_comparison(){
        return false;
    }
    uint64_t get_value(){
        return -1;
    }
    int get_array2(){
        return array2->getArray();
    }
    int get_column2(){
        return array2->getColumn();
    }

    set *getArray2() {
        return array2;
    }
    void setfilter(bool filter){
        isfilter=filter;
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
    List_Int *getNext() ;
    int getData();
};


class Priority_Queue{
    int size;
    Priority_Queue_Node * head;
    List_Int * used_arrays;  //needs delete ..
    List_Int * filtered_arrays; //needs delete

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

    ~Priority_Queue();
};



