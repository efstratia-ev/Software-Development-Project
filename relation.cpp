#include <cstdint>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "relation.h"
#include "bitmap.h"

using namespace std; 

void handle_errror(char *msg) {
    cout << msg << endl;
    exit(-1);
}

Relation::Relation(const char *filename) {
    int fd = open(filename,O_RDONLY);
    if (fd == -1) {
        char err[] = "relation: open: fail";
        handle_errror(err);
    }
    struct stat sb{};
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
    statistics = new stats*[cols];
    for (int i =0; i < cols; i++)
        statistics[i] = calculateStats(i);
    
}

Relation::~Relation() {
    //set data pointer to be equal to what mmap gave us at constructor
    data--;data--;
    munmap(data,fileSz);
}

stats *Relation::calculateStats(int col) {
    auto colData = get_col(col);
    uint64_t min = colData[0];
    uint64_t max = 0;
    for (int i = 0; i < rows; i++) {
        if (colData[i] < min) {
            min = colData[i];
        }
        if (colData[i] > max) {
            max = colData[i];
        }
    }
    auto bm = new BitMap(max - min + 1,min);
    for (int i = 0; i < rows; i++) {
        bm->set(colData[i]);
    }
    return new stats(min,max,bm->countSetBits());
}

uint64_t Relation::value(uint64_t row, uint64_t col) {
    return data[col * rows + row];
}

uint64_t Relation::getRows() { return rows; }

uint64_t Relation::getCols() { return cols; }


int Relation::compare_values(uint64_t row, uint64_t column1, uint64_t column2) {
    return (int)(value(row,column1)-value(row,column2));
}

int Relation::compare(uint64_t row, uint64_t column, uint64_t v) {
    return (int)(value(row,column)-v);
}

uint64_t *Relation::get_col(uint64_t i) {
    return data + i * rows;
}


uint64_t *Relation::col(uint64_t i,uint64_t &size){
    size=rows;
    return data + i * rows;
}

Relation::Relation(uint64_t *data, uint64_t rows, uint64_t cols) {
    this->data = data;
    this->rows = rows;
    this->cols = cols;
}
