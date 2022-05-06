#include <iostream>
#include <vector>
#include <string>

template<typename T>
class HashMap {
    public:
        HashMap();
        int hash_function(std::string key);
        void put(std::string key, T value);
        T get(std::string key);
        void increase_capacity();
        int size();
        void print();
        int get_capacity();
        
    private:
        typedef struct {
            T value;
            std::string key;
        } HashVal;

        std::vector< std::vector<HashVal> > map;
        int capacity;
};

template<typename T>
HashMap<T>::HashMap() {
    capacity = 16;

    for (int i = 0; i < capacity; i++) {
        std::vector<HashVal> new_val;
        map.push_back(new_val);
    }
}

template<typename T>
int HashMap<T>::hash_function(std::string key) {
    int hash = 0;
            
    // Loop through the characters of the key
    for (int i = 0; i < key.length(); i++) {
        // hash = 33(hash) + key[i]
        hash = ((hash << 5) + hash) + int(key[i]);
    }

    // Ensure the calculated hash falls within the valid capacity of the map
    return hash % capacity;
}

template<typename T>
void HashMap<T>::put(std::string key, T value) {
    // Create a new HashVal instance
    HashVal new_val;
    new_val.key = key;
    new_val.value = value;

    // If the map is more than 60% full, we need to increase the capacity of the map
    if (size() >= 0.6 * capacity) {
        increase_capacity();
    }

    // Calculate the position of the value in the map
    int index = hash_function(key);

    // Append the new HashVal to the end of the vector in the position
    map.at(index).push_back(new_val);
}

template<typename T>
T HashMap<T>::get(std::string key) {
    // Calculate the position of the value in the map and get the vector
    int index = hash_function(key);
    std::vector<HashVal> val = map.at(index);

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

template<typename T>
void HashMap<T>::increase_capacity() {
    // Create a vector to hold the non-empty members of the map
    std::vector< std::vector<HashVal> > current_data;

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

    // Loop from the old capacity to the new capacity and append an empty vector to the map
    for (int i = capacity / 2; i < capacity; i++) {
        std::vector<HashVal> new_val;
        map.push_back(new_val);
    }

    // Now we can place the data back in their new locations
    for (int i = 0; i < current_data.size(); i++) {
        std::vector<HashVal> val = current_data.at(i);

        // Loop through the elements of val and place each HashVal in their new position
        for (int j = 0; j < val.size(); j++) {
            int index = hash_function(val.at(j).key);
            map.at(index).push_back(val.at(j));
        }
    }
}

template<typename T>
int HashMap<T>::size() {
    // Calculate and return the number of non-empty vectors in the map
    int size = 0;

    for (int i = 0; i < capacity; i++) {
        if (!map.at(i).empty()) {
            size++;
        }
    }

    return size;
}

template<typename T>
void HashMap<T>::print() {
    // Pretty print the contents of the entire map
    std::cout << "\nCapacity: " << capacity << '\n';

    // Loop through the map
    for (int i = 0; i < capacity; i++) {
        // Get the current vector
        std::vector<HashVal> val = map.at(i);

        // If the vector is empty, print an "X" to indicate that the vector is empty
        if (val.empty()) {
            std::cout << "X\n";

        } else {
            // If the vector is non-empty, loop through the vector and print each key-value pair
            for (int j = 0; j < val.size(); j++) {
                std::cout << "{" << val.at(j).key << ", " << val.at(j).value << "} ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

template<typename T>
int HashMap<T>::get_capacity() {
    return capacity;
}