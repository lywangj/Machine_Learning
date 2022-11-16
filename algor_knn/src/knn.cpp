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
    bool operator()(data* d1, data* d2) {
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

    std::priority_queue<data *, std::vector<data *>, CompareDistance> pq;
    
    for(int i=0; i<training_data->size(); i++) {

        // std::cout << "find_knearest : " << i << std::endl;

        double dist = calculate_distance(query_pt, training_data->at(i));
        // std::cout << "distance : " << dist << std::endl;
        training_data->at(i)->set_distance(dist);

        // std::cout << "distance (retrieved) : " << training_data->at(i)->get_distance() << std::endl;

        pq.push(training_data->at(i));

        if (pq.size() > k) {
            pq.pop();
        }
    }

    neighbours = new std::vector<data *>;

    // std::cout << "========== check : ==========" << std::endl;

    while (!pq.empty()) {

        neighbours->push_back(pq.top());
        // std::cout << "distance : " << dist << std::endl;
        // std:: cout << pq.top()->get_distance() << std::endl;

        pq.pop();
    }

    // std::cout << "Successfully find " << neighbours->size() \
    //         << "th nearsted training pts." << std::endl; 
}

void knn::set_training_data(std::vector<data *> *vect){

    // std::cout << "aaa size " << vect->size() << std::endl;
    // std::cout << "aaa" << vect->at(0)->get_feature_vector_size() << std::endl;

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

        if(class_freq.find(neighbours->at(i)->get_label()) == class_freq.end()) {

            class_freq[neighbours->at(i)->get_label()] = 1;
        } else {

            class_freq[neighbours->at(i)->get_label()]++;
        }
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

    // std::cout << query_pt->get_feature_vector_size() << std::endl;
    // std::cout << input->get_feature_vector_size() << std::endl;

    if (query_pt->get_feature_vector_size() \
                != input->get_feature_vector_size()) {

        std::cout << "ERROR: Vector size mismatch." << std::endl;
        exit(1);
    }

    // #ifdef EUCLID
    for (unsigned i=0; i<query_pt->get_feature_vector_size(); i++) {

        // std::cout << (int)query_pt->get_feature_vector()->at(i) << std::endl;
        // std::cout << (int)input->get_feature_vector()->at(i) << std::endl;

        distance += pow(query_pt->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
        // std::cout << distance << std::endl;
    }
    distance = sqrt(distance);

    return distance;
    // #elif defined MANHATTAN
    // could be implemented later
    // #endif
}

double knn::validate_performance() {

    double curr_perf{0.0};
    int count{0};
    int data_index{0};

    for (data * query_pt : * validation_data) {

        // std::cout << "data * query_pt : * validation_data" << std::endl;
        // std::cout << query_pt->get_feature_vector_size() << std::endl;

        find_knearest(query_pt);
        int prediction = predict();
        if(prediction == query_pt->get_label()) {

            count++;
        }
        data_index++;
        // std::cout << "Current performance = " << \
        //         ((double)count*100.0)/((double)data_index) << "%" << std::endl;
    }
    curr_perf = ((double)count*100.0)/((double)validation_data->size());
    std::cout << "Validation performance for K = " << k << ":" << curr_perf << "%" << std::endl;

    return curr_perf;
}


double knn::test_performance()  {

    double curr_perf{0.0};
    int count{0};
    int data_index{0};

    for (data * query_pt : * test_data) {

        find_knearest(query_pt);
        int prediction = predict();
        if(prediction == query_pt->get_label()) {

            count++;
        }
        data_index++;
        // std::cout << "Current performance = " << \
        //         ((double)count*100.0)/((double)data_index) << "%" << std::endl;
    }
    curr_perf = ((double)count*100.0)/((double)test_data->size());
    std::cout << "Testing performance = " << curr_perf << "%" << std::endl;

    return curr_perf;
}