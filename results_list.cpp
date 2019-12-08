#include "results_list.h"

results_node::results_node(JoinArray *a, results_node *n) {
    results=a;
    next=n;
}

results_node::~results_node() {
    delete next;
}

results_list::results_list() {
    counter=0;
    start=NULL;
}

results_list::~results_list() {
    delete start;
}

void results_list::add(JoinArray *results) {
    start=new results_node(results,start);
    counter++;
}

void results_list::clear() {
    delete start;
    start=NULL;
    counter=0;
}
