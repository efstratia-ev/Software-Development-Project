
#include <cstring>
#include "hashtable.h"

hashtable::hashtable(int64_t o,int64_t s, mytuple *r,mytuple *_r,int b ){
    offset=o;
    size=s;
    R=r;
    _R=_r;
    byte=b;
}

void hashtable::initializeHist() {;
    memset(Hist, 0, N);
    for(int64_t i=0; i<size; i++) Hist[hash1(R[i].value)]++;
}

void hashtable::initializePsum() {
    memset(Psum, 0, N);
    for(int64_t i=1; i<N; i++) Psum[i]=Hist[i-1]+Psum[i-1];

}

void hashtable::reorderR() {
    int index,counter[N];
    memset(counter, 0, N);
    for(int i=offset; i<size; i++){
        index=hash1(R[i].value);
        _R[Psum[index]+counter[index]]=R[i];
        counter[index]++;
    }
}


int64_t hashtable::hash1(int64_t value) {
    int shift=(8-byte)*8;
    value=value>>shift;
    value=value & 0xFF;
    return value;
}



bool hashtable::fit_cache(int i) {
    return Hist[i]*sizeof(int64_t)<64000*sizeof(char);
}

void hashtable::split(stack *Stack) {
    for(int i=0; i<N; i++){
        if(fit_cache(i) || byte==7) continue; //quickshort
        Stack->push(new hashtable(Psum[i],Hist[i],_R,R,byte+1));
    }
}

void hashtable::quickshort(int start, int end) {
    if(start==end) return;
    int pi=partition(start,end);
    quickshort(start,pi-1);
    quickshort(pi+1,end);
}

int hashtable::partition(int start, int end) {
    mytuple pivot=R[end];
    int i=start-1;
    mytuple temp;

    for(int j=start; j<=end; j++){
        if(R[j].value<pivot.value){
            temp=R[i];
            R[i]=R[j];
            R[j]=temp;
            i++;
        }
    }
    temp=R[i+1];
    R[i+1]=R[end];
    R[end]=temp;
    return i+1;
}

