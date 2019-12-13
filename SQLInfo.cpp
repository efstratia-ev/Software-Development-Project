#include <cstdio>
#include "SQLInfo.h"

set::set(int a, int c) {
    array=a;
    column=c;
}

set::set() = default;

int set::getArray(){
    return array;
}

void set::setArray(int a) {
    set::array = a;
}

void set::setColumn(int c) {
    set::column = c;
}

int set::getColumn(){
    return column;
}

bool set::equal(set a) {
    return array==a.array && column==a.column;
}


Predicate::Predicate(int a, int c) {
    array1=new set(a,c);
}

int Predicate::get_array() {
    return array1->getArray();
}

int Predicate::get_column() {
    return array1->getColumn();
}

set *Predicate::getArray1() {
    return array1;
}

Predicate::~Predicate() {
    delete array1;
}

comparison::comparison(int a, int c, char cmp, int n) :Predicate(a,c){
    comp=cmp;
    num=n;
}

bool comparison::is_filter() {
    return true;
}

bool comparison::is_comparison() {
    return true;
}

char comparison::get_comp() {
    return comp;
}

uint64_t comparison::get_value() {
    return num;
}

set *comparison::getArray2() {return nullptr;}

int comparison::get_array2() {
    return -1;
}

int comparison::get_column2() {
    return -1;
}

bool comparison::getSorted() {
    return false;
}

void comparison::setSorted(bool s) {
    return;
}

join::join(int a1, int c1, int a2, int c2) :Predicate(a1,c1){
    array2=new set(a2,c2);
    isfilter = false;
    isSorted=false;
}

char join::get_comp() {
    return '\0';
}

bool join::is_filter() {
    return isfilter;
}

bool join::is_comparison() {
    return false;
}

uint64_t join::get_value() {
    return -1;
}

int join::get_array2() {
    return array2->getArray();
}

int join::get_column2() {
    return array2->getColumn();
}

set *join::getArray2() {
    return array2;
}

void join::setfilter(bool filter) {
    isfilter=filter;
}

join::~join() {
    delete array2;
}

bool join::getSorted() {
    return isSorted;
}

void join::setSorted(bool s) {
    isSorted=s;
}
