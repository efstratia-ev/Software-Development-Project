#include "do_query.h"

//check array if they have been filtered
int isRelationFiltered(JoinArray **filtered, int max, int relation) {
    for (int j = 0; j < max; j++) {
        if (filtered[j]->exists(relation)) {
            return j;
        }
    }
    return -1;
}

uint64_t *DoQuery(SQL *sql, Relations *relations) {
    bool results_exist = true;
    uint64_t *sums = NULL;
    Predicate *predicate = nullptr;
    JoinArray *results = nullptr, **filter_results;
    int filters = sql->get_filters_num(), max = 0;
    filter_results = new JoinArray *[filters];
    bool exists;
    for (int i = 0; i < filters; i++) { //filters arrays
        int curr;
        delete predicate;
        predicate = sql->getPredicate();  //next filter
        exists = false;
        if ((curr = isRelationFiltered(filter_results, max, predicate->get_array())) >= 0) exists = true;
        if (!exists) {
            curr = max;
            filter_results[curr] = new JoinArray(relations);
            max++;
        }
        if (predicate->is_comparison()) {
            int comparison = predicate->get_comp();
            if (comparison == '=') {
                if (exists) filter_results[curr]->equal(predicate->get_column(), predicate->get_value());
                else
                    filter_results[curr]->create_array(
                            relations->equal(predicate->get_array(), predicate->get_column(), predicate->get_value()),
                            predicate->get_array());
            } else if (comparison == '>') {
                if (exists) filter_results[curr]->grater_than(predicate->get_column(), predicate->get_value());
                else
                    filter_results[curr]->create_array(
                            relations->grater_than(predicate->get_array(), predicate->get_column(),
                                                   predicate->get_value()), predicate->get_array());
            } else {
                if (exists) filter_results[curr]->less_than(predicate->get_column(), predicate->get_value());
                else
                    filter_results[curr]->create_array(
                            relations->less_than(predicate->get_array(), predicate->get_column(),
                                                 predicate->get_value()), predicate->get_array());
            }
            if (filter_results[curr]->getSize() == 0) {
                results_exist = false;
                break;
            }
            continue;
        }
        if (exists)
            filter_results[curr]->compare(predicate->get_array(), predicate->get_column(), predicate->get_column2());
        else
            filter_results[curr]->create_array(
                    relations->filter(predicate->get_array(), predicate->get_column(), predicate->get_column2()),
                    predicate->get_array());
    }
    delete predicate;
    list *res;
    while (results_exist && (predicate = sql->getPredicate())) {  //joins between relation
        if (results && results->getSize() == 0) {
            delete predicate;
            results_exist = false;
            break;
        }
        if (predicate->is_filter()) {
            results->compare(predicate->get_array(), predicate->get_column(), predicate->get_array2(),
                             predicate->get_column2());
            delete predicate;
            continue;
        }
        int array1 = predicate->get_array(), array2 = predicate->get_array2();
        if (!results) {
            int curr = isRelationFiltered(filter_results, max, array1);
            if (curr != -1) {
                results = filter_results[curr];
                filter_results[curr] = filter_results[max - 1];
                max--;
            } else {
                curr = isRelationFiltered(filter_results, max, array2);
                if (curr != -1) {
                    results = filter_results[curr];
                    filter_results[curr] = filter_results[max - 1];
                    max--;
                }
            }
        }
        if (results && results->exists(array1)) {
            int curr = isRelationFiltered(filter_results, max, array2);
            if (curr == -1) {
                if (predicate->getSorted())
                    res = results->sortedJoin(array1, predicate->get_column(), array2, predicate->get_column2());
                else res = results->Join(array1, predicate->get_column(), array2, predicate->get_column2());
                results->update_array(res, array2);
            } else {
                if (predicate->getSorted())
                    res = results->sortedJoin(array1, predicate->get_column(), filter_results[curr], array2,
                                              predicate->get_column2());
                else
                    res = results->Join(array1, predicate->get_column(), filter_results[curr], array2,
                                        predicate->get_column2());
                results->update_array(res, filter_results[curr]);
                delete filter_results[curr];
                filter_results[curr] = filter_results[max - 1];
                max--;
            }
            delete predicate;
            continue;
        }
        if (results && results->exists(array2)) {
            int curr = isRelationFiltered(filter_results, max, array1);
            if (curr == -1) {
                if (predicate->getSorted())
                    res = results->sortedJoin(array2, predicate->get_column2(), array1, predicate->get_column());
                else res = results->Join(array2, predicate->get_column2(), array1, predicate->get_column());
                results->update_array(res, array1);
            } else {
                if (predicate->getSorted())
                    res = results->sortedJoin(array2, predicate->get_column2(), filter_results[curr], array1,
                                              predicate->get_column());
                else
                    res = results->Join(array2, predicate->get_column2(), filter_results[curr], array1,
                                        predicate->get_column());
                results->update_array(res, filter_results[curr]);
                delete filter_results[curr];
                filter_results[curr] = filter_results[max - 1];
                max--;
            }
            delete predicate;
            continue;
        }
        auto arr1 = sort(
                new radix(relations->get_relRows(array1), relations->get_column(array1, predicate->get_column())));
        auto arr2 = sort(
                new radix(relations->get_relRows(array2), relations->get_column(array2, predicate->get_column2())));
        list *resultlist = join(arr1, arr2, relations->get_column(array1, predicate->get_column()),
                                relations->get_column(array2, predicate->get_column2()));
        results = new JoinArray(relations);
        results->create_array(resultlist, array1, array2);
        delete[] arr1->Array;
        delete[] arr2->Array;
        delete arr1;
        delete arr2;
        delete predicate;
    }
    //joinPredicates(filter_results,sql,relations,max);
    if (!results_exist) {
        for(int i=0; i<max; i++) delete filter_results[i];
        delete[] filter_results;
        delete results;
        return sums;
    }
    if (!results) {
        results = filter_results[0];
    }
    int res_counter = sql->get_results_counter();
    sums = new uint64_t[res_counter];
    set *select = sql->get_select();
    for (int i = 0; i < res_counter; i++) {
        sums[i] = results->get_sum(select[i].getArray(), select[i].getColumn());
    }
    delete[] filter_results;
    delete results;
    return sums;
}