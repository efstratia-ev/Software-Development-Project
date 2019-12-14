#include <iostream>
#include "radix.h"

using  namespace std;

radix::radix(uint64_t offset,uint64_t s, uint64_t *r,uint64_t *_r,uint64_t *d,int b){
    this->offset=offset;
    this->size=s;
    R=r;
    _R=_r;
    this->byte=b;
    data=d;
}

radix::radix(uint64_t s,uint64_t *r,uint64_t *d) {
    this->offset = 0;
    this->size = s;
    this->R = r;
    this->_R = new uint64_t[size];
    this->byte = 1;
    data=d;
}

radix::radix(uint64_t s, uint64_t *d) {
    this->offset = 0;
    this->size = s;
    this->R = nullptr;
    this->_R = new uint64_t[size];
    this->byte = 1;
    data=d;
}
//just a wrapper
void radix::group() {
    if(R) histogram();
    else histogram_init();
    prefixSum();
    reorderR();
}

void radix::histogram() {
    for (unsigned int & i : Hist)
        i = 0;
    for(uint64_t i=offset; i<offset+size; i++) Hist[hash(data[R[i]])]++;
}

void radix::histogram_init() {
    R=new uint64_t[size];
    for (unsigned int & i : Hist)
        i = 0;
    for(uint64_t i=offset; i<offset+size; i++){
        R[i]=i;
        Hist[hash(data[R[i]])]++;
    }
}

void radix::prefixSum() {
    for (unsigned int & i : Psum)
        i = 0;
    Psum[0]=offset;
    for(uint64_t i=1; i<N; i++) Psum[i]=Hist[i-1]+Psum[i-1];

}

void radix::reorderR() {
    uint32_t index,counter[N];
    for (unsigned int & i : counter)
        i = 0;
    for(uint64_t i=offset; i<offset+size; i++){  //for a part of the array
        index=hash(data[R[i]]);
        _R[Psum[index]+counter[index]]=R[i];
        counter[index]++;
    }
}


uint64_t radix::hash(uint64_t value) { //keeps only 1 byte each time
    uint64_t shift=(8-byte)*8;
    value=value>>shift;
    value=value & 0xFFu;
    return value;
}



bool radix::fitsCache(uint64_t i) {
    return Hist[i]*sizeof(uint64_t)<L1size;
}

void radix:: split(stack *Stack) {
    for(uint64_t i=0; i<N; i++){
        if(byte==8) continue;
        //if no more hash is needed
        if(Psum[i]+Hist[i]==0) continue;
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
            Stack->push(new radix(Psum[i],Hist[i],_R,R,data,byte+1)); //hash again
        }
    }
}

void radix::quicksort(uint64_t start, uint64_t end) {
    if(start>=end) return;
    uint64_t pi=partition(start,end);
    if(pi>0) quicksort(start,pi-1);
    quicksort(pi+1,end);
}

uint64_t radix::partition(uint64_t start, uint64_t end) {
    uint64_t pivot=_R[end];
    uint64_t i=start;
    uint64_t temp;

    for(int j=start; j<end; j++){
        if(data[_R[j]]<data[pivot]){
            temp=_R[i];
            _R[i]=_R[j];
            _R[j]=temp;
            i++;
        }
    }
    temp=_R[i];
    _R[i]=_R[end];
    _R[end]=temp;
    return i;
}

array *radix::getR() {
    return new array(size,R);
}

void radix::delete_R() {
    delete[] _R;
}





