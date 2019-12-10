#include "results_list.h"





results_list::results_list() {
    counter=0;
    start=NULL;
}

results_list::~results_list() {
    delete start;
}


void results_list::clear() {
    delete start;
    start=NULL;
    counter=0;
}
