#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <unordered_set>
#include <iostream>
#include "data.h"

class data_handler
{
    std::vecctor<data *> *data_array;
    std::vecctor<data *> *training_data;
    std::vecctor<data *> *test_data;
    std::vecctor<data *> *validation_data;

    int num_classes;
    int feature_vector_size;
    std::map<uint8_t, int> class_map;

    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATIOM_SET_PERCENT = 0.5;

public:

    data_handler();
    ~data_handler();

    void read_feature_vector(std::string path);
    void read_feature_labels(std::string path);
    void split_data();
    void count_classes();

    uint32_t convert_to_little_endian(const unsigned char *bytes);

    std::vecctor<data *> *get_training_data();
    std::vecctor<data *> *get_test_data();
    std::vecctor<data *> *get_validation_data();


};

#endif