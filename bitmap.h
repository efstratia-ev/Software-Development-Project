#ifndef BITMAP
#define BITMAP
#include <cstdint>
#include <cstring>
#include <bitset>
#include <cstdint>

using namespace std;

#define MAX_SIZE 50000000 //in bits

class BitMap {
    uint8_t *bytes;
    int size; //size in bits
    int bytesSize; //  == size/8+1 (+1 here is like ceil)
    uint64_t min;
    public:
    BitMap(int sz,int min) {
        this->min = min;
        if (sz > MAX_SIZE) 
            size = MAX_SIZE;
        else 
            size = sz;
        bytesSize = size/8+1; 
        bytes = new uint8_t[bytesSize]; 
        memset(bytes,0,bytesSize);
    }
    ~BitMap() { delete[] bytes; }
    uint8_t *getBytes() { return bytes; }
    void set(int i) {
        i = zeroBased(i);
        int index = i/8;
        uint8_t mask = 1 << 7 - (i%8);
        bytes[index] |= mask;
    }
    bool get(int i) {
        i = zeroBased(i);
        int index = i/8;
        uint8_t mask = 1 << 7 - (i%8);
        return uint8_t(bytes[index]&mask) > 0;
    }
    int zeroBased(int i) {
        i = i - min;
        if (size == MAX_SIZE) //this means that i-min can be greater that size
            return i % MAX_SIZE;
        return i;
    }
    uint64_t countSetBits() {
        uint64_t sum = 0;
        for (int i =0; i < bytesSize; i++) {
            sum += myPopcount(bytes[i]);
        }
        return sum;
    }
    //population count of `byte`.
    int myPopcount(uint8_t byte) {
        int sum = 0;
        while (byte > 0) {
            sum += byte&1;
            byte = byte >> 1;
        }
        return sum;
    }
};

#endif