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

    int key_len = 5;

    // Add 20 different values with randomly generated 5 character keys
    for (int i = 0; i < 20; i++) {
        new_map.put(gen_key(key_len), std::to_string(i));

        if (i == 9 || i == 19) {
            // Pretty print the map after adding the first 10 and 20 values
            // Sometimes the map will become full enough that by the 20th value added,
            // the capacity will be increased
            new_map.print();
        }
    }

    return 0;
}