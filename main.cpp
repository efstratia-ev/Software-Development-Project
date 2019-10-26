#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include "hashtable.h"
#include "stack.h"
#include "inpoutFunctionsLibrary.h"

using namespace std;

void SortMergeJoin(hashtable *H){
    stack *Stack=new stack();
    Stack->push(H);
    hashtable *currentHash;
    while(Stack->notEmpty()){
        currentHash=Stack->pop();
        currentHash->split(Stack);
    }
}

int main(int argc, char *argv[]) {
    int count1=0,count2=0;
    struct mytuple *array1,*array2,*_array1,*_array2;
    int n=25;
    string filename;
	filename="text.txt";
    if(argc>1)
		filename=argv[1];
    if(argc>2){
		n=stoi(argv[2]);
		if(2<<(n-1)<0)
			n=20;
	}

    if(!getArraySize(filename,count1,count2)) {
        cout<<"Could not open file.\n";
        return 1;
    }
    array1=new mytuple[count1];
    array2=new mytuple[count2];
    if(!makearrays(filename,array1,array2)) {
        cout<<"Could not open file.\n";
        return 1;
    }
    _array1=new mytuple[count1];
    _array2=new mytuple[count2];
    hashtable H1(0,count1,array1,_array1,1);
    H1.initializeHist();
    H1.initializePsum();
    H1.reorderR();
    return 0;
}
