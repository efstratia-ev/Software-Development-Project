#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include "radix.h"
#include "stack.h"
#include "inputFunctions.h"
#include "sort.h"
#include "array.h"

// sed -i 's/,/ /g' filename

using namespace std;

int main(int argc, char *argv[]) {
    uint64_t count1=0,count2=0;
    struct array *array1,*array2,*_array1,*_array2;
    string filename1,filename2;
    if(argc!=3) {  //get filenames
        cout<<"Wrong amount of arguments\n";
        exit(-1);
    }
	filename1=argv[1];
    filename2=argv[2];
    if(!getArraysSize(filename1,filename2,count1,count2)) //find array's size
        exit(-1);
    array1=new array(count1);
    array2=new array(count2);
    if(!makeArrays(filename1,array1,filename2,array2)) //get array's data
        exit(-1);
    _array1=new array(count1);
    _array2=new array(count2);
    radix r1(0,array1->Size,array1->Array,_array1->Array,1);
    sort(&r1);
    radix r2(0,array2->Size,array2->Array,_array2->Array,1);
    sort(&r2);
    list *resultlist=join(array1,array2);
    resultlist->print();
    //resultlist->printSize();
    //cout<<countResults(array1,array2,count1,count2)<<"\n"; //without saving results to list
    delete array1;
    delete array2;
    delete _array1;
    delete _array2;
    delete resultlist;
    return 0;
}
