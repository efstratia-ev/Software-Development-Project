#ifndef BITMAP
#define BITMAP
#include <cstdint>
#include <cstring>
#include <bitset>
#include <cstdint>

using namespace std;

#define MAX_SIZE 50000000 //in bits

//population count of `byte`.
int popcount(char byte) {
    int sum = 0;
    while (byte > 0) {
        sum += byte&1;
        byte = byte >> 1;
    }
    return sum;
}

class BitMap {
    char *bytes;
    int bytesSize;
    int size;
    uint64_t min;
    public:
    BitMap(int sz) {
        if (sz > MAX_SIZE) 
            size = MAX_SIZE;
        else 
            size = sz;
        bytesSize = size/8+1; //+1 here is like ceil
        bytes = new char[bytesSize]; 
        memset(bytes,0,bytesSize);
    }
    void set(int i) {
        i = zeroBased(i);
        int index = i/8;
        char mask = char(1 << 7 - (i%8));
        bytes[index] |= mask;
    }
    bool get(int i) {
        i = zeroBased(i);
        int index = i/8;
        char mask = char(1 << 7 - (i%8));
        return bytes[index]&mask > 0;
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
            sum += popcount(bytes[i]);
        }
        return sum;
    }
};

#endif