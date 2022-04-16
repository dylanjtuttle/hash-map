#include "hash_map.h"

int main() {
    // Create a new hash map
    hash_map hash_map;

    // Create and place a value into the hash map
    char *value1 = "value_1";
    put(hash_map, "key_1", value1, strlen(value1));

    // Create and place a value into the hash map
    int value2 = 2;
    put(hash_map, "key_2", &value2, 4);

    // Retrieve value 1 from the hash map
    char *output1 = get(hash_map, "key_1");
    printf("output1: %s\n", output1);

    // Retrieve value 2 from the hash map
    int *output2 = get(hash_map, "key_2");
    printf("output2: %d\n", *output2);

    return 0;
}