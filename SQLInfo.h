#include <cstdint>

class set{
    int array{};
    int column{};
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
    Predicate(int a,int c);
    virtual ~Predicate();
    virtual bool is_filter()=0;
    virtual void setfilter(bool filter)=0;
    virtual bool is_comparison()=0;
    int get_array();
    int get_column();
    virtual char get_comp()=0;
    virtual uint64_t get_value()=0;
    virtual int get_array2()=0;
    virtual int get_column2()=0;
    virtual set * getArray2()=0;
    set *getArray1();
};

class comparison:public Predicate{
    char comp;
    uint64_t num;
public:
    comparison(int a,int c,char cmp,int n);
    bool is_filter() override;
    bool is_comparison() override;
    char get_comp() override;
    uint64_t get_value() override;
    set * getArray2() override;
    int get_array2() override;
    int get_column2() override;
    void setfilter(bool filter) override{}

};

class join:public Predicate{
    set *array2;
    bool isfilter;
public:
    join(int a1,int c1,int a2,int c2);
    ~join() override;
    char get_comp() override;
    bool is_filter() override;
    bool is_comparison() override;
    uint64_t get_value() override;
    int get_array2() override;
    int get_column2() override;
    set *getArray2() override;
    void setfilter(bool filter) override;
};







