#include <iostream>
#include <cstring>
#include "radix.h"

using  namespace std;

radix::radix(uint64_t offset,uint64_t size, mytuple *r,mytuple *_r,int byte){
    this->offset=offset;
    this->size=size;
    R=r;
    _R=_r;
    this->byte=byte;
}

radix::radix(uint64_t size,mytuple *r) {
    this->offset = 0;
    this->size = size;
    this->R = r;
    this->_R = new struct mytuple[size];
    this->byte = 1;
}


//just a wrapper
void radix::group() {

    histogram();
    prefixSum();
    reorderR();
}

void radix::histogram() {
    for (int i = 0; i < N; i++)
        Hist[i] = 0;
    for(uint64_t i=offset; i<offset+size; i++) Hist[hash(R[i].value)]++;
}

void radix::prefixSum() {
    for (int i = 0; i < N; i++)
        Psum[i] = 0;
    Psum[0]=offset;
    for(uint64_t i=1; i<N; i++) Psum[i]=Hist[i-1]+Psum[i-1];

}

void radix::reorderR() {
    uint32_t index,counter[N];
    for (int i = 0; i < N; i++)
        counter[i] = 0;
    for(uint64_t i=offset; i<offset+size; i++){  //for a part of the array
        index=hash(R[i].value);
        _R[Psum[index]+counter[index]]=R[i];
        counter[index]++;
    }
}


uint64_t radix::hash(uint64_t value) { //keeps only 1 byte each time
    uint64_t shift=(8-byte)*8;
    value=value>>shift;
    value=value & 0xFF;
    return value;
}



bool radix::fitsCache(uint64_t i) {
    return Hist[i]*sizeof(uint64_t)<L1size;
}

void radix:: split(stack *Stack) {
    for(uint64_t i=0; i<N; i++){
        if(byte==8) continue;
        //if no more hash is needed
        if(fitsCache(i)){  
            quicksort(Psum[i],Psum[i]+Hist[i]-1);
            //copy only if we wrote to _R
            if ((byte %2) == 1) {
                for(int j=Psum[i]; j<Psum[i]+Hist[i]; j++)
                    R[j]=_R[j];
            }
        }
        else{
            if(Hist[i]==0) continue;
            Stack->push(new radix(Psum[i],Hist[i],_R,R,byte+1)); //hash again
        }
    }
}

void radix::quicksort(int start, int end) {
    if(start>=end) return;
    int pi=partition(start,end);
    quicksort(start,pi-1);
    quicksort(pi+1,end);
}

int radix::partition(int start, int end) {
    mytuple pivot=_R[end];
    int i=start-1;
    mytuple temp;

    for(int j=start; j<end; j++){
        if(_R[j].value<pivot.value){
            i++;
            temp=_R[i];
            _R[i]=_R[j];
            _R[j]=temp;
        }
    }
    temp=_R[i+1];
    _R[i+1]=_R[end];
    _R[end]=temp;
    return i+1;
}

void radix::print() {
    for(uint64_t i=0; i<size; i++){
        cout<<R[i].value<<"\t"<<_R[i].value<<endl;
    }
}

