#include <fstream>
#include <cstdlib>
#include "inputFunctions.h"

using namespace std;

int getArraySize(string filename,uint64_t & count1,uint64_t & count2){  //finds the size of each array
    ifstream myReadFile;
    int flag=1;
    myReadFile.open(filename);
    char output[100];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> output;
            if(flag){
                count1++;
                flag=0;
            }
            else{
                count2++;
                flag=1;
            }

        }
        myReadFile.close();
        return 1;
    }
    return 0;
}

int makearrays(string filename,mytuple * array1,mytuple * array2){  //makes the two arrays
    int c1=0,c2=0;
    ifstream myReadFile;
    //char output[100];
    string output;
    bool array_who=false;
    myReadFile.open(filename);
    size_t num;
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> output;
            if (!array_who) {
                array1[c1].index=c1;
                array1[c1++].value = stoul(output,&num);
                array_who = true;
            } else {
                array2[c2].index=c2;
                array2[c2++].value = stoul(output,&num);
                array_who = false;
            }
        }
        myReadFile.close();
        return 1;
    }
    return 0;
}
