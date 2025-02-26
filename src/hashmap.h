#include <stdint.h>

#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_SIZE 100

typedef struct node_t {
    char key[20];
    void* value;
    struct node_t* next_node;
} node_t;

typedef struct hashmap_t {
    node_t* nodes[TABLE_SIZE];
    int (*cmp)(const char*, const char*);
    uint64_t (*hash)(const char*);

} hashmap_t;


hashmap_t* hashmap_init(int (*cmp_func)(const char*, const char*), uint64_t (*hash)(const char*));

void hashmap_insert(hashmap_t* hashmap, const char* key, void* value);
void hashmap_remove(hashmap_t* hashmap, const char* key);


void* hashmap_get(hashmap_t* hashmap, const char* key);


#endif