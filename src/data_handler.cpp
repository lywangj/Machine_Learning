#include "data_handler.h"

#define HEADER_NUM 4
#define HEADER_SIZE 4
#define NUM_IMAGES_IDX 1
#define NUM_ROW_IDX 2
#define NUM_COL_IDX 3
#define PIXEL_SIZE 1
#define PIXEL_IDX 0
#define LABEL_HEADER_NUM 2

data_handler::data_handler() {

    data_array = new std::vector<data *>;
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}

data_handler::~data_handler() {

}

void data_handler::read_feature_vector(std::string path) {

    uint32_t header[HEADER_NUM];             // 4 headers
    unsigned char bytes[HEADER_SIZE];        // 4 Hex-bit

    FILE *f = fopen(path.c_str(), "r");
    if(f) {

        for (int i=0; i<HEADER_NUM; ++i) {

            if(fread(bytes, sizeof(bytes), 1, f)) {
                header[i] = convert_to_little_endian(bytes);
            }
        }

        std::cout << "Done getting input header." << std::endl;
        int image_size = header[NUM_ROW_IDX]*header[NUM_COL_IDX];
        int image_num = header[NUM_IMAGES_IDX];

        for (int j=0; j<image_num; ++j) {

            data *d = new data();
            uint8_t element[PIXEL_SIZE];      // 1 Hex-bit

            for (int k=0; k<image_size; k++) {

                if(fread(element, sizeof(element), 1, f)) {

                    d->append_to_feature_vector(element[PIXEL_IDX]);   // 0
                }else {
                    std::cout << "ERROR: Incorrect file content." << std::endl;
                    exit(1);
                }
                // std::cout << k << " data with " << d->get_feature_vector_size() << std::endl;
            }
            data_array->push_back(d);
        }
        std::cout << "Successfully read and stored " << data_array->size() 
                << " feature vectors." << std::endl;
    }
    else {

        std::cout << "ERROR: Incorrect input directory." << std::endl;
        exit(1);
    }
}

void data_handler::read_feature_label(std::string path) {

    uint32_t header[LABEL_HEADER_NUM];       // 2 headers
    unsigned char bytes[HEADER_SIZE];        // 4 Hex-bit

    FILE *f = fopen(path.c_str(), "r");
    if(f) {

        for (int i=0; i<LABEL_HEADER_NUM; ++i) {

            if(fread(bytes, sizeof(bytes), 1, f)) {
                header[i] = convert_to_little_endian(bytes);
            }
        }

        std::cout << "Done getting input label header." << std::endl;
        int image_num = header[NUM_IMAGES_IDX];

        for (int j=0; j<image_num; ++j) {

            uint8_t element[PIXEL_SIZE];      // 1 Hex-bit

            if(fread(element, sizeof(element), 1, f)) {

                data_array->at(j)->set_label(element[PIXEL_IDX]);
            }else {
                std::cout << "ERROR: Incorrect label file content." << std::endl;
                exit(1);
            }
        }
        std::cout << "Successfully read and stored " << data_array->size() 
                << " feature labels." << std::endl;
    }
    else {

        std::cout << "ERROR: Incorrect input directory of label file." << std::endl;
        exit(1);
    }
}

void data_handler::split_data() {

    std::unordered_set<int> used_indexes;

    int training_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int validation_size = data_array->size() * VALIDATION_SET_PERCENT;
    int count{0};

    // training data
    while(count < training_size) {

        int rand_index = rand() % data_array->size();     // 0 & data_array->size() - 1
        if(used_indexes.find(rand_index)==used_indexes.end()) {

            training_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    std::cout << "Training data size: " << training_data->size() << std::endl;

    // test data
    count = 0;
    while(count < test_size) {

        int rand_index = rand() % data_array->size();     // 0 & data_array->size() - 1
        if(used_indexes.find(rand_index)==used_indexes.end()) {

            test_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    std::cout << "Test data size: " << test_data->size() << std::endl;

    // validation data
    count = 0;
    while(count < validation_size) {

        int rand_index = rand() % data_array->size();     // 0 & data_array->size() - 1
        if(used_indexes.find(rand_index)==used_indexes.end()) {

            validation_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }
    std::cout << "Validation data size: " << validation_data->size() << std::endl;

}

void data_handler::count_classes() {

    int count{0};
    for (unsigned i=0; i<data_array->size(); ++i) {

        if(class_map.find(data_array->at(i)->get_label())==class_map.end()) {

            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enumerated_label(count);
            count++;
        }
    }
    num_classes = count;
    std::cout << "Successfully Extracted " << num_classes << " Unique Classes." << std::endl;
}

uint32_t data_handler::convert_to_little_endian(const unsigned char *bytes) {

    return (uint32_t) ((bytes[0] << 24) |
                        (bytes[1] << 16) |
                        (bytes[2] << 8) |
                        (bytes[3])); 
}

std::vector<data *> *data_handler::get_training_data() {

    return training_data;
}
std::vector<data *> *data_handler::get_test_data() {

    return test_data;
}

std::vector<data *> *data_handler::get_validation_data() {

    return validation_data;
}