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
        HashVal(string k, T val) {
            key = k;
            value = val;
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
    vector< vector< HashVal<T> > > map;
    int capacity;

    public:
        HashMap() {
            capacity = 17;

            for (int i = 0; i < capacity; i++) {
                vector< HashVal<T> > new_val;
                map.push_back(new_val);
            }
        }

        // hash algorithm: djb2 by Dan Bernstein
        // http://www.cse.yorku.ca/~oz/hash.html
        // (Found from https://stackoverflow.com/questions/7666509/hash-function-for-string)
        int hash_function(string key) {
            int hash = 0;
            
            // Loop through the characters of the key
            for (int i = 0; i < key.length(); i++) {
                // hash = 33(hash) + key[i]
                hash = ((hash << 5) + hash) + int(key[i]);
            }

            // Ensure the calculated hash falls within the valid capacity of the map
            return hash % capacity;
        }

        void put(string key, T value) {
            // Create a new HashVal object
            HashVal<T> new_val(key, value);

            // If the map is more than 60% full, we need to increase the capacity of the map
            if (size() >= 0.6 * capacity) {
                increase_capacity();
            }

            // Calculate the position of the value in the map
            int index = hash_function(key);

            // Append the new HashVal to the end of the vector in the position
            map.at(index).push_back(new_val);
        }

        T get(string key) {
            // Calculate the position of the value in the map and get the vector
            int index = hash_function(key);
            vector< HashVal<T> > val = map.at(index);

            // If the vector is empty, the value we're searching for clearly isn't there
            if (val.empty()) {
                return nullptr;
            } else {
                // If the vector is non-empty, search through it to see if we can find the value
                for (int i = 0; i < val.size(); i++) {
                    // If the key of this HashVal matches the key we are searching on, return the HashVal's value
                    if (val.at(i).key == key) {
                        return val.at(i).value;
                    }
                }
                // If we have looped through the whole vector and didn't find any matching HashVals,
                // the value we're searching for isn't there
                return nullptr;
            }
        }

        void increase_capacity() {
            // Create a vector to hold the non-empty members of the map
            vector< vector< HashVal<T> > > current_data;

            // Loop through the map
            for (int i = 0; i < capacity; i++) {
                // If the current vector isn't empty
                if (!map.at(i).empty()) {
                    // Store the vector in the backup and empty it
                    current_data.push_back(map.at(i));
                    map.at(i).clear();
                }
            }

            // We now have backed up and cleared all the data in the map,
            // so we can double the capacity
            capacity *= 2;
            int prime_diff = 0;

            // Since the capacity must be a prime number, we need to find the 
            // closest prime number >= the doubled capacity

            // primes[10] = 31, and since the initial capacity of the map is 17,
            // if we're calling this function it must be true that the doubled capacity is >= 34
            int i = 10;
            while (primes[i] < capacity) {
                // Loop until we've found the closest prime number >= the doubled capacity
                i++;
            }
            prime_diff = primes[i] - capacity;  // Now capacity - prime_diff = the old doubled capacity
            capacity = primes[i];

            // Loop from the old capacity to the new (prime) capacity, adding empty vectors to the map
            for (int i = (capacity - prime_diff) / 2; i < capacity; i++) {
                vector< HashVal<T> > new_val;
                map.push_back(new_val);
            }

            // Now we can place the data back in their new locations
            for (int i = 0; i < current_data.size(); i++) {
                vector< HashVal<T> > val = current_data.at(i);

                // Loop through the elements of val and place each HashVal in their new position
                for (int j = 0; j < val.size(); j++) {
                    int index = hash_function(val.at(j).get_key());
                    map.at(index).push_back(val.at(j));
                }
            }
        }

        int size() {
            // Calculate and return the number of non-empty vectors in the map
            int size = 0;

            for (int i = 0; i < capacity; i++) {
                if (!map.at(i).empty()) {
                    size++;
                }
            }

            return size;
        }

        void print() {
            // Pretty print the contents of the entire map
            cout << "\nCapacity: " << capacity << '\n';

            // Loop through the map
            for (int i = 0; i < capacity; i++) {
                // Get the current vector
                vector< HashVal<T> > val = map.at(i);

                // If the vector is empty, print an "X" to indicate that the vector is empty
                if (val.empty()) {
                    cout << "X\n";

                } else {
                    // If the vector is non-empty, loop through the vector and print each key-value pair
                    for (int j = 0; j < val.size(); j++) {
                        cout << "{" << val.at(j).get_key() << ", " << val.at(j).get_value() << "} ";
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
    // Randomly generate an alphanumeric string of length given by the user
    // https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
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

    // Ceate a new HashMap to hold strings
    HashMap<string> new_map;

    int key_len = 5;

    // Add 20 different values with randomly generated 5 character keys
    for (int i = 0; i < 20; i++) {
        new_map.put(gen_key(key_len), to_string(i));

        if (i == 9 || i == 19) {
            // Pretty print the map after adding the first 10 and 20 values
            // Sometimes the map will become full enough that by the 20th value added,
            // the capacity will be increased
            new_map.print();
        }
    }

    return 0;
}
