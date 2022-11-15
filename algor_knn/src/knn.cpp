#include <cmath>
#include <limits>
#include <map>
#include <iostream>
#include <queue>
#include "../include/knn.h"
#include "../../include/data_handler.h"

    // std::vector<data *> *neighbours;
    // std::vector<data *> training_data;
    // std::vector<data *> test_data;
    // std::vector<data *> validation_data;

struct CompareDistance {
    bool operator()(data const& d1, data const& d2) {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return d1->get_distance() < d2->get_distance();
    }
};

knn::knn(int val) {

    k = val;
}

knn::knn() {
    // NA
}
knn::~knn() {
    // NA
}

void knn::find_knearest(data *query_pt) {

    priority_queue<data *, vector<data *>, CompareDistance> pq;
    
    for(int i=0; i<training_data->size(); i++) {

        double dist = calculate_distance(query_pt, training_data->at(i));
        training_data->at(i)->set_distance(dist);

        std:: cout << to_string(training_data->at(i)->get_distance()) << std::endl;

        pq.push(training_data->at(i));

        if (pq.size() > k) {
            pq.pop();
        }
    }

    neighbors = new std::vector<data *>;

    std::cout << "========== check : ==========" << std::endl;

    for (auto d : pq) {

        std:: cout << to_string(d->get_distance()) << std::endl;
        neighbors.push_back(d);
    }

    std::cout << "Successfully find " << neighbors->size() \
            << "th nearsted training pts." << std::endl; 
}

void knn::set_training_data(std::vector<data *> *vect){

    training_data = vect;
}

void knn::set_test_data(std::vector<data *> *vect) {

    test_data = vect;
}

void knn::set_validation_data(std::vector<data *> *vect) {

    validation_data = vect;
}

void knn::set_k(int val) {

    k = val;
}

int knn::predict() {

    std::map<uint8_t, int> class_freq;
    for (int i=0; i<neighbours->size(); ++i) {

        if(class_freq[neighbours->at(i)->get_label()]) = 1;
    }else {

        class_freq[neighbours->at(i)->get_label()]++;
    }

    int best{0};
    int max{0};

    for (auto kv : class_freq) {

        if (kv.second > max) {

            max = kv.second;
            best = kv.first;
        }
    }

    delete neighbours;
    return best;
}

double knn::calculate_distance(data* query_pt, data* input) {

    double distance{0.0};

    if (query_pt->get_feature_vector_size() \
                != input->get_feature_vector_size()) {

        std::cout << "ERROR: Vector size mismatch." << std::endl;
        exit(1);
    }

    #ifdef EUCLID
    for (unsigned i=0; i<query_pt->get_feature_vector_size(); i++) {

        distance += pow(query_pt->get_feature_vector->at(i) \
                        input->get_feature_vector->at(i), 2);
    }
    return sqrt(distance);
    #elif defined MANHATTAN
    // could be implemented later
    #endif
}

double knn::validate_performance() {

    double curr_perf{0.0};
    int count{0};
    int data_index{0};

    for (data * query_pt : * validatioin_data) {

        find_knearest(query_pt);
        int prediction = predict();
        if(prediction == query_pt->get_label()) {

            count++;
        }
        data_index++;
        std::cout << "Current performance = " << \
                ((double)count*100.0)/((double)data_index) << "%" << std::endl;
    }
    curr_perf = ((double)count*100.0)/((double)validation_data->size());
    std::cout << "Validation performance for K = " << k << ":" << curr_perf << "%" << std::endl;

    return curr_perf;
}


double knn::test_performance()  {

    double curr_perf{0.0};
    int count{0};
    int data_index{0};

    for (data * query_pt : * testing_data) {

        find_knearest(query_pt);
        int prediction = predict();
        if(prediction == query_pt->get_label()) {

            count++;
        }
        data_index++;
        std::cout << "Current performance = " << \
                ((double)count*100.0)/((double)data_index) << "%" << std::endl;
    }
    curr_perf = ((double)count*100.0)/((double)validation_data->size());
    std::cout << "Testing performance = " << curr_perf << "%" << std::endl;

    return curr_perf;
}