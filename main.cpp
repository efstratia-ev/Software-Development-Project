#include<iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include "radix.h"
#include "sort.h"
#include "relations.h"
#include "results_list.h"
#include "JoinArray.h"
#include "join_preds.h"
#include "job.h"
#include "jobscheduler.h"
//#include "SQL.h"

// sed -i 's/,/ /g' filename

//using namespace std;
JobScheduler *js;

char *create_outfileName(char *filename) {
    int i;
    for (i = strlen(filename); i > 0; i--)
        if (filename[i] == '.') break;
    char *outfile = new char[i + 10];
    strncpy(outfile, filename, i + 1);
    strcpy(outfile + i + 1, "myresult");
    return outfile;
}

//process every batch (each batch is seperated by 'F') of queries in parallel.
void DoQueries(Relations *rels) {
    char *line = nullptr;
    size_t size = 0;
    js = new JobScheduler();
    js->Init(4);
    auto *resultsList=new results_list();
    while (true) {
        getline(&line, &size, stdin);
        line = strtok(line, "\n");
        if (!line) continue;
        if (strcmp(line, "Done") == 0) break;
        else if (strcmp(line, "F") == 0) {
            js->Barrier();
            resultsList->print();
            resultsList->clear();
        }
        else {
            //we need to create a new Relations instance for each query
            //because query_rels field varies between them and we need to process
            //queries in parallel.
            auto tmpRels = new Relations(rels->getRels(),rels->getSize());
            //TODO:maybe give threads the task of parsing the query?
            //If there is much complexity, we should definetely.
            auto sql = new SQL(line,tmpRels);
            uint64_t *sums=new uint64_t[sql->get_results_counter()];
            auto job = new QueryJob(sql, tmpRels,sums);
            resultsList->add(sql->get_results_counter(),sums);
            js->Schedule(job);
        }
    }
    delete resultsList;
    free(line);
}

int main(int argc, char *argv[]) {

    char *filename;
    if (argc != 2) {  //get filename for init
        cout << "Wrong amount of arguments\n";
        exit(-1);
    }
    filename = argv[1];

    auto *relations = new Relations(filename);  //data
    DoQueries(relations);
    delete relations;
    return 0;
}
