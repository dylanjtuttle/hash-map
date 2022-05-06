#include <iostream>
#include <string>
#include "hash_map.hpp"

std::string gen_key(const int len) {
    // Randomly generate an alphanumeric string of length given by the user
    // https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main() {
    srand(time(0));

    // Ceate a new HashMap to hold strings
    HashMap<std::string> new_map;

    // Length of key to generate
    int key_len = 5;

    std::string key_14;
    std::string val_14;

    // Add 20 different values with randomly generated 5 character keys
    for (int i = 0; i < 20; i++) {
        std::string key = gen_key(key_len);
        new_map.put(key, std::to_string(i));

        if (i == 9 || i == 19) {
            // Pretty print the map after adding the first 10 and 20 values
            // Sometimes the map will become full enough that by the 20th value added,
            // the capacity will be increased
            new_map.print();
        }

        if (i == 14) {
            key_14 = key;
            val_14 = std::to_string(i);
        }
    }

    std::cout << "Value for key \"" << key_14 << "\" (should be " << val_14 << "): ";

    try {
        std::cout << new_map.get(key_14) << "\n\n";
    } catch (std::invalid_argument& e) {
        std::cerr << "Not found :(\n\n";
    }

    return 0;
}