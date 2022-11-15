#include "data.h"
#include "data_handler.h"

int main() {

    data_handler *dh = new data_handler();

    dh->read_feature_vector("../dataset/train-images-idx3-ubyte");
    dh->read_feature_label("../dataset/train-labels-idx1-ubyte");
    dh->split_data();
    dh->count_classes();
}

// g++ -std=c++17 -I ./include/ ./src/* -o main