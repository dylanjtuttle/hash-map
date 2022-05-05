#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include <unistd.h>
#include <time.h>
#include "hash_map.hpp"

using namespace std;

const int NULL_INT = -9999999;

class HashVal {
    public:
        int value;
        string key;
        HashVal *next;

        HashVal() {
            value = NULL_INT;
            key.clear();
            next = NULL;
        }

        void clear() {
            value = NULL_INT;
            key.clear();
            next = NULL;
        }

        bool is_empty() {
            return value == NULL_INT && key == "" && next == NULL;
        }

        void append(HashVal *new_val) {
            HashVal *next = next;
            while ((*next).next != NULL) {
                next = (*next).next;
            }
            (*next).next = new_val;
        }
};

class HashMap {
    vector<HashVal> map;
    int capacity;

    public:
        HashMap() {
            capacity = 17;
            for (int i = 0; i < capacity; i++) {
                HashVal *new_val = new HashVal();
                map.push_back(*new_val);
            }
        }

        // hash algorithm: djb2 by Dan Bernstein
        // http://www.cse.yorku.ca/~oz/hash.html
        // (Found from https://stackoverflow.com/questions/7666509/hash-function-for-string)
        int hash_function(string key) {
            int hash = 0;
            
            for (int i = 0; i < key.length(); i++) {
                hash = (hash << 5) + hash + int(key[i]);
            }

            return hash % capacity;
        }

        void put(string key, int value) {
            HashVal *new_val = new HashVal();
            (*new_val).key = key;
            (*new_val).value = value;

            // If the map is more than 60% full, we need to increase the capacity of the map
            if (size() >= 0.6 * capacity) {
                increase_capacity();
            }

            // Calculate the position of the value in the map and get the HashVal
            int index = hash_function(key);
            HashVal *val = &(map.at(index));

            if ((*val).is_empty()) {
                // If it's empty, replace it with the new val
                map.at(index) = (*new_val);
            } else {
                // If it's not empty, append the new val to the end of the list and put it back in the map
                (*val).append(new_val);
            }
        }

        int get(string key) {
            // Calculate the position of the value in the map and get the HashVal
            int index = hash_function(key);
            HashVal *val = &(map.at(index));

            if ((*val).is_empty()) {
                return NULL_INT;
            } else {
                HashVal *next = val;
                while (next != NULL) {
                    if ((*next).key == key) {
                        return (*next).value;
                    }
                    next = (*next).next;
                }
                return NULL_INT;
            }
        }

        void increase_capacity() {
            vector<HashVal> current_data;

            for (int i = 0; i < capacity; i++) {
                // If the current bucket isn't empty
                if (!map.at(i).is_empty()) {
                    // Store the bucket in the backup and clear it from the map
                    current_data.push_back(map.at(i));
                    map.at(i).clear();
                }
            }

            // We now have backed up and cleared all the data in the map,
            // so we can double the capacity
            capacity *= 2;
            int prime_diff = 0;
            // Search through primes[] for the closest prime number >= to the doubled capacity
            for (int i = 0; i < sizeof(primes); i++) {
                if (primes[i] >= capacity) {
                    prime_diff = primes[i] - capacity;
                    capacity = primes[i];
                    break;
                }
            }

            for (int i = (capacity - prime_diff) / 2; i < capacity; i++) {
                HashVal *new_val = new HashVal();
                map.push_back(*new_val);
            }

            cout << "Doubling capacity to " << capacity << '\n';

            // Now we can place the data back in their new locations
            for (int i = 0; i < current_data.size(); i++) {
                HashVal val = current_data.at(i);
                int index = hash_function(val.key);
                map.at(index) = val;
            }
        }

        int size() {
            int size = 0;
            for (int i = 0; i < capacity; i++) {
                if (!map.at(i).is_empty()) {
                    size++;
                }
            }

            return size;
        }

        void print() {
            cout << "\nCapacity: " << capacity << '\n';

            for (int i = 0; i < capacity; i++) {
                HashVal val = map.at(i);
                if (val.is_empty()) {
                    cout << "X\n";
                } else {
                    HashVal *next = &val;
                    while (next != NULL) {
                        cout << "{" << (*next).key << ", " << (*next).value << "} ";
                        next = (*next).next;
                    }
                    cout << "\n";
                }
            }
            cout << "\n";
        }

        int get_capacity() {
            return capacity;
        }
};

string gen_key(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main() {
    srand(time(0));

    HashMap new_map;

    int key_len = 5;

    for (int i = 0; i < 20; i++) {
        new_map.put(gen_key(key_len), i);

        if (i == 9 || i == 19) {
            new_map.print();
        }
    }

    return 0;
}
