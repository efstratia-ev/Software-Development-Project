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
    //TODO: forget about vectors and use lists or be a bad boy 
    vector<char *> lines;
    vector<SQL *> sqlVec;
    while (true) {
        getline(&line, &size, stdin);
        line = strtok(line, "\n");
        if (!line) continue;
        if (strcmp(line, "Done") == 0) break;
        else if (strcmp(line, "F") == 0) {
            auto sumsArr = new uint64_t*[lines.size()];
            int i = 0;
            //SQL *sql;
            for (auto line: lines) {
                //we need to create a new Relations instance for each query
                //because query_rels field varies between them and we need to process
                //queries in parallel.
                auto tmpRels = new Relations(rels->getRels(),rels->getSize());
                //TODO:maybe give threads the task of parsing the query? 
                //If there is much complexity, we should definetely.
                auto sql = new SQL(line,tmpRels);
                sqlVec.push_back(sql);
                auto job = new QueryJob(sql, tmpRels, sumsArr[i]);
                js->Schedule(job);
                i++;
            }
            js->Barrier();
            for (int i =0; i < lines.size(); i++) {
                cout << endl;
                for (int j = 0; j < sqlVec[i]->get_results_counter();j++)  {
                    if (sumsArr[i] == nullptr || sumsArr[i][j]==0)
                        cout << "NULL ";
                    else 
                        cout << sumsArr[i][j] << " ";
                }
            }
            lines.clear();
            sqlVec.clear();
        }
        else {
            auto _line = new char[strlen(line)+1];
            strcpy(_line,line);
            lines.push_back(_line); 
            /*auto sql = new SQL(line);  //translate query
            auto tmpRels = *rels;
            tmpRels.set_query_rels(sql->get_from_arrays()); //keeps query arrays
            */
            
            //relations->set_query_rels(sql->get_from_arrays()); //keeps query arrays
            //results->add(sql->get_results_counter(), join(sql, relations)); // keeps results from join
            //delete sql;
        }
    }

}

int main(int argc, char *argv[]) {

    char *filename;
    if (argc != 2) {  //get filename for init
        cout << "Wrong amount of arguments\n";
        exit(-1);
    }
    filename = argv[1];

    auto *relations = new Relations(filename);  //data
    SQL *sql;
    auto *results = new results_list();

    char *line = nullptr;
    size_t size = 0;
    DoQueries(relations);
    delete results;
    delete relations;
    //free(line);
    //fclose(file);
    return 0;
}
