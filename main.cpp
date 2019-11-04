#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include "radix.h"
#include "stack.h"
#include "inputFunctions.h"
#include "sort.h"

// sed -i 's/,/ /g' filename

using namespace std;

int main(int argc, char *argv[]) {
    uint64_t count1=0,count2=0;
    struct mytuple *array1,*array2,*_array1,*_array2;
    string filename1,filename2;
    if(argc!=3) {  //get filenames
        cout<<"Wrong amount of arguments\n";
        exit(-1);
    }
	filename1=argv[1];
    filename2=argv[2];
    if(!getArraysSize(filename1,filename2,count1,count2)) //find array's size
        exit(-1);
    array1=new mytuple[count1];
    array2=new mytuple[count2];
    if(!makeArrays(filename1,array1,count1,filename2,array2,count2)) //get array's data
        exit(-1);
    _array1=new mytuple[count1];
    _array2=new mytuple[count2];
    radix r1(0,count1,array1,_array1,1);
    sort(&r1);
    radix r2(0,count2,array2,_array2,1);
    sort(&r2);
    list *resultlist=finalresults(array1,array2,count1,count2);
    //resultlist->print();
    resultlist->printSize();
    //cout<<countResults(array1,array2,count1,count2)<<"\n"; //without saving results to list
    delete[] array1;
    delete[] array2;
    delete[] _array1;
    delete[] _array2;
    delete resultlist;
    return 0;
}
