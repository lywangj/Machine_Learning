#include "data_handler.h"

#define HEADER_NUM = 4
#define HEADER_SIZE = 4
#define NUM_IMAGES_IDX = 1
#define NUM_ROW_IDX = 2
#define NUM_COL_IDX = 3
#define PIXEL_SIZE = 1
#define PIXEL_IDX = 0
#define LABEL_HEADER_NUM = 2

data_handler::data_handler() {

    data_array = new std::vecctor<data *>;
    training_data = new std::vecctor<data *>;
    test_data = new std::vecctor<data *>;
    validation_data = new std::vecctor<data *>;
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

        std::cout << "Done getting input header." << endl;
        int image_size = header[NUM_ROW_IDX]*header[NUM_COL_IDX];
        int image_num = header[NUM_IMAGES_IDX];

        for (int j=0; j<image_num; ++j) {

            data *d = new data();
            uint8_t element[PIXEL_SIZE];      // 1 Hex-bit

            for (int k=0; k<image_size; k++) {

                if(fread(element, sizeof(element), 1, f)) {

                    d->append_to_feature_vector(element[PIXEL_IDX]);
                }else {
                    std::cout << "ERROR: Incorrect file content." << endl;
                    exit(1);
                }
            }
            data_array->push_back(d);
        }
        std::cout << "Successfully read and stored " << data_array->size() 
                << " feature vectors." << endl;
    }
    else {

        std::cout << "ERROR: Incorrect input directory." << endl;
        exit(1);
    }
}

void data_handler::read_feature_labels(std::string path) {

    uint32_t header[LABEL_HEADER_NUM];       // 2 headers
    unsigned char bytes[HEADER_SIZE];        // 4 Hex-bit

    FILE *f = fopen(path.c_str(), "r");
    if(f) {

        for (int i=0; i<LABEL_HEADER_NUM; ++i) {

            if(fread(bytes, sizeof(bytes), 1, f)) {
                header[i] = convert_to_little_endian(bytes);
            }
        }

        std::cout << "Done getting input label header." << endl;
        int image_num = header[NUM_IMAGES_IDX];

        for (int j=0; j<image_num; ++j) {

            uint8_t element[PIXEL_SIZE];      // 1 Hex-bit

            if(fread(element, sizeof(element), 1, f)) {

                data_array[i]->set_label(element[PIXEL_IDX]);
            }else {
                std::cout << "ERROR: Incorrect label file content." << endl;
                exit(1);
            }
        }
        std::cout << "Successfully read and stored " << data_array->size() 
                << " feature labels." << endl;
    }
    else {

        std::cout << "ERROR: Incorrect input directory of label file." << endl;
        exit(1);
    }
}

void data_handler::split_data();
void data_handler::count_classes();

uint32_t data_handler::convert_to_little_endian(const unsigned char *bytes) {

    return (unit32_t) ((bytes[0] << 24) |
                        (bytes[1] << 16) |
                        (bytes[2] << 8) |
                        (bytes[3])); 
}

std::vecctor<data *> *data_handler::get_training_data();
std::vecctor<data *> *data_handler::get_test_data();
std::vecctor<data *> *data_handler::get_validation_data();