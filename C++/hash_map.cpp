#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <ctime>
#include <unistd.h>
#include <time.h>
#include "hash_map.hpp"

using namespace std;

template <class T>
class HashVal {
        T value;
        string key;

    public:
        HashVal *next;

        HashVal() {
            key.clear();
            next = NULL;
        }

        void clear() {
            key.clear();
            next = NULL;
        }

        bool is_empty() {
            return key == "" && next == NULL;
        }

        void append(HashVal<T> *new_val) {
            HashVal<T> *next = next;
            while ((*next).next != NULL) {
                next = (*next).next;
            }
            (*next).next = new_val;
        }

        T get_value() {
            return value;
        }

        void set_value(T val) {
            value = val;
        }

        string get_key() {
            return key;
        }

        void set_key(string k) {
            key = k;
        }
};

template <class T>
class HashMap {
    vector< HashVal<T> > map;
    int capacity;

    public:
        HashMap() {
            capacity = 17;

            for (int i = 0; i < capacity; i++) {
                HashVal<T> *new_val = new HashVal<T>();
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

        void put(string key, T value) {
            HashVal<T> *new_val = new HashVal<T>();
            (*new_val).set_key(key);
            (*new_val).set_value(value);

            // If the map is more than 60% full, we need to increase the capacity of the map
            if (size() >= 0.6 * capacity) {
                increase_capacity();
            }

            // Calculate the position of the value in the map and get the HashVal
            int index = hash_function(key);
            HashVal<T> *val = &(map.at(index));

            if ((*val).is_empty()) {
                // If it's empty, replace it with the new val
                map.at(index) = (*new_val);
            } else {
                // If it's not empty, append the new val to the end of the list
                val->append(new_val);
            }
        }

        T get(string key) {
            // Calculate the position of the value in the map and get the HashVal
            int index = hash_function(key);
            HashVal<T> *val = &(map.at(index));

            if ((*val).is_empty()) {
                return NULL;
            } else {
                HashVal<T> *next = val;
                while (next != NULL) {
                    if ((*next).get_key() == key) {
                        return (*next).get_value();
                    }
                    next = (*next).next;
                }
                return NULL;
            }
        }

        void increase_capacity() {
            vector< HashVal<T> > current_data;

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
                
                HashVal<T> *new_val = new HashVal<T>();
                map.push_back(*new_val);
            }

            cout << "Doubling capacity to " << capacity << '\n';

            // Now we can place the data back in their new locations
            for (int i = 0; i < current_data.size(); i++) {
                HashVal<T> val = current_data.at(i);
                int index = hash_function(val.get_key());
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
                HashVal<T> val = map.at(i);
                if (val.is_empty()) {
                    cout << "X\n";
                } else {
                    HashVal<T> *next = &val;
                    while (next != NULL) {
                        cout << "{" << (*next).get_key() << ", " << (*next).get_value() << "} ";
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

    HashMap<int> new_map;

    int key_len = 5;

    for (int i = 0; i < 20; i++) {
        new_map.put(gen_key(key_len), i);

        if (i == 9 || i == 19) {
            new_map.print();
        }
    }

    return 0;
}
