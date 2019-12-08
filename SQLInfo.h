class set{
    int array;
    int column;
public:
    set();
    set(int a, int c);

    int getArray() const;

    int getColumn() const;

    void setArray(int array);
    void setColumn(int column);
};

class Predicate{
    set *array1;
public:
    Predicate(int a,int c){
        array1=new set(a,c);
    }
    virtual bool is_filter()=0;
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
};



class node{
    Predicate *predicate;
    node * next;
public:
    node(Predicate *p,node *n){
        predicate=p;
        next=n;
    }
    node* get_next(){
        return next;
    }
    Predicate *getPredicate(){
        return predicate;
    }
};

class Priority_Queue{
    int size;
    node * head;

public:
    Priority_Queue();
    void Push(Predicate *p);
    Predicate *Pop();
    //void Rearrange(int,int);
    bool IsEmpty();
};



