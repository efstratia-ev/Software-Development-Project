#include <fstream>
#include <cstdlib>
#include<iostream>
#include "inputFunctions.h"

using namespace std;

int getArraySize(string filename,uint64_t & count){  //finds the size of array
    ifstream myReadFile;
    myReadFile.open(filename);
    char output1[100],output2[100];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> output1 >> output2;
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

int makeArray(string filename,mytuple * array1,uint64_t size){  //makes the array
    int c=0;
    ifstream myReadFile;
    string output1,output2;
    myReadFile.open(filename);
    size_t num;
    if (myReadFile.is_open()) {
        for(int i=0;i<size;i++){
            myReadFile >> output1 >> output2;
            array1[c].index=c;
            array1[c++].value = stoul(output1,&num);
        }
        myReadFile.close();
        return 1;
    }
    return 0;
}

int makeArrays(string filename1,mytuple * array1,uint64_t count1,string filename2,mytuple * array2,uint64_t count2){
    if(!makeArray(filename1,array1,count1)) {
        cout<<"could not open file.\n";
        return 0;
    }
    if(!makeArray(filename2,array2,count2)) {
        cout<<"could not open file.\n";
        return 0;
    }
    return 1;
}

