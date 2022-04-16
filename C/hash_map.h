#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// There are 9,592 prime numbers which are <= 100,000
#define NUM_PRIMES 9592

// Reference:
// https://www.geeksforgeeks.org/generic-linked-list-in-c-2/
struct bucket {
    char *key;
    void *data;
    struct bucket *next;
};
typedef struct bucket bucket;

typedef bucket *hash_map[99991];
// Hash map functions
extern hash_map *new_hash_map();
extern void put(hash_map, char *, void *, size_t);
extern void *get(hash_map, char *);

extern int primes[];

#endif // HASH_MAP_H