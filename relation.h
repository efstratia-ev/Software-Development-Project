#ifndef PROJECT_RELATION_H
#define PROJECT_RELATION_H
#include <cstdint>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void handle_errror(char *msg) {
    cout << msg << endl;
    exit(-1);
}

class Relation {
    uint64_t *data;
    uint64_t rows;
    uint64_t cols;
    uint64_t fileSz;
    public:
    //loads file into memory with mmap
    Relation(const char *filename) {
        int fd = open(filename,O_RDONLY);
        if (fd == -1) {
            char err[] = "relation: open: fail";
            handle_errror(err);
        }
        struct stat sb;
        //find length of file - we need for mmap call
        if (fstat(fd, &sb) == -1)  {
            char err[] = "fstat failed";
            handle_errror(err);
        }
        fileSz = sb.st_size;
        data = (uint64_t *)mmap(nullptr,fileSz,PROT_READ,MAP_PRIVATE,fd,0);
        if (data == MAP_FAILED) {
            char err[] = "mmap failed";
            handle_errror(err);
        }
        close(fd);
        //bytes 0-7 are #rows
        rows = data[0];
        //bytes 8-15 bytes are #cols
        cols = data[1];
        //data should point to actual data - not metadata
        data++;data++;
    }

    ~Relation() {
        //set data pointer to be equal to what mmap gave us at constructor
        data--;data--;
        munmap(data,fileSz);
    }

    //return ith column and sets sz to be the size of the column (zero based)
    uint64_t *col(int i,int *sz) {
        *sz = rows;
        return data + i * rows;
    }

    //return value at row 'row' and col 'col' (zero based)
    uint64_t value(int row,int col) {
        return data[col * rows + row];
    }

    uint64_t getRows() { return rows; }
    uint64_t getCols() { return cols; }
    uint64_t *getData() { return data; }
};


#endif