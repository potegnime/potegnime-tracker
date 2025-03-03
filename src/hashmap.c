#include "hashmap.h"

#include <stdlib.h>
#include <string.h>

#include "logger.h"

hashmap_t* hashmap_init(int (*cmp_func)(const char*, const char*), uint64_t (*hash_func)(const char*)) {

    hashmap_t* hashmap = (hashmap_t*)malloc(sizeof(hashmap_t));
    if (!hashmap)
        return NULL;

    hashmap->cmp = cmp_func;
    hashmap->hash = hash_func;

    for(uint32_t i = 0; i < TABLE_SIZE; i++) {
        hashmap->nodes[i] = NULL;
    }

    return hashmap;
}


void hashmap_insert(hashmap_t* hashmap, const char* key, void* value) {

    if (!hashmap || !key) {
        LOG_ERROR("hashmap_insert(): hashmap || key == NULL");
        return;
    }

    size_t index = hashmap->hash(key) % TABLE_SIZE;

    node_t* current_node = hashmap->nodes[index];
    if (hashmap->nodes[index] != NULL) {

        node_t* current_node = hashmap->nodes[index];
        while(current_node != NULL) {
            if(hashmap->cmp(current_node->key, key) == 0) {
                LOG_DEBUG("hashmap_insert(): node %s already exists. Assigning new value.");
                current_node->value = value;
                return;
            }
            current_node = current_node->next_node;
        }

    }
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node) {
        LOG_ERROR("hashmap_insert(): new_node == NULL");
        return;
    }

    strncpy(new_node->key, key, 20);
    new_node->value = value;
    new_node->next_node = NULL;
    
    if (hashmap->nodes[index] == NULL) {
        hashmap->nodes[index] = new_node;
        return;
    }
       
    while(current_node->next_node != NULL) {
        current_node = current_node->next_node;
    }
    current_node->next_node = new_node;

}

void hashmap_remove(hashmap_t* hashmap, const char* key) {

    if (!hashmap || !key) {
        LOG_ERROR("hashmap_remove(): hashmap || key == NULL");
        return;
    }


    size_t index = hashmap->hash(key) % TABLE_SIZE;

    node_t* current_node = hashmap->nodes[index];
    if(current_node == NULL)
        return;

    if (hashmap->cmp(current_node->key, key) == 0) {
        hashmap->nodes[index] = current_node->next_node;
        free(current_node->value);
        free(current_node);
        return;
    }

    node_t* previous_node = NULL;
    while(current_node != NULL) {

        if (hashmap->cmp(current_node->key, key) == 0 && previous_node != NULL) {
            previous_node->next_node = current_node->next_node;
            free(current_node->value);
            free(current_node);

            break;
        }

        previous_node = current_node;
        current_node = current_node->next_node;
    }

}


void* hashmap_get(hashmap_t* hashmap, const char* key) {

    if (!hashmap || !key) {
        LOG_ERROR("hashmap_get(): hashmap || key == NULL");
        return NULL;
    }

    size_t index = hashmap->hash(key) % TABLE_SIZE;

    node_t* current_node = hashmap->nodes[index];

    while(current_node != NULL) {

        if (hashmap->cmp(current_node->key, key) == 0)
            return current_node->value;

        current_node = current_node->next_node;
    }

    return NULL;
}

