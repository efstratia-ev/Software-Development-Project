#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include<iostream>
#include "inputFunctions.h"

using namespace std;

int getArraySize(string filename,uint64_t & count){  //finds the size of array
    ifstream myReadFile;
    myReadFile.open(filename);
    uint64_t output1,output2;
    char c;
    if (myReadFile.is_open()) {
        while (myReadFile>>output1>>c>>output2 && c == ','){
            count++;
        }
        myReadFile.close();
        return 1;
    }
    return 0;
}

int getArraysSize(string filename1,string filename2,uint64_t & count1,uint64_t & count2){
    if(!getArraySize(filename1,count1)) {
        cout<<"could not open file.\n";
        return 0;
    }
    if(!getArraySize(filename2,count2)) {
        cout<<"could not open file.\n";
        return 0;
    }
    return 1;
}

int makeArray(string filename,mytuple * array1){  //makes the array
    int c=0;
    ifstream myReadFile;
    myReadFile.open(filename);
    uint64_t output1,output2;
    char ch;
    if (myReadFile.is_open()) {
        while (myReadFile>>output1>>ch>>output2 && ch == ','){
            array1[c].index=c;
            array1[c].key = output1;
            array1[c++].value = output2;
        }
        myReadFile.close();
        return 1;
    }
    return 0;
}

int makeArrays(string filename1,mytuple * array1,string filename2,mytuple * array2){
    if(!makeArray(filename1,array1)) {
        cout<<"could not open file.\n";
        return 0;
    }
    if(!makeArray(filename2,array2)) {
        cout<<"could not open file.\n";
        return 0;
    }
    return 1;
}

